#include "object_detector.h"

using namespace mctracker;
using namespace mctracker::objectdetection;

ObjectDetector
::ObjectDetector(const DetectorParam& params) 
{
    cfg = params.getConfig();
    weights = params.getWeights();
    minimum_thresh = params.getThreshold();
    detector = std::shared_ptr<Detector>(new Detector(cfg, weights));
}


cv::Mat 
ObjectDetector::classify(cv::Mat& frame, bool draw)
{
    final_dets.clear();
    dets = detector->detect(frame);
    refining(dets);
    if(draw)
    {
        draw_boxes(frame, final_dets);
    }
    return frame;
}


void 
ObjectDetector::refining(const std::vector<bbox_t>& dets)
{
    for(const auto& det : dets)
    {
        //0 corresponds to PERSON and minimum_thresh is the minimum threshold I consider for being a person
        if(det.obj_id == 0 && det.prob >= minimum_thresh) 
        {
            final_dets.push_back(det);
        }
    }
}



void
ObjectDetector::draw_boxes(cv::Mat& mat_img, std::vector<bbox_t>& result_vec)
{
    static const cv::Scalar color(0, 0, 255);
    static const std::string obj_name = "person";
    
    for (auto &i : result_vec)
    {
        cv::rectangle(mat_img, cv::Rect(i.x, i.y, i.w, i.h), color, 2);
        
        cv::Size const text_size = cv::getTextSize(obj_name, cv::FONT_HERSHEY_COMPLEX_SMALL, 1.2, 2, 0);
        int const max_width = (text_size.width > (int)i.w + 2) ? text_size.width : (i.w + 2);
        cv::rectangle(mat_img, cv::Point2f(std::max((int)i.x - 1, 0), std::max((int)i.y - 30, 0)),
                        cv::Point2f(std::min((int)i.x + max_width, mat_img.cols-1), std::min((int)i.y, mat_img.rows-1)),
                        color, CV_FILLED, 8, 0);
        cv::putText(mat_img, obj_name, cv::Point2f(i.x, i.y - 10), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.2, cv::Scalar(0, 0, 0), 2);
        
    }
}
