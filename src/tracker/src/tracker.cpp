#include "tracker.h"

using namespace mctracker::tracker;

Tracker
::Tracker(const KalmanParam& _param, const std::vector<Camera>& camerastack)
    : streams(camerastack)
{
    param = _param;
    rng = cv::RNG(12345);
    trackIds = 1;
    numCams = streams.size();
    if(numCams > 10 || numCams == 0)
    {
        throw std::invalid_argument("The number of cameras has to be: 0 < num of cam <= 10");
    }
    
    auto numClose = 0;
    for(const auto& stream : streams)
    {
        if(stream.getProximity())
            numClose++;
    }
    auto numNotClose = numCams - numClose;

    auto p1 = 1. / numCams;
    auto p2 = .2*p1*numNotClose;
    auto not_close_prob1 = p1 - (p2 / numNotClose);
    auto close_prob = p1 + (p2 / numClose);
    
    for(const auto& stream : streams)
    {
        if(stream.getProximity())
        {
            cameraProbabilities.push_back(close_prob);
        }
        else
        {
            cameraProbabilities.push_back(not_close_prob1);
        }
    }
}

void 
Tracker::refine_detections(std::vector<Detections>& _detections)
{
    auto m = 0;
    for(auto& det : _detections)
    {
        const auto& fov = streams.at(m).getFOV();
        for(int i = int(det.size()) - 1; i >= 0 ; --i)
        {
            if(det.at(i).y() < 0 || det.at(i).x() < 0 || det.at(i).y() > fov.rows || det.at(i).x() > fov.cols ||  
                int(fov.at<uchar>(cv::Point2i(det.at(i).x(), det.at(i).y()))) != 255)
            {
                det.erase(det.begin() + i);
            }
        }
        ++m;
    }
}


Detections 
Tracker::first_assosiation(std::vector<Detections>& observations)
{
    Detections detections;
    std::vector< std::vector<bool> > checked(observations.size());
    
    for(auto i = 0; i < int(observations.size()); ++i)
    {
        checked.at(i).resize(observations.at(i).size(), false);
    }
    
    std::vector< std::vector< std::pair<int, int> > > associations; //camera, idx_detection
    //scroll the observations from the camera i
    for(auto i = 0; i < int(observations.size() - 1); ++i)
    {
        //assign the obeservations
        const auto& points_i = observations.at(i);
        auto m = 0;
        //compare each point of the camera i with all the points of the other cameras
        for(const auto& pi : points_i)
        {
            std::vector< std::pair<int, int> > association;
            //if the point is not already checked
            if(!checked[i][m])
            {
                //insert the point in the local association
                association.push_back(std::make_pair(i, m));
                
                //take the obeservations for the other camera j (i+1)
                for(auto j = i + 1; j < int(observations.size()); j++)
                {
                    const auto& points_j = observations.at(j);
                    auto min_dist = std::numeric_limits<float>::max();
                    auto idx = -1;
                    auto  k = 0;
                    //scroll all the obeservations of the camera j
                    for(const auto& pj : points_j)
                    {
                        //if the point is not checked
                        if(!checked[j][k])
                        {
                            //I compute the distance
                            auto p = cv::Point2f(pi.x() - pj.x(), pi.y() - pj.y());
                            float dist = sqrt( p.x * p.x + p.y * p.y);

                            if(dist < min_dist)
                            {
                                min_dist = dist;
                                idx = k;
                            }
                        }
                        ++k;
                    }
                    
                    //if the distance is less than 30 pixel
                    // the point included in the observesion j 
                    // is added to the point related to the
                    // observation i
                    if(min_dist < 30 && idx != -1)
                    {
                        //set the point as checked
                        checked[j][idx] = true;
                        association.push_back(std::make_pair(j, idx));
                    }
                }
            }
            if(association.size() > 0)
                associations.push_back(association);
            
            checked[i][m] = true;
            ++m;
        }
    }
    
    
    for(const auto& association : associations)
    {
        if(association.size() == 1)
        {
            const auto& idx = association.at(0);
            const auto& obs = observations[idx.first][idx.second];
            Detection d(obs.x(),   obs.y(), obs.w(), obs.h(), obs.hist());
            detections.push_back(d);
        }
        else
        {
            Detection d;
            float prob;
            d.setX(0.);
            d.setY(0.);
            
            for(const auto& ass : association)
            {
                prob = cameraProbabilities.at(ass.first);
                const auto& obs = observations[ass.first][ass.second];
                d.setX(d.x() + prob*obs.x());
                d.setY(d.y() + prob*obs.y());
                if(streams.at(ass.first).getProximity())
                {
                    d.setHist(obs.hist());
                }
                d.setSize(obs.w(), obs.h());
            }
            
            detections.push_back(d);
        }
    }
    
    return detections;
}

void 
Tracker::track(std::vector< Detections >& _detections, const int& w, const int& h)
{
    //prediction
    evolveTracks();
    
    auto d = _detections;
    
    //check if the detections are in the FOV of the cameras
    refine_detections(_detections);
    
    //check if the freezed tracks can be restored
    if(old_tracks.size() > 0)
        check_old_tracks(_detections);


    //assign the new observations to the tracklets
    std::vector<cv::Mat> assignments(_detections.size());
    
    auto i = 0;
    for(const auto& det : _detections)
    {
        assignments.at(i) = associate_tracks(det).clone();

        if(assignments.at(i).total() != 0)
            Hyphothesis::instance()->new_hyphothesis(assignments.at(i), single_tracks, det, w, h, 
                                                                                    param.getNewhypdummycost(), prev_unassigned, param, (streams.size()));
        assignments.at(i).setTo(0, (assignments.at(i) == 255));
    
        i++;
    } 
    

    if(single_tracks.size() == 0)
    {
        //first association between camera points
        const auto& detections = first_assosiation(_detections);
        
        //start new tracks
        for(const auto& t : detections)
        {
            single_tracks.push_back(Track_ptr(new Track(t.x(), t.y(),  param, t.hist(), int(streams.size()))));
        }
    }
    else
    {
        //update the tracks given the assigments
        update_tracks(assignments, _detections);
        //delete or freeze the tracks which have no detections
        delete_tracks();

    }
}

void 
Tracker::delete_tracks()
{
    for(int i = single_tracks.size() - 1; i >= 0; --i)
    {
        const auto& p = single_tracks.at(i)->getPointPrediction();
        const auto& ntime_missed = single_tracks.at(i)->ntime_missed;

        if(p.x < 0 || p.x >= int(width) || p.y < 0 || p.y >= int(height) || ntime_missed >= param.getMaxmissed())
        {
            bool isInside = false;
            for(const auto& stream : streams)
            {
                const auto& fov = stream.getFOV();
                if(int(fov.at<uchar>(p)) != 0)
                {
                    isInside = true;
                    break;
                }
            }
            
            if(isInside)
            {
                old_tracks.push_back(single_tracks.at(i));
            }
            
            single_tracks.erase(single_tracks.begin() + i);
        }
    }
}


cv::Mat 
Tracker::associate_tracks(const Detections& _detections)
{
    if(_detections.size() == 0) return cv::Mat();
    cv::Mat assigmentsBin(cv::Size(_detections.size(), single_tracks.size()), CV_8UC1, cv::Scalar(0));
    assignments_t assignments;
    distMatrix_t cost(_detections.size() * single_tracks.size());
    cv::Mat costs(cv::Size(_detections.size(), single_tracks.size()), CV_32FC1);
    cv::Mat mu;
    cv::Mat sigma;
    cv::Point2f t;


    //COMPUTE COSTS
    const uint& tSize = single_tracks.size();
    const uint& dSize = _detections.size();

    for(uint i = 0; i < tSize; ++i)
    {
        mu = single_tracks.at(i)->getPrediction();
        sigma = single_tracks.at(i)->S();
        t = cv::Point2f(mu.at<float>(0), mu.at<float>(1));
        
        for(uint j = 0; j < dSize; ++j)
        {
            cv::Mat detection(cv::Size(1, 2), CV_32FC1);
            detection.at<float>(0) = _detections.at(j).x();
            detection.at<float>(1) = _detections.at(j).y();
            //compute mahalanobis distance costs
            costs.at<float>(i, j) = cv::Mahalanobis(detection, cv::Mat(t), sigma.inv());
            cost.at(i + j * single_tracks.size() ) = costs.at<float>(i, j);
        }
    }
    
    //compute the munkres algorithm
    AssignmentProblemSolver APS;
    APS.Solve(cost, single_tracks.size(), _detections.size(), assignments, AssignmentProblemSolver::optimal);
    
    for(auto i = 0; i < int(assignments.size()); ++i)
    {
        //check if there is an assigment and if the cost associated to the assignment is less then 
        //a threshold
        if(costs.at<float>(i, assignments[i]) < association_thresh && assignments[i] != -1)
            assigmentsBin.at<uchar>(i, assignments[i]) = 1;
    }
    


    return assigmentsBin;
}



void 
Tracker::update_tracks(const std::vector<cv::Mat>& assigments, const std::vector<Detections>& _detections)
{
    std::map<int, std::vector<std::pair< int, cv::Rect> > > idx_to_update;
    std::map<int, std::vector<cv::Mat> >  idx_hists;
    auto m = 0;

    for(const auto& assignment : assigments)
    {
        const uint& aRows = assignment.rows;
        const uint& aCols = assignment.cols;
        const auto& detection = _detections.at(m);
        if(detection.size() > 0)
        {
            for(uint i = 0; i  < aRows; ++i)
            {
                for(uint j = 0; j < aCols; ++j)
                {
                    if(assignment.at<uchar>(i, j) == uchar(1))
                    {
                        if(idx_to_update.find(i) != idx_to_update.end())
                        {
                            idx_to_update[i].push_back(std::make_pair (m, cv::Rect(detection.at(j).x(), detection.at(j).y(),
                                detection.at(j).w(), detection.at(j).h())));
                            idx_hists[i].push_back(detection.at(j).hist());
                        }
                        else
                        {
                            idx_to_update.insert(std::make_pair(i, std::vector<std::pair< int,cv::Rect> >()));
                            idx_to_update[i].push_back(std::make_pair(m, cv::Rect(detection.at(j).x(), detection.at(j).y(),
                                detection.at(j).w(), detection.at(j).h())));
                            idx_hists.insert(std::make_pair(i, std::vector<cv::Mat>()));
                            idx_hists[i].push_back(detection.at(j).hist());
                        }
                    }
                }
            }
        }
        ++m;
    }
    
    for(const auto& idx : idx_to_update)
    {
        const auto& points = idx.second;
        int id_hist = -1;
        if(points.size() == 1)
        {
            single_tracks.at(idx.first)->push_point(points.at(0).second.tl());
            single_tracks.at(idx.first)->sizes.at(points.at(0).first) = cv::Size(points.at(0).second.width, points.at(0).second.height);
            single_tracks.at(idx.first)->set_hist(idx_hists[idx.first].at(0));
        }
        else
        {
            cv::Point2f p(0, 0);
            for(const auto& point : points)
            {
                p.x += (cameraProbabilities.at(point.first) * point.second.x);
                p.y += (cameraProbabilities.at(point.first) * point.second.y);
                single_tracks.at(idx.first)->sizes.at(point.first) = cv::Size(point.second.width, point.second.height);
            }
            single_tracks.at(idx.first)->push_point(p);
        }
               
        single_tracks.at(idx.first)->update();
        single_tracks.at(idx.first)->ntime_missed = 0;
        
        if(id_hist != -1)
            single_tracks.at(idx.first)->set_hist(idx_hists[idx.first].at(id_hist));
        
        
        if(single_tracks.at(idx.first)->nTimePropagation() >= param.getMinpropagate() && !single_tracks.at(idx.first)->isgood)
        {
            single_tracks.at(idx.first)->setLabel(trackIds++);
            
            single_tracks.at(idx.first)->setColor(cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), 
                                rng.uniform(0, 255)));
            single_tracks.at(idx.first)->isgood = true;
        }
    }

    for(const auto& assignment : assigments)
    {
        cv::Mat ass_sum;
        cv::reduce(assignment, ass_sum, 1, CV_REDUCE_SUM, CV_32S);
    
        for(uint i = 0; i < ass_sum.total(); ++i)
        {
            if(ass_sum.at<int>(i) == 0)
            {
                single_tracks.at(i)->ntime_missed++;
            }
        }
    }
    
}

void 
Tracker::check_old_tracks(std::vector<Detections>& _detections)
{
    std::vector<cv::Mat> assignents;
    for(auto &det : _detections)
    {
        const int& tSize = int(old_tracks.size());
        const int& dSize = int(det.size());
        
        if(dSize > 0)
        {
        
            //COMPUTE COSTS
            assignments_t assignments;
            distMatrix_t cost;
            
            cv::Mat costs(cv::Size(det.size(), old_tracks.size()), CV_32FC1);
            cv::Mat hist_costs(cv::Size(det.size(), old_tracks.size()), CV_32FC1);
            cv::Mat assigmentsBin(cv::Size(det.size(), old_tracks.size()), CV_8UC1, cv::Scalar(0));
            
            cv::Mat mu;
            cv::Mat sigma;
            cv::Point2f t;
    
            for(auto i = 0; i < tSize; ++i)
            {
                mu = old_tracks.at(i)->getPrediction();
                sigma = old_tracks.at(i)->S();
                t = cv::Point2f(mu.at<float>(0), mu.at<float>(1));
                
                for(auto j = 0; j < dSize; ++j)
                {
                    cv::Mat detection(cv::Size(1, 2), CV_32FC1);
                    detection.at<float>(0) = det.at(j).x();
                    detection.at<float>(1) = det.at(j).y();
                    // compute the mahalanobis distance
                    costs.at<float>(i, j) = cv::Mahalanobis(detection, cv::Mat(t), sigma.inv());
                    // compute the correlation distance
                    hist_costs.at<float>(i, j) = 1 - cv::compareHist(det.at(j).hist(), old_tracks.at(i)->histogram(), cv::HISTCMP_CORREL);
                }
            }
            //normalize the costs between 0 and 1
            cv::normalize(costs, costs, 0., 1., cv::NORM_MINMAX, -1, cv::Mat());
            //linear combination between Mahalanobis distance costs and histogram costs
            costs = .6 * costs + .4 *  hist_costs;
            
            //copy the  matrix of costs into a vector
            if (costs.isContinuous()) 
            {
                cost.assign((float*)costs.data, (float*)costs.data + costs.total());
            } 
            else 
            {
                for (int i = 0; i < costs.rows; ++i) 
                {
                    cost.insert(cost.end(), costs.ptr<float>(i), costs.ptr<float>(i)+costs.cols);
                }
            }
            
            //computing munkres algorithm
            AssignmentProblemSolver APS;
            APS.Solve(cost, old_tracks.size(), det.size(), assignments, AssignmentProblemSolver::optimal);
            
            
            for(auto i = 0; i < int(assignments.size()); ++i)
            {
                //check if there is an assigment and if the cost associated to the assignment is less then 
                //a threshold 
                if(costs.at<float>(i, assignments[i]) < freezed_thresh && assignments[i] != -1)
                    assigmentsBin.at<uchar>(i, assignments[i]) = 1;
            }
            assignents.push_back(assigmentsBin);
        }
    }
    
    std::vector<cv::Mat> notAssigned; 
    std::set<int> notUp;
    std::set<int, std::greater<int> > to_restore;
    //check the ids of the tracks to restore
    for(const auto& assignment : assignents)
    {
        for(auto i = 0; i < assignment.rows; ++i)
        {
            for(auto j = 0; j < assignment.cols; ++j)
            {
                if(assignment.at<uchar>(i, j) == uchar(1))
                { 
                    to_restore.insert(i);
                }
            }
        }
    }

    //restore the freezed tracks 
    for(const auto& id : to_restore)
    {
        single_tracks.push_back(old_tracks.at(id));
        old_tracks.erase(old_tracks.begin() + id);
    }
    
    
    for(int m = old_tracks.size() - 1; m >= 0; --m)
    {
        //delete freezed tracks
        if(old_tracks.at(m)->freezed >= param.getMaxmissed())
        {
            old_tracks.erase(old_tracks.begin() + m);
        }
        //or increment the number of "freezing" 
        else
        {
            old_tracks.at(m)->freezed++;
        }
    }
}

const Entities 
Tracker::getTracks()
{
    tracks.clear();
    tracks.insert(tracks.end(), single_tracks.begin(), single_tracks.end());
    return tracks;
}
