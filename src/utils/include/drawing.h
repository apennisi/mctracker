#ifndef _DRAWING_H_
#define _DRAWING_H_

#include <iostream>
#include <memory>
#include <opencv2/opencv.hpp>

#include "utils.h"

namespace mctracker
{
    namespace utils
    {
        namespace tools
        {
            class Drawing
            {
                public:
                    Drawing() { ; }
                    static void history(const Points& _history, const cv::Scalar& color, cv::Mat& img);
                    static void arrow(const cv::Point& start_point, const cv::Point& end_point, const cv::Scalar& color, cv::Mat& img);
                    static void rectangle(const cv::Rect& _rect, const cv::Scalar& color, cv::Mat& img);
            };
        }
    }
}

#endif

