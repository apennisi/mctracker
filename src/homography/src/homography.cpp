#include "homography.h"

using namespace mctracker;
using namespace mctracker::geometry;

void 
Homography::setPoints(const std::vector<cv::Point2f>& src, const std::vector<cv::Point2f>& dst)
{
	auto itPts = srcPoints.begin();
	
    for(const auto& point : src)
    {
		srcPoints.push_back(point);
	}
	
    for(const auto& point : dst)
    {
		dstPoints.push_back(point);
	}
}


cv::Mat 
Homography::calcHomography() 
{

	std::cout << "\nsrc points: " << std::endl;
	
    for(const auto& point : srcPoints)
	{
        std::cout << point << std::endl;
	}

	std::cout << "\ndst points: " << std::endl;

    for(const auto& point : dstPoints)
    {
        std::cout << point << std::endl;
	}

	H = cv::findHomography(
						cv::Mat(srcPoints), cv::Mat(dstPoints),  // corresponding points
						//inliers,	// outputed inliers matches
						0, 			//force all corrispondences to be used
						0.5);	    // max distance to reprojection point

	std::cout << std::endl;
	std::cout << "MATRIX H" << std::endl;
    std::cout << H << std::endl;
	
    cv::Mat invH = H.inv();

    std::cout << std::endl;
    std::cout << "INV MATRIX H" << std::endl;
    std::cout << invH << std::endl;

	return H;
}


void 
Homography::draw(const cv::Mat& srcImage, const cv::Mat& dstImage, const cv::Mat& H) {
	
    cv::Mat dstMat;
	dstImage.copyTo(dstMat);
	cv::Mat srcMat;
	srcImage.copyTo(srcMat);

	// Draw the inlier points
	std::vector<cv::Point2f>::const_iterator itPts = dstPoints.begin();

    for(const auto& point : dstPoints)
    {
		cv::circle(dstMat, point, 6, cv::Scalar(120,120,120), 2);
	}

    for(const auto& point : srcPoints)
    {
		cv::circle(srcMat, point, 6, cv::Scalar(120,120,120), 2);
		++itPts;
	}

	// Display the images with points
	cv::namedWindow("Dst Points");
	cv::imshow("Dst Points", dstMat);
	cv::namedWindow("Src Points");
	cv::imshow("Src Points",srcMat);

	//reprojection
	std::cout << "Reprojection..." << std::endl;

	cv::Mat reprojectionImage = dstMat.clone();

	int cnt = 0;
    auto dist = [] (const cv::Point2f& p1, const cv::Point2f& p2)
    {
        const auto& p = p1 - p2;
        return sqrt(p.x * p.x + p.y * p.y);
    };
    
    error = 0.0;
    auto ii = 0; 
    for(const auto& point : srcPoints)
    {
        double x, y;
 		cv::Point2f p(point.x, point.y);
		calcProjection(p, H, x, y);

		std::cout << "(" << p.x << "," << p.y << ") --> (" << x << "," << y << ")" << std::endl;
        error += dist(dstPoints.at(ii++), cv::Point2f(x, y));

		if(x < 0 || y < 0) 
        {
			std::cout << "WARNING: Negative reprojection values" << std::endl;
		}
		else {
			cv::line(reprojectionImage, cv::Point2f(x, y),
				cv::Point2f(x+10, y), CV_RGB(0,0,255), 2);
			cv::line(reprojectionImage, cv::Point2f(x, y),
				cv::Point2f(x-10, y), CV_RGB(0,0,255), 2);
			cv::line(reprojectionImage, cv::Point2f(x, y),
				cv::Point2f(x, y+10), CV_RGB(0,0,255), 2);
			cv::line(reprojectionImage, cv::Point2f(x, y),
				cv::Point2f(x, y-10), CV_RGB(0,0,255), 2);

			std::string s;
			std::stringstream out;
			out << cnt;
			s.assign(out.str());
			cv::putText(reprojectionImage, s, cv::Point2f(x+5,y-5),
						CV_FONT_HERSHEY_SIMPLEX, 0.6, CV_RGB(0, 0, 255));
		}
	}
	
	error /= srcPoints.size();
    
    std::cout << "Reprojection error: " << error << std::endl;

	cv::namedWindow("Reprojected Points");
	cv::imshow("Reprojected Points", reprojectionImage);

	cv::Mat warpImage = cv::Mat::zeros(dstMat.size(), dstMat.type());
	cv::warpPerspective(srcImage, warpImage, H, dstMat.size());
    
    
    cv::Mat total;
    cv::addWeighted(warpImage, 0.7, dstImage, 1., 0, total);
    
    cv::imshow("Total", total);

	cv::namedWindow("Warp Image");
	cv::imshow("Warp Image", warpImage);

	cv::waitKey(0);
}

void 
Homography::calcProjection(const cv::Point2f &point, const cv::Mat &H, double &x, double &y) 
{
    if(H.empty())
    {
        throw std::invalid_argument("Invaling homography");
    }
    
	x =	(point.x*H.at<double>(0,0)+point.y*H.at<double>(0,1)+H.at<double>(0,2))/
			(point.x*H.at<double>(2,0)+point.y*H.at<double>(2,1)+H.at<double>(2,2));
	y =	(point.x*H.at<double>(1,0)+point.y*H.at<double>(1,1)+H.at<double>(1,2))/
			(point.x*H.at<double>(2,0)+point.y*H.at<double>(2,1)+H.at<double>(2,2));
}

void
Homography::writeToFile(const std::string& filepath)
{
    if(H.empty())
    {
        throw std::invalid_argument("Homography Matrix is empty");
    }

    configFileWriter.writeElem("Matrix", H);
    configFileWriter.writeElem("Error", error);
    configFileWriter.write(filepath);
}



