/*
 * Written by Andrea Pennisi
 */

#ifndef _DETECTOR_PARAM_H_
#define _DETECTOR_PARAM_H_

namespace mctracker
{
    namespace config
    {
        class DetectorParam
        {
            public:
                /**
                 * @brief Constructor class DetectorParam
                 */
                DetectorParam() { ; }
                
                /**
                 * @brief set the path to the weight file
                 * @param w path to the weight file
                 */
                void
                setWeights(const std::string& w)
                {
                    weights = w;
                }
                
                /**
                 * @brief get the path to the file containing the weights
                 * @return a string containing the path
                 */
                inline const std::string
                getWeights() const
                {
                    return weights;
                }
                
                /**
                 * @brief set the path to the file containing the network configuration
                 * @param c value containing the path to the config file
                 */
                void
                setConfig(const std::string& c)
                {
                    config = c; 
                }
                
                /**
                 * @brief get the path to the file containing the network configuration
                 * @return a string containing the path to the file containing the network configuration
                 */
                inline const std::string
                getConfig() const
                {
                    return config;
                }
                
                /**
                 * @brief set the minimum threshold above which a detection is considered
                 * @param thres value contianing the minimum threshold above which a detection is considered
                 */
                void
                setThreshold(const float& thres)
                {
                    minThreshold = thres;
                }
                
                /**
                 * @brief get the minimum threshold above which a detection is considered
                 * @return a float containing the minimum threshold above which a detection is considered
                 */
                inline const float
                getThreshold() const
                {
                    return minThreshold;
                }
                
                /**
                 * @brief copy operator
                 * @param _param object to copy
                 * @return the pointer to the current instance
                 */ 
                DetectorParam& operator=(const DetectorParam& _param)
                {
                    config = _param.getConfig();
                    weights = _param.getWeights();
                    minThreshold = _param.getThreshold();
                    return *this;
                }
                
                /**
                 * @brief print all the parameters
                 */
                void 
                print()
                {
                    std::cout << "[CONFIG]: " << config << std::endl;
                    std::cout << "[WEIGHTS]: " << weights << std::endl;
                    std::cout << "[THRESHOLD]: " << minThreshold << std::endl;
                }
                
            private:
                std::string config;
                std::string weights;
                float minThreshold;
        };
    }
}

#endif
