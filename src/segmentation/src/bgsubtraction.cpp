#include "bgsubtraction.h"

using namespace mctracker;
using namespace mctracker::segmentation;

BgSubtraction
::BgSubtraction()
{
    pMOG2 = cv::createBackgroundSubtractorKNN();
    frameNum = 0;
}

void
BgSubtraction::process(const cv::Mat& frame, bool morph)
{
    //Decrease the learning rate when the background model becomes more reliable
    if (frameNum < 100) 
    {
        learningRate = -1;
    } 
    else if (frameNum < 1000) 
    {
        learningRate = -.1;
    } 
    else 
    {
        learningRate = -0.001;
    }

    cv::UMat frameUMat = frame.getUMat(cv::ACCESS_WRITE);
    cv::blur(frameUMat, ref_frame, cv::Size(15, 15));   
    
    pMOG2->apply(ref_frame, fgMask, learningRate);
    cv::compare(fgMask, cv::Scalar(255), fgMask, cv::CMP_EQ);
    
    if(morph)
    {
        cv::erode(fgMask, fgMask, cv::Mat(), cv::Point(-1, -1), 1);
        cv::dilate(fgMask, fgMask, cv::Mat(), cv::Point(-1, -1), 2);
    }
    
    frameUMat.release();
    
    frameNum++;
}
