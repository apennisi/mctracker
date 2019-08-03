#include "camera.h"

using namespace mctracker;
using namespace mctracker::utils;

Camera
::Camera(const std::string& stream, const std::string& view, const std::string& homography_file,  bool prox)
    : proximity(prox)
{
    cap.open(stream);
    if(!cap.isOpened())
    {
        throw std::invalid_argument("Invalid Stream File: " + stream);
    }
    
    try
    {
        image_view = cv::imread(view, cv::IMREAD_GRAYSCALE);
    }
    catch(cv::Exception& ex)
    {
        std::cout << "Invalid image name: " << view << std::endl;
        std::cout << ex.what() << std::endl;
        exit(-1);
    }
    
    YamlManager homographyReader;
    homographyReader.read(homography_file);
    
    if(!homographyReader.getElem("Matrix", H, CV_64FC1))
    {
        throw std::invalid_argument("Homography is not specified!");
    }
    
    Hinv = H.inv();
}

bool 
Camera::getFrame(cv::Mat& frame)
{
    cap >> frame;
    if(frame.empty())
    {
        return false;
    }
    return true;
}

cv::Point2f 
Camera::camera2world(const cv::Point2f& p) const
{
    double x, y;
    mctracker::geometry::Homography::calcProjection(p, H, x, y);
    return cv::Point2f(x, y);
}

cv::Point2f 
Camera::world2camera(const cv::Point2f& p) const
{
    double x, y;
    mctracker::geometry::Homography::calcProjection(p, Hinv, x, y);
    return cv::Point2f(x, y);
}
