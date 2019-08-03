#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include "homography.h"

std::vector<cv::Point2f> mapPoints;
std::vector<cv::Point2f> rgbPoints;
std::string filename;

cv::Mat rgb;
cv::Mat map;

using namespace mctracker::geometry;

static void add(cv::Mat& img, std::vector<cv::Point2f>& points, int x, int y)
{
    cv::circle(img, cv::Point2f(x, y), 3, cv::Scalar(0, 0, 255), -1);
    points.push_back(cv::Point2f(x, y));
}

static void compute(const std::vector<cv::Point2f>& points_1, const std::vector<cv::Point2f>& points_2, cv::Mat& img_1, cv::Mat& img_2)
{
    if(points_1.size() == points_2.size()) 
    {
        Homography h;
        h.setPoints(points_1, points_2);
        cv::Mat H = h.calcHomography();
        h.draw(img_1, img_2, H);
        h.writeToFile(filename);
    } 
    else 
    {
        std::cout << "ERROR!!" << std::endl;
    }
}

void 
mouseHandlerRobot(int event, int x, int y, int flags, void *param) 
{
    if(event == CV_EVENT_LBUTTONDOWN) 
    {
        add(map, mapPoints, x, y);
        cv::imshow("map", map);
    }

    if(event == CV_EVENT_RBUTTONDOWN) 
    {
        compute(rgbPoints, mapPoints, rgb, map);
    }
}

void mouseHandlerRGB(int event, int x, int y, int flags, void *param) 
{
    if(event == CV_EVENT_LBUTTONDOWN) 
    {
        add(rgb, rgbPoints, x, y);
        cv::imshow("rgb", rgb);
    }

    if(event == CV_EVENT_RBUTTONDOWN) 
    {
        compute(rgbPoints, mapPoints, rgb, map);
    }
}



int main(int argc, char **argv) {

    if(argc != 4) 
    {
        std::cout << "ERROR: " << argv[0] << " rgb_image map_image save_file_name" << std::endl;
        exit(-1);
    }

    rgb = cv::imread(argv[1]);
    map = cv::imread(argv[2]);
    filename = std::string(argv[3]);


    cv::namedWindow("map", CV_WINDOW_AUTOSIZE);
    cv::namedWindow("rgb", CV_WINDOW_AUTOSIZE);

    cv::setMouseCallback("map", &mouseHandlerRobot, NULL);
    cv::setMouseCallback("rgb", &mouseHandlerRGB, NULL);


    cv::imshow("rgb", rgb);
    cv::imshow("map", map);

    cv::waitKey(0);

}
