/*
 * Written by Andrea Pennisi
 */

#ifndef _DETECTION_H_
#define _DETECTION_H_

#include <iostream>
#include <opencv2/opencv.hpp>

namespace mctracker
{
    namespace tracker
    {
        class Detection
        {
            public:
                /**
                * @brief Constructor class Detection
                */
                Detection() { ; }
                /**
                * @brief Constructor class Detection
                * @param _x the x-coordinate of the top left point
                * @param _y the y-coordinate of the top left point
                * @param _w the width of the detection
                * @param _h the height of the detection
                * @param hist the hsv histogram of the observation 
                */
                Detection(const float& _x, const float& _y, const float& _w, const float& _h, const cv::Mat& hist)
                : m_x(_x), m_y(_y) , m_w(_w), m_h(_h), m_hist(hist)
                { 
                        point = cv::Point2f(m_x, m_y);
                }
                
                /**
                 * @brief get the x-coordinate of the top left point
                 * @return the x value of the x-coordinate of the top left point
                 */
                inline const float 
                x() const 
                { 
                    return m_x; 
                }
                
                /**
                 * @brief get the y-coordinate of the top left point
                 * @return the y value of the y-coordinate of the top left point
                 */
                inline const float 
                y() const 
                { 
                    return m_y; 
                }
                
                /**
                 * @brief get the width of the detection
                 * @return the width value of the detection
                 */
                inline const float 
                w() const
                {
                    return m_w;
                }
                
                /**
                 * @brief get the height of the detection
                 * @return the height value of the detection
                 */
                inline const float 
                h() const
                {
                    return m_h;
                }
                
                /**
                 * @brief get the hsv histogram of the detection
                 * @return the hsv histogram of the detection
                 */
                inline const cv::Mat 
                hist() const
                {
                    return m_hist;
                }
                
                /**
                 * @brief set the x-coordinate of the top left point
                 * @param x containing the value of the x-coordinate of the top left point
                 */
                void 
                setX(const float& x)
                {
                    m_x = x;
                }
                
                /**
                * @brief set the y-coordinate of the top left point
                * @param y containing the value of the y-coordinate of the top left point
                */
                void 
                setY(const float& y)
                {
                    m_y = y;
                }
                
                /**
                * @brief set the hsv histrogram of the detection
                * @param hist containing the hsv histrogram of the detection
                */
                void 
                setHist(const cv::Mat& hist)
                {
                    m_hist = hist;
                }
                
                /**
                * @brief set the width and the height of the detection
                * @param w containing the width of the detection
                * @param h containing the height of the detection
                */
                void 
                setSize(int w, int h)
                {
                    m_w = w;
                    m_h = h;
                }
                
                /**
                 * @brief operator()
                 * @return a cv::Point2f containing the topleft point of the detection
                 */
                inline const cv::Point2f 
                operator()() const
                {
                    return point;
                }
                
                /**
                 * @brief copy operator
                 * @param d_copy object to copy
                 * @return the pointer to the current instance
                 */ 
                Detection& operator=(const Detection& d_copy)
                {
                    this->m_x = d_copy.x();
                    this->m_y = d_copy.y();
                    this->m_h = d_copy.h();
                    this->m_w = d_copy.w();
                    this->m_hist = d_copy.hist().clone();
                    return *this;
                }
                
            private:
                float m_x, m_y;
                float m_w, m_h;
                cv::Point2f point;
                cv::Mat m_hist;
        };
    }
}

#endif
