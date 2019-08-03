#include "camerastack.h"

using namespace mctracker;
using namespace mctracker::utils;

CameraStack
::CameraStack(const std::vector<CameraParam>& params)
{
    open_streams(params);
}

void
CameraStack::open_streams(const std::vector<CameraParam>& params)
{
    for(const auto& param : params)
    {
        Camera cam(param.getStream(), param.getFOV(), param.getHomography(), param.getProximity());
        streams.push_back(cam);
    }
}

bool
CameraStack::getFrame(std::vector<cv::Mat>& frames)
{
    if(frames.size() !=0)
        frames.clear();
    
    cv::Mat frame;
    
    for(auto& stream : streams)
    {
        if(!stream.getFrame(frame))
            return false;
        frames.push_back(frame.clone());
    }
    return true;
}
