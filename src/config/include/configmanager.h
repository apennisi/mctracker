/*
 * Written by Andrea Pennisi
 */

#ifndef _CONFIG_MANAGER_H_
#define _CONFIG_MANAGER_H_

#include <iostream>
#include <opencv2/opencv.hpp>
#include <sys/stat.h>

#include "detector_param.h"
#include "camera_param.h"
#include "kalman_param.h"
#include "yamlmanager.h"

namespace mctracker
{
    namespace config
    {
        class ConfigManager
        {
            public:
                /**
                 * @brief Constructor class ConfigManager
                 */
                ConfigManager() { ; }
            public:
                /**
                 * @brief read all the parameters included in a configuration files
                 * @param config a string containing the path to the configuration file
                 * @return true if the configuration file is correctly readed, false otherwise
                 */
                bool read(const std::string& config);
                /**
                 * @brief print all the values read from the configuration file
                 */
                void print();
            public:
                /**
                 * @brief get the number of cameras of the system specified in the file
                 * @return the the number of cameras of the system specified in the file
                 */
                inline const int
                getCameraNumber() const
                {
                    return cameraNum; 
                }
                
                /**
                 * @brief get the kalman parameters specified in the configuration file
                 * @return the kalman parameters specified in the configuration file
                 */
                inline const KalmanParam
                getKalmanParam() const
                {
                    return kalmanParam;
                }
                
                /**
                 * @brief get the detector parameters specified in the configuration file
                 * @return the detector parameters specified in the configuration file
                 */
                inline const DetectorParam 
                getDetectorParam() const
                {
                    return detectorParam;
                }
                
                /**
                 * @brief get the camera parameters specified in the configuration file
                 * @return the camera parameters specified in the configuration file
                 */
                inline const std::vector<CameraParam>
                getCameraParam() const
                {
                    return cameraParam;
                }
                
                /**
                 * @brief get the plan view of the monitored environment
                 * @return a cv::Mat containing the image of the plan view
                 */
                inline const cv::Mat
                getPlaview() const
                {
                    return planView;
                }
                
                /**
                 * @brief get if the planview has to be showd
                 * @return a bool value: true if the planview has to be showed, false otherwise
                 */
                inline const bool
                showPlanView() const
                {
                    return show;
                }
            private:
                /**
                 * @brief check if a file exists on the hd
                 * @return true if exists, false otherwise
                 */
                bool exist(const std::string& name) const;
            private:
                KalmanParam kalmanParam;
                DetectorParam detectorParam;
                std::vector<CameraParam> cameraParam;
                YamlManager yamlManager;
                cv::Mat planView;
                bool show;
                int cameraNum;
        };
    }
}

#endif
