#ifndef _BGSUBTRACTION_H_
#define _BGSUBTRACTION_H_

#include <iostream>
#include <opencv2/opencv.hpp>

namespace mctracker
{
    namespace segmentation
    {
        class BgSubtraction
        {
            public:
                /**
                * @brief Constructor class BgSubtraction
                */
                BgSubtraction();
                /**
                * @brief compute the background model
                * @param frame cv::Mat containing the current frame
                * @param morph a boolean which if is true enable morphology operations on the resulting foreground mask
                */
                void process(const cv::Mat& frame, bool morph = true);
            public:
                /**
                 * @brief get the foreground mask
                 * @param mask cv::Mat variable in which the mask will be stored
                 * @return true if the mask is ready, false otherwise
                 */
                inline bool 
                getFgMask(cv::Mat& mask) const
                {
                    if(frameNum >= 2)
                    {
                        mask = fgMask.getMat(cv::ACCESS_WRITE).clone();
                        return true;
                    }
                    return false;
                }
            private:
                cv::Ptr<cv::BackgroundSubtractorKNN> pMOG2;
                double learningRate;
                int frameNum;
                cv::UMat fgMask;
                cv::UMat ref_frame;
        };
    }
}

#endif
