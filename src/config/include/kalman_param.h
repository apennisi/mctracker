/*
 * Written by Andrea Pennisi
 */

#ifndef _KALMAN_PARAM_H_
#define _KALMAN_PARAM_H_

#include <iostream>
#include <fstream>
#include <algorithm>
#include <boost/algorithm/string.hpp>

namespace mctracker
{
    namespace config
    {
        class KalmanParam
        {
            public:
                /**
                 * @brief Constructor class KalmanParam
                 */
                KalmanParam() { ; }
                
                /**
                 * @brief set the number of times after which a track is accepted
                 * @param propagate unsigned integer containing the value of the propagation
                 */ 
                void
                setMinPropagate(uint propagate)
                {
                    min_propagate = propagate;
                }
                
                /**
                 * @brief get the number of times after which a track is accepted
                 * @return the number of times after which a track is accepted
                 */
                inline const uint 
                getMinpropagate() const
                {
                    return min_propagate;
                }
                
                /**
                 * @brief set the dt value for the transition matrix
                 * @param dt float containing the value of the sample frequency
                 */
                void
                setDt(const float& dt)
                {
                    d_t = dt;
                }
                
                /**
                 * @brief get the dt value of the transition matrix
                 * @return the dt value
                 */
                inline const float 
                getDt() const
                {
                    return d_t;
                }
                
                /**
                 * @brief get the association dummy cost value for the lienar assignment cost
                 * @return the value of the association cost
                 */
                inline const uint 
                getAssocdummycost() const
                {
                    return assoc_dummy_cost;
                }
                
                /**
                 * @brief set the association dummy cost for the lienar assignment cost
                 * @param ass unsigned integer containing the association value
                 */
                void
                setAssociationDummyCost(uint ass) 
                {
                    assoc_dummy_cost = ass;
                }
                
                /**
                 * @brief set the dummy cost for new hypothesis in the linear assignment cost
                 * @param nydc value containing the cost
                 */ 
                void
                setNewHypDummyCost(uint nydc)
                {
                    new_hyp_dummy_cost = nydc;
                }
                
                /**
                 * @brief get the dymmy cost for the new hypothesis in the linear assignment cost
                 * @return the value for the new hypothesis cost
                 */
                inline const uint 
                getNewhypdummycost() const
                {
                    return new_hyp_dummy_cost;
                }
                
                /**
                 * @brief set the maximum number of times after which a track is deleted
                 * @param missed the value containing the maximum number of times after which a track is deleted
                 */
                void
                setMaxMissed(uint missed)
                {
                    max_missed = missed;
                }
                
                /**
                 * @brief get the maximum number of times after which a track is deleted
                 * @return the value containing the maximum number of times after which a track is deleted
                 */
                inline const uint 
                getMaxmissed() const
                {
                    return max_missed;
                }
                
                /**
                 * @brief copy operator
                 * @param _param object to copy
                 * @return the pointer to the current instance
                 */ 
                KalmanParam& operator=(const KalmanParam& _param)
                {
                    this->min_propagate = _param.getMinpropagate();
                    this->d_t = _param.getDt();
                    this->assoc_dummy_cost = _param.getAssocdummycost();
                    this->new_hyp_dummy_cost = _param.getNewhypdummycost();
                    this->max_missed = _param.getMaxmissed();
                    return *this;
                }
                
                /**
                 * @brief print all the parameters
                 */
                void 
                print()
                {
                    std::cout << "[MINPROPAGATE]: " << min_propagate << std::endl;
                    std::cout << "[MAXMISSED]: " << max_missed << std::endl;
                    std::cout << "[ASSOCDUMMYCOST]: " << assoc_dummy_cost << std::endl;
                    std::cout << "[NEW_HYP_DUMMY_COST]: " << new_hyp_dummy_cost << std::endl;
                    std::cout << "[DT]: " << d_t << std::endl;
                }
            private:
                uint max_missed;
                uint min_propagate;
                float d_t;
                uint assoc_dummy_cost;
                uint new_hyp_dummy_cost;
        };
    }
}

#endif
