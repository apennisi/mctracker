#include "yamlmanager.h"

using namespace mctracker::config;

YamlManager
::YamlManager()
{
    out << YAML::BeginMap;
}

bool
YamlManager::keyExists(const std::string& key)
{
    if(in.IsDefined())
    {
        if(in[key])
        {
            return true;
        }
    }
    else
    {
        throw std::invalid_argument("File YAML is not open!");
    }
    return false;
}



bool
YamlManager::write(const std::string& filepath)
{
    out << YAML::EndMap;
    std::ofstream fout(filepath);
    try
    {
        fout << out.c_str();
    }
    catch(const std::runtime_error& e)
    {
        std::cout << "ERROR: Cannot write the yaml file: " << filepath  << std::endl;
        return false;
    }
    fout.close();
    return true;
}

bool
YamlManager::read(const std::string& filepath)
{
    if(!exists(filepath))
    {
        throw std::invalid_argument("The File " + filepath + " does not exist!!");
        exit(-1);
    }
    try
    {
        in = YAML::LoadFile(filepath);
    }
    catch(const std::runtime_error& e)
    {
        std::cout << "ERROR: Cannot read the yaml file: " << filepath << std::endl;
        return false;
    }
    return true;
}

void
YamlManager::writeElem(const std::string& key, const cv::Mat& elem)
{
    try
    {
        out << YAML::Key << key;
    }
    catch(const std::runtime_error& e)
    {
        std::cout << "ERROR: Cannot write the key: " << key << std::endl;
    }
    
    
    auto wirteMatrix = [](auto vec, YAML::Emitter& out)
    {
        try
        {
            out << YAML::Flow << vec;
        }
        catch(const std::runtime_error& e)
        {
            std::cout << "ERROR: Cannot write the matrix"  << std::endl;
            return false;
        }
        return true;
    };
    
    const int depth = elem.type();

    switch (depth)
    {
        case CV_32FC1:  
        {
            std::vector<float> vec(elem.begin<float>(), elem.end<float>());
            if(!wirteMatrix(vec, out))
                throw std::invalid_argument("ERROR: Cannot write the Key : " + key);
            break;
        }
        case CV_64FC1: 
        {
            std::vector<double> vec(elem.begin<double>(), elem.end<double>());
            if(!wirteMatrix(vec, out))
                throw std::invalid_argument("ERROR: Cannot write the Key : " + key);
            break;
        }
        
        default:
            throw std::invalid_argument("Invalid depth type of matrix!");
    }
}


void
YamlManager::writeElem(const std::string& key, const float& elem)
{
    try
    {
        out << YAML::Key << key;
    }
    catch(const std::runtime_error& e)
    {
        std::cout << "ERROR: Cannot write the key: " << key << std::endl;
    }
    try
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << elem << YAML::EndSeq;
    }
    catch(const std::runtime_error& e)
    {
        std::cout << "ERROR: Cannot write the elem: " << elem << std::endl;
    }

}

void
YamlManager::writeElem(const std::string& key, const cv::Point& elem)
{
    try
    {
        out << YAML::Key << key;
    }
    catch(const std::runtime_error& e)
    {
        std::cout << "ERROR: Cannot write the key: " << key << std::endl;
    }
    try
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << elem.x << elem.y << YAML::EndSeq;
    }
    catch(const std::runtime_error& e)
    {
        std::cout << "ERROR: Cannot write the elem: " << elem << std::endl;
    }

}

bool
YamlManager::getElem(const std::string& name, bool& elem)
{
    if(in[name])
    {
        try
        {
            elem = in[name].as<bool>();
        }
        catch(const std::runtime_error& e)
        {
            std::cout << "ERROR: Cannot read element " << name << e.what() << std::endl;
            return false;
        }
    }
    else
    {
        return false;
    }
    return true;
}


bool
YamlManager::getElem(const std::string& name, int& elem)
{
    if(in[name])
    {
        try
        {
            elem = in[name].as<int>();
        }
        catch(const std::runtime_error& e)
        {
            std::cout << "ERROR: Cannot read element " << name << e.what() << std::endl;
            return false;
        }
    }
    else
    {
        return false;
    }
    return true;
}


bool
YamlManager::getElem(const std::string& name, float& elem)
{
    if(in[name])
    {
        try
        {
            elem = in[name].as<float>();
        }
        catch(const std::runtime_error& e)
        {
            std::cout << "ERROR: Cannot read element " << name << e.what() << std::endl;
            return false;
        }
    }
    else
    {
        return false;
    }
    return true;
}

bool
YamlManager::getElem(const std::string& name, double& elem)
{
    if(in[name])
    {
        try
        {
            elem = in[name].as<double>();
        }
        catch(const std::runtime_error& e)
        {
            std::cout << "ERROR: Cannot read element " << name << e.what() << std::endl;
            return false;
        }
    }
    else
    {
        return false;
    }
    return true;
}


bool
YamlManager::getElem(const std::string& name, std::string& elem)
{
    if(in[name])
    {
        try
        {
            elem = in[name].as<std::string>();
        }
        catch(const std::runtime_error& e)
        {
            std::cout << "ERROR: Cannot read element " << name << e.what() << std::endl;
            return false;
        }
    }
    else
    {
        return false;
    }
    return true;
}

bool
YamlManager::getElem(const std::string& name, std::vector<int>& elem)
{
    if(in[name] && in[name].IsSequence())
    {
        uint i = 0;
        for(const auto& e : in[name])
        {
            try
            {
                elem.push_back(e.as<int>());
            }
            catch(const std::runtime_error& e)
            {
                std::cout << "ERROR: Cannot read element " << i << e.what() << std::endl;
                return false;
            }
        }
    }
    else
    {
        return true;
    }
    return false;
}

bool
YamlManager::getElem(const std::string& name, std::vector<float>& elem)
{
    if(in[name] && in[name].IsSequence())
    {
        uint i = 0;
        for(const auto& e : in[name])
        {
            try
            {
                elem.push_back(e.as<float>());
            }
            catch(const std::runtime_error& e)
            {
                std::cout << "ERROR: Cannot read element " << i << e.what() << std::endl;
                return false;
            }
        }
    }
    else
    {
        return true;
    }
    return false;
}


bool
YamlManager::getElem(const std::string& name, std::vector<double>& elem)
{
    if(in[name] && in[name].IsSequence())
    {
        uint i = 0;
        for(const auto& e : in[name])
        {
            try
            {
                elem.push_back(e.as<double>());
            }
            catch(const std::runtime_error& e)
            {
                std::cout << "ERROR: Cannot read element " << i << e.what() << std::endl;
                return false;
            }
        }
    }
    else
    {
        return true;
    }
    return false;
}

bool
YamlManager::getElem(const std::string& name, std::vector< cv::Point2f >& elem)
{
    if(in[name] && in[name].IsSequence())
    {
        for (YAML::iterator it = in[name].begin(); it != in[name].end(); ++it)
        {
            try
            {
                elem.push_back(cv::Point2f((*it).as<float>(), (*(++it)).as<float>()));
            }
            catch(const std::runtime_error& e)
            {
                std::cout << "ERROR: Cannot read element " << e.what() << std::endl;
                return false;
            }
        }
        return true;
    }
    return false;
}



bool
YamlManager::getElem(const std::string& name, cv::Mat& elem, int type)
{
    if(in[name] && in[name].IsSequence())
    {
        elem = cv::Mat(cv::Size(3, 3), type);
        uint i = 0;
        for(const auto& e : in[name])
        {
            try
            {
                if(type == CV_32FC1)
                    elem.at<float>(i++) = e.as<float>();
                else if(type == CV_64FC1)
                    elem.at<double>(i++) = e.as<double>();
            }
            catch(const std::runtime_error& e)
            {
                std::cout << "ERROR: Cannot read element " << i << e.what() << std::endl;
                return false;
            }
        }
        return true;
    }
    else
    {
        return true;
    }
    return false;
}

bool
YamlManager::getElem(const std::string& name, cv::Point2i& elem)
{
    if(in[name] && in[name].IsSequence() && in[name].size() == 2)
    {
        try
        {
            elem.x = in[name][0].as<int>();
            elem.y = in[name][1].as<int>();
        }
        catch(const std::runtime_error& e)
        {
            std::cout << "ERROR: Cannot read element " << name << e.what() << std::endl;
            return false;
        }
    }
    else
    {
        return false;
    }
    return true;
}

bool
YamlManager::getElem(const std::string& name, cv::Point2f& elem)
{
    if(in[name] && in[name].IsSequence() && in[name].size() == 2)
    {
        try
        {
            elem.x = in[name][0].as<float>();
            elem.y = in[name][1].as<float>();
        }
        catch(const std::runtime_error& e)
        {
            std::cout << "ERROR: Cannot read element " << name << e.what() << std::endl;
            return false;
        }
    }
    else
    {
        return false;
    }
    return true;
}

bool
YamlManager::getElem(const std::string& name, cv::Point2d& elem)
{
    if(in[name] && in[name].IsSequence() && in[name].size() == 2)
    {
        try
        {
            elem.x = in[name][0].as<double>();
            elem.y = in[name][1].as<double>();
        }
        catch(const std::runtime_error& e)
        {
            std::cout << "ERROR: Cannot read element " << name << e.what() << std::endl;
            return false;
        }
    }
    else
    {
        return false;
    }
    return true;
}


