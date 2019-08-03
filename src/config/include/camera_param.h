/*
 * Written by Andrea Pennisi
 */

#ifndef _CAMERA_PARAM_H_
#define _CAMERA_PARAM_H_

#include <iostream>

namespace mctracker
{
    namespace config
    {
        class CameraParam
        {
            public:
                /**
                 * @brief Constructor class KalmanParam
                 */
                CameraParam() { ; }
                
                /**
                 * @brief set the path to the stream device
                 * @param s string containing the path
                 */
                void 
                setStream(const std::string& s)
                {
                    stream = s;
                }
                
                /**
                 * @brief get the path to the stream device
                 * @return string containing the path
                 */
                inline const std::string 
                getStream() const
                {
                    return stream;
                }
                
                /**
                 * @brief set the path to the homography file
                 * @param h string containing the path
                 */
                void
                setHomography(const std::string& h)
                {
                    homography = h;
                }
                
                /**
                 * @brief get the path to the homography file
                 * @return a string containing the path
                 */
                inline const std::string
                getHomography() const
                {
                    return homography;
                }
                
                /**
                 * @brief set the path to the fov image
                 * @param fov string containing the path
                 */
                void
                setFOV(const std::string& fov)
                {
                    fieldOfView = fov;
                }
                
                /**
                 * @brief set the path to the fov image
                 * @return string containing the path
                 */
                inline const std::string
                getFOV() const
                {
                    return fieldOfView;
                }
                
                /**
                 * @brief set if a camera is close to the scene to monitor
                 * @param prox a boolean value where true means that the camera is close to the scene, false otherwise
                 */
                void
                setProximity(const bool prox)
                {
                    proximity = prox;
                }
                
                /**
                 * @brief get the proximity valuer
                 * @return a boolean value where true means that the camera is close to the scene, false otherwise
                 */
                inline const float
                getProximity() const
                {
                    return proximity;
                }
                
                /**
                 * @brief copy operator
                 * @param _param object to copy
                 * @return the pointer to the current instance
                 */ 
                CameraParam& operator=(const CameraParam& _param)
                {
                    stream = _param.getStream();
                    homography = _param.getHomography();
                    fieldOfView = _param.getFOV();
                    proximity = _param.getProximity();
                    return *this;
                }
                
                /**
                 * @brief print all the parameters
                 */
                void 
                print()
                {
                    std::cout << "[STREAM]: " << stream << std::endl;
                    std::cout << "[HOMOGRAPHY]: " << homography << std::endl;
                    std::cout << "[FOV]: " << fieldOfView << std::endl;
                    std::cout << "[PROXIMITY]: " << proximity << std::endl;
                }
                
            private:
                std::string stream;
                std::string homography;
                std::string fieldOfView;
                bool proximity;
        };
    }
}

#endif
