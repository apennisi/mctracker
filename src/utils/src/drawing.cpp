#include "drawing.h"

using namespace mctracker;
using namespace mctracker::utils::tools;


void 
Drawing::arrow(const cv::Point& start_point, const cv::Point& end_point, 
			  const cv::Scalar& color, cv::Mat& img) 
{
    cv::line(img, start_point, end_point, color, 2);
}

void 
Drawing::history(const Points& _history, const cv::Scalar& color, cv::Mat& img) 
{
    for(const auto& point : _history)
    {
        cv::circle(img, point, 1, color, -1);
    }
}

void 
Drawing::rectangle(const cv::Rect& _rect, const cv::Scalar& color, cv::Mat& img)
{
    cv::rectangle(img, _rect, color, 2);
}


