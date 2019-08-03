/*
 * Written by Andrea Pennisi
 */

#ifndef _HOMOGRAPHY_H_
#define _HOMOGRAPHY_H_

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <vector>
#include <string>
#include <iostream>

#include "yamlmanager.h"

namespace mctracker
{
    namespace geometry
    {
        class Homography {
            public:
                /**
                 * @brief Constructor class Homography
                 */
                Homography() { ; }
                /**
                 * @brief set the source and destination points
                 * @param src a vecotor of points of the source image
                 * @param dst a vecotor of points of the destination image
                 */
                void setPoints(const std::vector<cv::Point2f>& src, const std::vector<cv::Point2f>& dst);
                /**
                 * @brief calculate the homography
                 * @return a cv::Mat 3x3 containing the homography
                 */
                cv::Mat calcHomography();
                /**
                 * @brief draw the original points and the reprojected into the images
                 * @param srcImage image representing the source to be reprojected
                 * @param dstImage image representing the detestination image
                 * @param H the homography matrix
                 */
                void draw(const cv::Mat& srcImage, const cv::Mat& dstImage, const cv::Mat& H);
                /**
                 * @brief write the matrix into a yaml filepath
                 * @param filepath path to the file in which the homography will be stored
                 */
                void writeToFile(const std::string& filepath = "../config/homography.yaml");
            public:
                /**
                 * @brief calculate the projection of point give the homography matrix
                 * @param point point to be converted
                 * @param H homography matrix
                 * @param x variable where the x-coordinate of the converted point will be stored
                 * @param y variable where the y-coordinate of the converted point will be stored
                 */
                static void calcProjection(const cv::Point2f &point, const cv::Mat &H, double &x, double &y);
            private:
                std::vector<cv::Point2f> srcPoints; 
                std::vector<cv::Point2f> dstPoints; 
                cv::Mat H;
                float error;
                config::YamlManager configFileWriter;
        };
    }
}


#endif
