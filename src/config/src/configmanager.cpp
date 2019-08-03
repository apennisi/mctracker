#include "configmanager.h"

using namespace mctracker::config;

bool 
ConfigManager::read(const std::string& config)
{
    if(!exist(config))
    {
        throw std::invalid_argument("Invalid file name: " + config);
    }
    yamlManager.read(config);
    
    if(!yamlManager.getElem("Number", cameraNum))
    {
        std::cout << "Camera Number is not specified!" << std::endl;
        return false;
    }
    
    std::string planViewName;
    if(!yamlManager.getElem("Planview", planViewName))
    {
        std::cout << "Planview is not specified!" << std::endl;
        return false;
    }
    
    planView = cv::imread(planViewName);
    
    if(!yamlManager.getElem("Show Planview", show))
    {
       show = false;
    }
    
    std::stringstream ss;
    for(auto i = 0; i < cameraNum; ++i)
    {
        CameraParam param;
        ss << "Camera" << i+1;
        std::string tmpVar;
        if(!yamlManager.getElem(ss.str(), tmpVar))
        {
            std::cout << "Camera"<< i+1 << " Stream is not specified!" << std::endl;
            return false;
        }
        param.setStream(tmpVar);
        
        ss.str("");
        ss << "Homography" << i+1;
        if(!yamlManager.getElem(ss.str(), tmpVar))
        {
            std::cout << "Homography"<< i+1 << " is not specified!" << std::endl;
            return false;
        }
        param.setHomography(tmpVar);
        
        ss.str("");
        ss << "FOV" << i+1;
        if(!yamlManager.getElem(ss.str(), tmpVar))
        {
            std::cout << "FOV"<< i+1 << " is not specified!" << std::endl;
            return false;
        }
        param.setFOV(tmpVar);
        
        ss.str("");
        ss << "Proximity" << i+1;   
        bool prox;
        if(!yamlManager.getElem(ss.str(), prox))
        {
            std::cout << "Proximity"<< i+1 << " is not specified!" << std::endl;
            return false;
        }
        ss.str("");
        param.setProximity(prox);
        cameraParam.push_back(param);
    }
    
    
    std::string kalman_file;
    if(!yamlManager.getElem("Kalman", kalman_file))
    {
        std::cout << "No Kalman file found, predefined values will be use." << std::endl;
        kalmanParam.setAssociationDummyCost(50);
        kalmanParam.setNewHypDummyCost(2);
        kalmanParam.setDt(1.0);
        kalmanParam.setMaxMissed(15);
        kalmanParam.setMinPropagate(15);
    }
    else
    {   
        if(!exist(kalman_file))
        {
            throw std::invalid_argument("Invalid file name: " + kalman_file);
        }
        
        YamlManager kalmanReader;
        kalmanReader.read(kalman_file);
        
        
        int tmpValue;
        if(!kalmanReader.getElem("ACD", tmpValue))
        {
            kalmanParam.setAssociationDummyCost(50);
        } 
        kalmanParam.setAssociationDummyCost(tmpValue);
        
        if(!kalmanReader.getElem("NHDC", tmpValue))
        {
            kalmanParam.setNewHypDummyCost(2);
        } 
        kalmanParam.setNewHypDummyCost(uint(tmpValue) );
        
        if(!kalmanReader.getElem("MINPROPAGATE", tmpValue))
        {
            kalmanParam.setMinPropagate(15);
        } 
        kalmanParam.setMinPropagate(uint(tmpValue) );
        
        if(!kalmanReader.getElem("MAXMISSED", tmpValue))
        {
            kalmanParam.setMaxMissed(15);
        } 
        kalmanParam.setMaxMissed(uint(tmpValue) );
        
        float dt;
        if(!kalmanReader.getElem("DT", dt))
        {
            kalmanParam.setMaxMissed(1.0);
        } 
        kalmanParam.setDt( dt );
    }
    
    std::string detectorTmpVal;
    
    if(!yamlManager.getElem("Config", detectorTmpVal))
    {
        std::cout << "Config is not specified!" << std::endl;
        return false;
    }
    
    if(!exist(detectorTmpVal))
    {
        throw std::invalid_argument("Invalid file name: " + detectorTmpVal);
    }
    
    detectorParam.setConfig(detectorTmpVal);
    
    if(!yamlManager.getElem("Weights", detectorTmpVal))
    {
        std::cout << "Weights is not specified!" << std::endl;
        return false;
    }
    
    if(!exist(detectorTmpVal))
    {
        throw std::invalid_argument("Invalid file name: " + detectorTmpVal);
    }
    
    detectorParam.setWeights(detectorTmpVal);
    
    float thresh;
    
    if(!yamlManager.getElem("MinThreshold", thresh))
    {
        thresh = .8;
    }
    
    detectorParam.setThreshold(thresh);

    return true;    
}

void 
ConfigManager::print()
{
    auto i = 0;
    for(auto& param : cameraParam)
    {
        std::cout << "CAMERA " << i+1 << std::endl;
        param.print();
        i++;
        std::cout << std::endl;
    }
    
    std::cout << std::endl;
    std::cout << "KALMAN" << std::endl;
    kalmanParam.print();
    
    std::cout << std::endl;
    std::cout << "DETECTOR" << std::endl;
    detectorParam.print();
}

bool 
ConfigManager::exist(const std::string& name) const
{
    if(name.find("://")!=std::string::npos) return true;

    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}
