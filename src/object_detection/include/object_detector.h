/*
 * Written by Andrea Pennisi
 */

#ifndef _OBJECT_DETECTOR_H_
#define _OBJECT_DETECTOR_H_

#include <iostream>
#include <future>
#include <opencv2/opencv.hpp>
#include "detector_param.h"
#include "yolo_v2_class.hpp"


using namespace mctracker::config;
namespace mctracker
{
    namespace objectdetection
    {
        class ObjectDetector
        {
            public:
                /**
                 * @brief Constructor class Homography
                 * @param params object detector params extracted from the configuration file
                 */
                ObjectDetector(const DetectorParam& params);
                /**
                 * @brief classify the current frame
                 * @param frame cv::Mat containing the image to classify
                 * @param draw boolean which if is true allow the function to draw the detections
                 * @return a cv::Mat containing the original frame with the detections drawed on (if draw is true)
                 */
                cv::Mat classify(cv::Mat& frame, bool draw = true);
            public:
                /**
                 * @brief return the detections of the last classified frame
                 * @brief a vector of bbo_t contianing all the detections
                 */
                inline const std::vector<bbox_t> 
                detections() const
                {
                    return final_dets;
                }
            private:
                /**
                 * @brief draw a bounding box around the detections
                 * @param img the cv::Mat where the detections are drawed on
                 * @param result_vec the vector of bbox_t containing all the detections
                 */
                void draw_boxes(cv::Mat& mat_img, std::vector<bbox_t>& result_vec);
                /**
                 * @brief refine all the detections according to the thresholds specified in the config file
                 * @param dets the vector of bbox_t containing all the detections
                 */
                void refining(const std::vector<bbox_t>& dets);
            private:
                std::string cfg, weights;
                float minimum_thresh;
                std::shared_ptr<Detector> detector;
                std::vector<bbox_t> dets;
                std::vector<bbox_t> final_dets;
        };
    }
}

#endif
