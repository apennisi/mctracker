/*
 * Written by Andrea Pennisi
 */

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <iostream>
#include <opencv2/opencv.hpp>
#include "homography.h"
#include "yamlmanager.h"

using namespace mctracker::config;

namespace mctracker
{
    namespace utils
    {
        class Camera
        {
            public:
                /**
                 * @brief Constructor class Camera
                 * @param stream a string containing the source path of the streaming
                 * @param view a string containing the path to the binary image representing the field of view of the cameras
                 * @param homography_file a string containing the path to the homography file
                 * @param prox a bool which if is true means that the camera is close to scene to monitor
                 */
                Camera(const std::string& stream, const std::string& view, const std::string& homography_file, bool prox);
                /**
                 * @brief get a frame from the stream
                 * @param frame cv::Mat where the frame is stored
                 * @return true if the frame is read successfully
                 */
                bool getFrame(cv::Mat& frame);
                
                /**
                 * @brief convert a point to world coordinates
                 * @param p cv::Point2f to convert
                 * @return the converted point
                 */
                cv::Point2f camera2world(const cv::Point2f& p) const;
                
                /**
                 * @brief convert a point to image coordinates
                 * @param p cv::Point2f to convert
                 * @return the converted point
                 */
                cv::Point2f world2camera(const cv::Point2f& p) const;
            public:
                /**
                 * @brief get if the camera is close to the scene
                 * @return a bool which if is true means that the camera is close to scene to monitor
                 */
                inline const bool 
                getProximity() const
                {
                    return proximity;
                }
                
                /**
                 * @brief get the binary image representing the fov of the camera
                 * @return a cv::Mat representing the fov of the camera
                 */
                inline const cv::Mat
                getFOV() const
                {
                    return image_view;
                }
            private:
                cv::VideoCapture cap;
                cv::Mat image_view;
                cv::Mat H, Hinv;
                bool proximity;
        }; 
    }
}

#endif
