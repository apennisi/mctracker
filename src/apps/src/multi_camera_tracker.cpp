#include <iostream>
#include <opencv2/opencv.hpp>

#include "object_detector.h"
#include "camerastack.h"
#include "bgsubtraction.h"
#include "track.h"
#include "tracker.h"
#include "kalman_param.h"
#include "object_detector.h"
#include "configmanager.h"
#include "homography.h"
#include "utility.h"


using namespace mctracker;
using namespace mctracker::config;
using namespace mctracker::utils;
using namespace mctracker::objectdetection;
using namespace mctracker::segmentation;


auto main(int argc, char **argv) -> int
{
    if(argc != 2)
    {
        std::cout << "Error: too few/much arguments!" << std::endl;
        std::cout << "Usage: " << argv[0] << " /path/to/the/config/file" << std::endl;
        exit(-1);
    }
    
    ConfigManager config;
    config.read(std::string(argv[1]));
    config.print();
    
    const int w = config.getPlaview().cols;
    const int h = config.getPlaview().rows;
    
    //initialize all the main objects
    CameraStack streams(config.getCameraParam());
    ObjectDetector detector(config.getDetectorParam());
    std::vector<BgSubtraction> bgSub(config.getCameraNumber());
    Tracker tr(config.getKalmanParam(), streams.getCameraStack());
    
    //set tracker space
    tr.setSize(w, h);
    
    //storing variables
    std::vector<cv::Mat> frames;
    std::vector<cv::Mat> detectionFrames(config.getCameraNumber());
    std::vector<cv::Mat> trackingFrames(config.getCameraNumber());
    std::vector<cv::Mat> fgMasks(config.getCameraNumber());
    std::vector< std::vector<bbox_t> > detections(config.getCameraNumber());
    bool compute = false;
    cv::Mat imageTracks;
    
    auto& cameras = streams.getCameraStack();
    
    while(streams.getFrame(frames))
    {
        imageTracks = config.getPlaview().clone();
        
        std::stringstream ss;
        auto i = 0;
        
        for(const auto& frame : frames)
        {
            detectionFrames.at(i) = frame.clone();
            trackingFrames.at(i) = frame.clone();
            cv::Mat fgMask;
            bgSub.at(i).process(frame);
            compute = bgSub.at(i).getFgMask(fgMask);
            if(compute)
            {
                fgMasks.at(i) = fgMask.clone();
                detector.classify(detectionFrames.at(i), false);
                detections.at(i) = detector.detections();
            }
            i++;
        }
        
        if(compute)
        {
            auto observations = 
                Utility::dets2Obs(detections, frames, fgMasks, cameras);
            
            tr.track(observations, w, h);
            
            const auto& tracks = tr.getTracks();
            for(auto& track : tracks)
            {
                track->drawTrack(trackingFrames, cameras);
                if(config.showPlanView())
                    track->drawTrackPlanView(imageTracks);
            }
            
            cv::imshow("TRACKING", Utility::makeMosaic(trackingFrames));
            if(config.showPlanView())
                cv::imshow("TRACKS", imageTracks);
            
            cv::waitKey(1);
        }
    }
    return 0;
}
