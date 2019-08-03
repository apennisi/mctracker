#include "track.h"

using namespace mctracker::tracker;

Track
::Track(const float& _x, const float& _y,  const KalmanParam& _param, const cv::Mat& h, const int cameraNum)
  : Entity(), hist(h)
{
    kf = std::shared_ptr<KalmanFilter>(new KalmanFilter(_x, _y,  _param.getDt()));
    ntimes_propagated = 0;
    freezed = 0;
    ntime_missed = 0;
    isgood = false;
    m_label = -1;
    time = (double)cv::getTickCount();
    sizes.resize(cameraNum);
}

const cv::Mat
Track::update()
{
    if(points.size() > 0)
    {
        cv::Point2f result(0,0);
        for(const auto& p : points)
        {
            result += p;
        }
        
        const auto& correction = correct(result.x, result.y);
        
        points.clear();
    
        return correction;
    }
    
    ntime_missed++;
    return  cv::Mat();
}

const cv::Mat 
Track::predict()
{
    const auto& prediction = kf->predict();
    m_history.push_back(cv::Point2f(prediction.at<float>(0), prediction.at<float>(1)));
    checkHistory();
    return prediction;
}

const cv::Mat 
Track::correct(const float& _x, const float& _y)
{
    ntimes_propagated++;
    time_in_sec = ((double) cv::getTickCount()  - time) / cv::getTickFrequency();
    return kf->correct(_x, _y);
}

const cv::Point2f 
Track::getPoint()
{
    const auto& prediction = kf->getPrediction();
    return cv::Point2f(prediction.at<float>(0), prediction.at<float>(1));
}

const std::string 
Track::label2string()
{
    std::stringstream ss;
    ss << m_label;
    return ss.str();
}
