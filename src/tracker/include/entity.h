/*
 * Written by Andrea Pennisi
 */


#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <iostream>
#include <memory>
#include <opencv2/opencv.hpp>

#include "kalman.h"
#include "drawing.h"
#include "utils.h"
#include "camera.h"

using namespace mctracker::utils;

namespace mctracker
{
    namespace tracker
    {
        class Entity
        {   
            friend class Tracker;

            public:
                /**
                * @brief Constructor class Entity
                */
                Entity() { ; }
                /**
                 * @brief get the 2d point representing the last predicted position of the entity
                 * @return the 2d point in a cv::Point2f format
                 */
                virtual const cv::Point2f getPoint() = 0;
                /**
                 * @brief get the id of the entity
                 * @return the integer representing the id assigned to the entity
                 */
                virtual const int label() const = 0;
            public:
                /**
                 * @brief get the sizes of the the entity from all the views
                 * @return a vector of cv::Size containing all the sizes from all the view
                 */
                const std::vector<cv::Size> getSizes() const
                {
                    return sizes;
                }
            
                /**
                 * @brief draw the tracked entities in all the camera views
                 * @param images a vector containing the current frames grabbed from all the cameras
                 * @param cameras a vector containing all the camera infos
                 */
                inline const void 
                drawTrack(std::vector<cv::Mat>& images, const std::vector<Camera>& cameras)
                {
                    if(isgood)
                    {
                        auto i = 0;
                        for(auto& img : images)
                        {
                            auto p = getPoint();
                            auto imPoint = cameras.at(i).world2camera(p);
                            const std::vector<cv::Size>& sz = sizes;
                            const cv::Size& curr_size = sz.at(i);
                            if(curr_size.width != 0)
                            {
                                imPoint.y -= curr_size.height;
                                imPoint.x -= (curr_size.width>>1);

                                if(imPoint.x >= 0 && imPoint.y >= 0 && (imPoint.x + curr_size.width) < (img.cols - 1) && (imPoint.y + curr_size.height) < (img.rows - 1))
                                {
                                    tools::Drawing::rectangle(cv::Rect(imPoint.x, imPoint.y, curr_size.width, curr_size.height), color, img);
                                    std::stringstream ss;
                                    ss << "#" << label2string();
    
                                    cv::putText(img, ss.str().c_str(), imPoint, cv::FONT_HERSHEY_SIMPLEX,
                                                            0.55, cv::Scalar(0, 255, 0), 2, CV_AA);
                                }
                            }
                            ++i;
                        }
                    }
                }
                
                /**
                 * @brief draw all the entities on the planview
                 * @param img cv::Mat containing the plan view
                 * @param history boolean: if true all the history realted to the entity is drawed
                 */
                inline const void 
                drawTrackPlanView(cv::Mat& img, bool history = true)
                {
                    if(isgood)
                    {
                        const auto& p = getPoint();
                        cv::circle(img, p, 3, color, -1);
                        cv::putText(img, label2string().c_str(), p, cv::FONT_HERSHEY_SIMPLEX,
                                            0.55, cv::Scalar(0, 255, 0), 2, CV_AA);
                        
                        if(history) 
                            tools::Drawing::history(m_history, color, img);
                    }
                }
                
                /**
                 * @brief get the history of the entity
                 * @return a vector of cv::Points containing the history of the entity
                 */
                inline const Points 
                history() const
                {
                    return m_history;
                }
                
            protected:
                typedef std::shared_ptr<KalmanFilter> Kalman_ptr;
            protected:
                virtual const std::string label2string() = 0;
            protected:
                /**
                 * @brief set the color of the entity
                 * @param _color a cv::Scalar containing the color
                 */
                void 
                setColor(const cv::Scalar& _color)
                {
                    color = _color;
                }
                
                /**
                 * @brief set the update frequencies of the entity
                 * @param dt a float containing the value of dt
                 */
                inline const void 
                setDt(const float& dt)
                {
                    kf->setDt(dt);
                }
                
                /**
                 * @brief get the the control-input model of the kalman filter associated to the entity
                 * @return the control-input model
                 */
                inline const 
                cv::Mat B() const
                {
                    return kf->B();
                }
                
                /**
                 * @brief get posteriori error covariance matrix of the kalman filter associated to the entity
                 * @return the posteriori error covariance matrix
                 */
                inline const cv::Mat 
                P() const
                {
                    return kf->P();
                }
                
                /**
                 * @brief get the error measurement covariance matrix of the kalman filter associated to the entity
                 * @return the error measurement covariance matrix
                 */
                inline const 
                cv::Mat S() const
                {
                    return kf->S();
                }
                
                /**
                 * @brief check the history of the entity in order to keep only the last 10 observations
                 */
                const 
                void checkHistory()
                {
                    if(m_history.size() > history_size)
                    {
                        m_history.erase(m_history.begin());
                    }
                }
                
            protected:
                Points m_history;
                cv::Scalar color;
                Kalman_ptr kf;
                uint w, h;
                bool isgood;
                std::vector<cv::Size> sizes;
            protected:
                constexpr static uint history_size = 10;
        };
    }
}

#endif
