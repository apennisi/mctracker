/*
 * Written by Andrea Pennisi
 */

#ifndef _HYPHOTHESIS_H_
#define _HYPHOTHESIS_H_

#include <iostream>
#include <memory>
#include <opencv2/opencv.hpp>

#include <iostream>
#include "lap.h"
#include "hungarianAlg.h"
#include "detection.h"
#include "kalman_param.h"
#include "track.h"
#include "utils.h"

using namespace mctracker::tracker::costs;

namespace mctracker 
{
    namespace tracker
    {
        class Hyphothesis
        {
            private:
                typedef std::shared_ptr<Track> Track_ptr;
                typedef std::vector<Track_ptr> Tracks;
            public:
                static std::shared_ptr<Hyphothesis> instance();
                /**
                 * @brief compare the previous unassigned points to the new detection in order to create new tracks
                 * @param dummmy_assignments a matrix containing the assignment coming from the hungarian algorithm
                 * @param tracks the se of the current tracks
                 * @param w width of the tracking space
                 * @param h height of the tracking space
                 * @param new_hyp_dummy_costs a dummy cost for making a new hyphothesis
                 * @param prev_unassigned vector containing the previous unassigned detections
                 * @param param kalman parameters
                 * @param cameraNum camera number
                 */
                void new_hyphothesis(const cv::Mat& dummmy_assignments, Tracks& tracks, const Detections& detections, const uint& w, const uint& h,
                                                        const uint& new_hyp_dummy_costs, Detections& prev_unassigned, const KalmanParam& param, const int cameraNum);
            private:
                static std::shared_ptr<Hyphothesis> m_instance;
            private:
                // 15 sig. digits for 0<=real(z)<=171
                // coeffs should sum to about g*g/2+23/24	
                constexpr static float g = 4.7422;
                constexpr static float sq2pi=  2.5066282746310005024157652848110;
            private:
                 /**
                * @brief Constructor class Hyphothesis
                */
                Hyphothesis() { ; }
                /**
                 * @brief compute th beta likelihood given a points
                 * @param prev_unassigned a point previously unassigned
                 * @param alpha horizontal range of the detection
                 * @param beta vertical range of the detection
                 * @param w width of the tracking space
                 * @param h height of the tracking space
                 * @return the beta likelihood
                 */
                float beta_likelihood(const cv::Point2f& prev_unassigned, const float& alpha, const float& beta, const uint& w, const uint& h);
                /**
                 * @brief compute the gamma function
                 * @param z may be complex and of any size.
                 * @return the gamma function value
                 */
                float gamma(const float& z);
        };
    }
}

#endif
