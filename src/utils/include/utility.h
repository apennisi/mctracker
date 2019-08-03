/*
 * Written by Andrea Pennisi
 */

#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <opencv2/opencv.hpp>
#include "object_detector.h"
#include "detection.h"
#include "camera.h"

using namespace mctracker::tracker;
using namespace mctracker::objectdetection;

namespace mctracker
{
    namespace utils
    {
        class Utility
        {
            public:
                /**
                 * @brief Constructor class Utility
                 */
                Utility() { ; }
                /**
                 * @brief convert a set of detections of type bbox_t coming from the detector into type Detection
                 * @param detections a vector of vector of type bbox_t coming from the detector
                 * @param frames all the frames grabbed from all the streams in the system
                 * @param streams camera stack
                 * @return a vector of vector of type Detection compatible with the tracker format
                 */
                static std::vector<std::vector<Detection> > dets2Obs(const std::vector< std::vector<bbox_t> >& detections,
                        const std::vector<cv::Mat>& frames, const std::vector<cv::Mat>& masks, const std::vector<Camera>& streams);
                
                /**
                 * @brief make a mosaic between a set of images
                 * @param images a set of images
                 * @return an image containing a mosaic of all the input images
                 */
                static cv::Mat makeMosaic(const std::vector<cv::Mat>& images);
                
                /**
                 * @brief compute a hsv histogram
                 * @param img input image
                 * @param mask mask (if any) used for computing the histogram
                 * @param d bounding box representing the detection for which the histogram is computed
                 */
                static cv::Mat computeHist(const cv::Mat& img, const cv::Mat& mask, const bbox_t& d);
        };
    }
}

#endif
