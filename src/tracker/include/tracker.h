/*
 * Written by Andrea Pennisi
 */


#ifndef _TRACKER_H_
#define _TRACKER_H_

#include <iostream>
#include <utility>
#include <vector>
#include <functional>
#include <iterator>

#include "entity.h"
#include "kalman_param.h"
#include "track.h"
#include "hypothesis.h"
#include "detection.h"
#include "hungarianAlg.h"
#include "utils.h"
#include "camera.h"

using namespace mctracker::utils;
using namespace mctracker::config;

namespace mctracker
{
    namespace tracker
    {
        typedef std::shared_ptr<Entity> Entity_ptr;
        typedef std::vector<Entity_ptr> Entities;

        class Tracker
        {
            friend class Entity;
            friend class Track;

            private:
                typedef std::shared_ptr<Track> Track_ptr;
                typedef std::vector<Track_ptr> Tracks;
            public:
                /**
                 * @brief Constructor class Tracker
                 * @param _param kalaman parameters
                 * @param camerastack the camera stack stack 
                 */
                Tracker(const KalmanParam& _param, const std::vector<Camera>& camerastack);
                /**
                 * @brief track the detections
                 * @param _detections the current detections coming from all the cameras
                 * @param w width of the tracking space
                 * @param h height of the tracking space
                 */
                void track(std::vector<Detections>& _detections,  const int& w, const int& h);
            public:
                /**
                 * @brief set the tracking space size
                 * @param _w width of the tracking space
                 * @param _h height of the tracking space
                 */
                inline const void 
                setSize(const uint& _w, const uint& _h)
                {
                    width = _w;
                    height = _h;
                }
                
                /**
                 * @brief evolve the tracks in order to compute the predictions
                 */
                inline const void 
                evolveTracks()
                {
                    for(const auto& track : single_tracks)
                    {
                        track->predict();
                    }
                }
                
                /**
                 * @brief get the current tracks
                 * @return a vector containing the tracks
                 */
                const Entities getTracks();
            private:
                
                /**
                 * @brief compute the association between the detections and the tracks
                 * @param _detections a vector containing all the detections
                 * @retun a cv::Mat containing the association track-detection
                 */
                cv::Mat associate_tracks(const Detections& _detections);
                
                /**
                 * @brief update the tracks 
                 * @param assigments a vector containing the assigments between the detections of each camera and the old tracks
                 * @param _detections a vector containing all the detections
                 */
                void update_tracks(const std::vector<cv::Mat>& assigments, const std::vector<Detections>& _detections);
                
                /**
                 * @brief delete/freeze the tracks for which no detections are available
                 */
                void delete_tracks();
                
                /**
                 * @brief check if the freezed tracks can be associated to the new detections
                 * @param _detections a vector containing all the detections
                 */
                void check_old_tracks(std::vector< Detections >& _detections);
                
                /**
                 * @brief refine the input detections
                 * @param _detections a vector containing all the detections
                 */
                void refine_detections(std::vector<Detections>& _detections);
                
                /**
                 * @brief compute the first association between cameras 
                 * @param _detections a vector containing all the detections
                 * @return a vector containing the associated points
                 */
                Detections first_assosiation(std::vector<Detections>& _detections);
            private:
                KalmanParam param;
                Detections last_detection;
                Detections prev_unassigned;
                Tracks single_tracks;
                Tracks old_tracks;
                Entities tracks;
                uint width, height;
                cv::RNG rng;
                uint trackIds;
                std::vector<Camera> streams;
                //storing the probabilities of the camera system
                //each value represensts: 
                //  -prob that the obs is in the FOV of all the cameras given the proximity of the camera
                std::vector<float > cameraProbabilities; 
                int numCams;
            private:
                static constexpr float freezed_thresh = 0.4;
                static constexpr uint association_thresh = 40;
        };
    }
}

#endif
