/*
 * Written by Andrea Pennisi
 */

#ifndef _CAMERA_STACK_H_
#define _CAMERA_STACK_H_

#include <opencv2/opencv.hpp>
#include <iostream>
#include "camera.h"
#include "camera_param.h"
#include "yamlmanager.h"

using namespace mctracker::config;

namespace mctracker
{
    namespace utils
    {
        class CameraStack
        {
            public:
                /**
                 * @brief Constructor class Camera
                 */
                CameraStack() { ; }
                /**
                 * @brief Constructor class Camera
                 * @param params camera parameters
                 */
                CameraStack(const std::vector<CameraParam>& params);
                /**
                 * @brief Get the frames from all the camera of the system
                 * @param frames a vector of cv::Mat where the frames are stored
                 * @return a bool which if true means that the frames have been correctly readed from all the cameras, false otherwise
                 */
                bool getFrame(std::vector<cv::Mat>& frames);
            public:
                /**
                 * @brief get the vector containing the cameras
                 * @retun the camera stack
                 */
                inline const std::vector<Camera>
                getCameraStack() 
                {
                    return streams;
                }
            private:
                /**
                 * @brief open all the streams of the system
                 * @param params a vector containing all the parameters of each stream of the system
                 */
                void open_streams(const std::vector<CameraParam>& params);
            private:
                std::vector<Camera> streams;
        };
    }
}

#endif
