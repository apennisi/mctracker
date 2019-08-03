/*
 * Written by Andrea Pennisi
 */

#ifndef KALMAN_H
#define KALMAN_H

#include <opencv2/opencv.hpp>

namespace mctracker
{
    namespace tracker
    {
        class KalmanFilter
        {
            public:
                /**
                * @brief Constructor class KalmanFilter
                */
                KalmanFilter() {;}
                /**
                * @brief Constructor class KalmanFilter
                * @param _x x-coordinate of the current detection
                * @param _y y-coordinate of the current detection
                * @param _dt inital update frequency
                */
                KalmanFilter(const float &_x, const float &_y,  const float &dt);
                
                /**
                 * @brief get the kalman filter
                 * @return the kalman filter
                 */
                inline cv::KalmanFilter kf()
                {
                    return KF;
                }

                /**
                 * @brief preditction step of the kalman filter
                 * @return a cv::Mat containing the prediction
                 */
                cv::Mat predict();
                /**
                 * @brief correction step of the kalman filter
                 * @param _x x-coordinate of the current detection
                 * @param _y y-coordinate of the current detection
                 * @return  cv::Mat containing the corrected prediction
                 */
                cv::Mat correct(const int &_x, const int &_y);
                /**
                 * @brief the last prediction
                 * @return a cv;:Mat containing the last prediction
                 */
                inline cv::Mat 
                getPrediction()
                {
                    return prediction;
                }
                
                /**
                 * @brief get the the control-input model of the kalman filter associated to the entity
                 * @return the control-input model
                 */
                inline const cv::Mat 
                B() const
                {
                    return KF.controlMatrix;
                }
                
                /**
                 * @brief get posteriori error covariance matrix of the kalman filter associated to the entity
                 * @return the posteriori error covariance matrix
                 */
                inline const cv::Mat 
                P() const
                {
                    return KF.errorCovPre;
                }
                
                /**
                 * @brief get the error measurement covariance matrix of the kalman filter associated to the entity
                 * @return the error measurement covariance matrix
                 */
                inline const cv::Mat 
                S() const
                {
                    return KF.errorCovPre(cv::Rect(0, 0, 2, 2)) + KF.measurementNoiseCov;
                }
                
                /**
                 * @brief set the update frequencies of the entity
                 * @param dt a float containing the value of dt
                 */
                const void 
                setDt(const float& dt)
                {
                    KF.transitionMatrix.at<float>(2) = dt;
                    KF.transitionMatrix.at<float>(7) = dt;
                }

            private:
                //the kalman filter
                cv::KalmanFilter KF;
                cv::Mat_<float> measurement;
                cv::Mat processNoise;
                cv::Mat_<float> state;
                cv::Mat prediction;
                uint w, h;
        };
    }
}

#endif //KALMAN_H
