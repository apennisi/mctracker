/*
 * Written by Andrea Pennisi
 */


#ifndef _TRACK_H_
#define _TRACK_H_

#include <iostream>
#include <memory>

#include "utils.h"
#include "entity.h"
#include "kalman.h"
#include "kalman_param.h"

using namespace mctracker::config;

namespace mctracker
{
    namespace tracker
    {
        class Track : public Entity
        {
            friend class Tracker;
            
            public:
                /**
                 * @brief Constructor class Track
                 */
                Track() { ; }
                /**
                 * @brief Constructor class Track
                 * @param _x x-coordinate of the current detection
                 * @param _y y-coordinate of the current detection
                 * @param _param kalaman filter params
                 * @param _h hsv histogram of the current detection
                 * @param cameraNum number of cameras of the system
                 */
                Track(const float& _x, const float& _y, const KalmanParam& _param, const cv::Mat& _h, const int cameraNum);
                /**
                 * @brief get the last prediction
                 * @return the last predition of the kalman filter
                 */
                const cv::Point2f getPoint();
            public:
                /**
                 * @brief get the id associated to the track
                 * @return the id
                 */
                inline const int 
                label() const
                {
                    return m_label;
                }
            protected:
                /**
                 * @brief preditction step of the kalman filter
                 * @return a cv::Mat containing the prediction
                 */
                const cv::Mat predict();
                 /**
                 * @brief correction step of the kalman filter
                 * @param _x x-coordinate of the current detection
                 * @param _y y-coordinate of the current detection
                 * @return  cv::Mat containing the corrected prediction
                 */
                const cv::Mat correct(const float& _x, const float& _y);
                /**
                 * @brief update the kalman filter considering all the observation of the same detection
                 * @return a cv::Mat containing the corrected prediction
                 */
                const cv::Mat update();
            protected:
                /**
                 * @brief compare the current instance with an external object of the same type
                 * @return true if the objects are the same, false otherwise
                 */
                bool 
                operator==(const Track& _compare)
                {
                    return m_label == _compare.label();
                }
                
                /**
                 * @brief get number of times that the track has been ntimes_propagated
                 * @return the number of times that the track has been ntimes_propagated
                 */
                inline const uint 
                nTimePropagation() const
                {
                    return ntimes_propagated;
                }
                
                /**
                 * @brief get the prediction of the kalman filter
                 * @return a cv::Mat containing the prediction
                 */
                inline const cv::Mat 
                getPrediction() const
                {
                    return kf->getPrediction();
                }
                
                /**
                 * @brief the point representing the last prediction
                 * @eturn a cv::Point2f containing the last prediction
                 */
                inline const cv::Point 
                getPointPrediction() 
                {
                    const cv::Mat& prediction = kf->getPrediction();
                    return cv::Point2f(prediction.at<float>(0), prediction.at<float>(1));
                }
                
                /**
                 * @brief get the alive time of the track
                 * @return the time
                 */
                inline const double 
                getTime() const
                {
                    return time_in_sec;
                }
                
                /**
                 * @brief return the histogram associated to the track
                 * @return the hsv histogram
                 */
                inline const cv::Mat 
                histogram() const
                {
                    return hist;
                }
                
                /**
                 * @brief set the label of the track
                 * @param _label unsigned int representing the label
                 */
                void 
                setLabel(const uint& _label)
                {
                    m_label = _label;
                }
                
                /**
                 * @brief set the points representing the detecions associated to the track
                 * @param p a cv::Point2f representing a single detection
                 */
                void 
                push_point(const cv::Point2f& p)
                {
                    points.push_back(p);
                }
                
                /**
                 * @brief set the hsv histogram of the track
                 * @param h a cv::Mat containing the hsv histogram
                 */
                void 
                set_hist(const cv::Mat& h)
                {
                    hist = h;
                }
            private:
                /**
                 * @brief convert the id of the track to a string
                 * @return a string containin the id of the track
                 */
                const std::string label2string();
            private:
                int m_label;
                uint ntimes_propagated;
                uint freezed;
                double time;
                std::vector<cv::Point2f> points;
                double time_in_sec;
                uint ntime_missed;
                cv::Mat hist;
        };
    }
}

#endif
