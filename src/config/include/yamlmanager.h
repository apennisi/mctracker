/*
 * Written by Andrea Pennisi
 */

#ifndef _YAML_MANAGER_H_
#define _YAML_MANAGER_H_

#include <iostream>
#include <opencv2/opencv.hpp>
#include <yaml-cpp/yaml.h>
#include <stdexcept>
#include <fstream>
#include <sys/stat.h>

namespace mctracker
{
    namespace config
    {
        class YamlManager
        {
            public:
                /**
                * @brief Constructor class YamlManager
                */
                YamlManager();
                /**
                * @brief Write a new yaml file
                * @param filepath the path where you have to save your file, including the filename
                * @return true if the file is correctly written, false otherwise
                */
                bool write(const std::string& filepath);
                /**
                 * @brief Read a yaml file
                 * @param filepath the path where your file is stored
                 * @return true if the fiel is correctly read, false otherwise
                 */
                bool read(const std::string& filepath);
                /**
                 * @brief Read an integer element from the yaml file given the key
                 * @param name key to read
                 * @param elem variable where the value is stored
                 * @return true if the element exists and is correctly read, false otherwise
                 */
                bool getElem(const std::string& name, int& elem);
                /**
                 * @brief Read a float element from the yaml file given the key
                 * @param name key to read
                 * @param elem variable where the value is stored
                 * @return true if the element exists and is correctly read, false otherwise
                 */
                bool getElem(const std::string& name, float& elem);
                /**
                 * @brief Read a double element from the yaml file given the key
                 * @param name key to read
                 * @param elem variable where the value is stored
                 * @return true if the element exists and is correctly read, false otherwise
                 */
                bool getElem(const std::string& name, double& elem);
                 /**
                 * @brief Read a boolean element from the yaml file given the key
                 * @param name key to read
                 * @param elem variable where the value is stored
                 * @return true if the element exists and is correctly read, false otherwise
                 */
                bool getElem(const std::string& name, bool& elem);
                 /**
                 * @brief Read a cv::Point2i element from the yaml file given the key
                 * @param name key to read
                 * @param elem variable where the value is stored
                 * @return true if the element exists and is correctly read, false otherwise
                 */
                bool getElem(const std::string& name, cv::Point2i& elem);
                /**
                 * @brief Read a cv::Point2f element from the yaml file given the key
                 * @param name key to read
                 * @param elem variable where the value is stored
                 * @return true if the element exists and is correctly read, false otherwise
                 */
                bool getElem(const std::string& name, cv::Point2f& elem);
                /**
                 * @brief Read a cv::Point2d element from the yaml file given the key
                 * @param name key to read
                 * @param elem variable where the value is stored
                 * @return true if the element exists and is correctly read, false otherwise
                 */
                bool getElem(const std::string& name, cv::Point2d& elem);
                /**
                 * @brief Read a string element from the yaml file given the key
                 * @param name key to read
                 * @param elem variable where the value is stored
                 * @return true if the element exists and is correctly read, false otherwise
                 */
                bool getElem(const std::string& name, std::string& elem);
                /**
                 * @brief Read a vector of integers from the yaml file given the key
                 * @param name key to read
                 * @param elem variable where the value is stored
                 * @return true if the element exists and is correctly read, false otherwise
                 */
                bool getElem(const std::string& name, std::vector<int>& elem);
                /**
                 * @brief Read a vector of boolean from the yaml file given the key
                 * @param name key to read
                 * @param elem variable where the value is stored
                 * @return true if the element exists and is correctly read, false otherwise
                 */
                bool getElem(const std::string& name, std::vector<bool>& elem);
                /**
                 * @brief Read a vector of floats from the yaml file given the key
                 * @param name key to read
                 * @param elem variable where the value is stored
                 * @return true if the element exists and is correctly read, false otherwise
                 */
                bool getElem(const std::string& name, std::vector<float>& elem);
                /**
                 * @brief Read a vector of doubles from the yaml file given the key
                 * @param name key to read
                 * @param elem variable where the value is stored
                 * @return true if the element exists and is correctly read, false otherwise
                 */
                bool getElem(const std::string& name, std::vector<double>& elem);
                /**
                 * @brief Read a vector of cv::Point from the yaml file given the key
                 * @param name key to read
                 * @param elem variable where the value is stored
                 * @return true if the element exists and is correctly read, false otherwise
                 */
                bool getElem(const std::string& name, std::vector<cv::Point>& elem);
                /**
                 * @brief Read a vector of cv::Point2f from the yaml file given the key
                 * @param name key to read
                 * @param elem variable where the value is stored
                 * @return true if the element exists and is correctly read, false otherwise
                 */
                bool getElem(const std::string& name, std::vector<cv::Point2f>& elem);
                /**
                 * @brief Read a cv::Mat from the yaml file given the key
                 * @param name key to read
                 * @param elem variable where the value is stored
                 * @param type the type of cv::Mat: CV_FC321 or CV_FC64U1
                 * @return true if the element exists and is correctly read, false otherwise
                 */
                bool getElem(const std::string& name, cv::Mat& elem, int type);
                /**
                 * @brief Write a float into the yaml file given the key
                 * @param name key to write
                 * @param elem variable to write
                 * @return true if the is correctly written, false otherwise
                 */
                void writeElem(const std::string& key, const float& elem);
                /**
                 * @brief Write a cv::Point into the yaml file given the key
                 * @param name key to write
                 * @param elem variable to write
                 * @return true if the is correctly written, false otherwise
                 */
                void writeElem(const std::string& key, const cv::Point& elem);
                /**
                 * @brief Write a cv::Mat into the yaml file given the key
                 * @param name key to write
                 * @param elem variable to write
                 * @return true if the is correctly written, false otherwise
                 */
                void writeElem(const std::string& key, const cv::Mat& elem);
                /**
                 * @brief Check if a key exists
                 * @param name name of the key to check
                 * @return true if exists, false otherwise
                 */
                bool keyExists(const std::string& key);
                
            private:
                /**
                 * @brief Check if a file exists on your local HD
                 * @param name path of the file name to check
                 * @return true if exists, false otherwise
                 */
                inline bool 
                exists(const std::string& name) const
                {
                    if(name.find("://")!=std::string::npos) return true;
                    struct stat buffer;
                    return (stat (name.c_str(), &buffer) == 0);
                }

            private:
                YAML::Emitter out;
                YAML::Node in;
        };
    }
}


#endif
