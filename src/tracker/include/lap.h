/************************************************************************
*
*  lap.h
   version 1.0 - 21 june 1996
   author  Roy Jonker, MagicLogic Optimization Inc.
   modified by Andrea Pennisi
   
   header file for LAP
*
**************************************************************************/

#ifndef _LAP_H_
#define _LAP_H_

#include <stdio.h>
#include <iostream>
#include <vector>
#include <memory>
#include <limits>

namespace mctracker
{
    namespace tracker
    {
        namespace costs
        {
            class LapCost
            {
                public:
                    static std::shared_ptr<LapCost> instance();
                    /**
                     * @brief compute the linear association costs
                     * @param dim problem size
                     * @param assigncost cost matrix
                     * @param rowsol column assigned to row in solution
                     * @param colsol row assigned to column in solution
                     * @param u dual variables, row reduction numbers
                     * @param v dual variables, column reduction numbers
                     * @return an integer representing the lap cost
                     */
                    int lap(int dim, const std::vector< std::vector<int> >& assigncost, std::vector<int>& rowsol, std::vector<int>& colsol, std::vector<int>& u, std::vector<int>& v);
                private:
                    typedef int row;
                    typedef int col;
                    typedef int cost;
                private:
                    static std::shared_ptr<LapCost> m_instance;
                    /**
                    * @brief Constructor class LapCost
                    */
                    LapCost() { ; }
                private:
                    constexpr static int BIG = std::numeric_limits<int>::max();
            };
        }
    }
}

#endif
