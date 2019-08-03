/************************************************************************
*
*  hungarianAlg.h
   author  Andrey Smorodov
   modified by Andrea Pennisi
   
   header file for Hungarian algorithm
*
**************************************************************************/

#ifndef _HUNGARIAN_ALG_H_
#define _HUNGARIAN_ALG_H_

#include <vector>
#include <iostream>
#include <limits>
#include <time.h>
#include <assert.h>

#include "utils.h"

typedef float track_t;
typedef std::vector<int> assignments_t;
typedef std::vector<track_t> distMatrix_t;

namespace mctracker
{
    namespace tracker
    {
        namespace costs
        {
            class AssignmentProblemSolver
            {
                 public:
                    // enum containing the Method for searching the optimal solutions
                    enum TMethod
                    {
                        optimal,
                        many_forbidden_assignments,
                        without_forbidden_assignments
                    };
                public:
                    /**
                    * @brief Constructor class AssignmentProblemSolver
                    */
                    AssignmentProblemSolver() { ; }
                    /**
                     * @brief compute the hungarian algorithm
                     * @param distMatrix matrix containing the costs
                     * @param nOfRows representing the number of tracks already existing
                     * @param nOfColumns number of detections
                     * @param assignment a vector in which all the assignments are stored
                     * @param Method the enum variable representing which method that will be used for the hungarian algorithm
                     */
                    track_t Solve(const distMatrix_t& distMatrixIn, const size_t& nOfRows, const size_t& nOfColumns, assignments_t& assignment, const TMethod& Method = optimal);
                private:
                    typedef std::vector<uint> BoolVec;
                private:
                    // --------------------------------------------------------------------------
                    // Computes the optimal assignment (minimum overall costs) using Munkres algorithm.
                    // --------------------------------------------------------------------------
                    void assignmentoptimal(assignments_t& assignment, track_t& cost, const distMatrix_t& distMatrixIn, const size_t& nOfRows, const size_t& nOfColumns);
                    void buildassignmentvector(assignments_t& assignment, BoolVec& starMatrix, const size_t& nOfRows, const size_t& nOfColumns);
                    void computeassignmentcost(const assignments_t& assignment, track_t& cost, const distMatrix_t& distMatrixIn, const size_t& nOfRows);
                    void step2a(assignments_t& assignment, distMatrix_t& distMatrix, BoolVec& starMatrix, BoolVec& newStarMatrix, BoolVec& primeMatrix, BoolVec& coveredColumns, BoolVec& coveredRows, const size_t& nOfRows, const size_t&  nOfColumns, const size_t& minDim);
                    void step2b(assignments_t& assignment, distMatrix_t& distMatrix, BoolVec& starMatrix, BoolVec& newStarMatrix, BoolVec& primeMatrix, BoolVec& coveredColumns, BoolVec& coveredRows, const size_t& nOfRows, const size_t& nOfColumns, const size_t& minDim);
                    void step3(assignments_t& assignment, distMatrix_t& distMatrix, BoolVec& starMatrix, BoolVec& newStarMatrix, BoolVec& primeMatrix, BoolVec& coveredColumns, BoolVec& coveredRows, const size_t& nOfRows, const size_t& nOfColumns, const size_t& minDim);
                    void step4(assignments_t& assignment, distMatrix_t& distMatrix, BoolVec& starMatrix, BoolVec& newStarMatrix, BoolVec& primeMatrix, BoolVec& coveredColumns, BoolVec& coveredRows, const size_t& nOfRows, const size_t& nOfColumns, const size_t& minDim, const size_t& row, const size_t& col);
                    void step5(assignments_t& assignment, distMatrix_t& distMatrix, BoolVec& starMatrix, BoolVec& newStarMatrix, BoolVec& primeMatrix, BoolVec& coveredColumns, BoolVec& coveredRows, const size_t& nOfRows, const size_t& nOfColumns, const size_t& minDim);
                    // --------------------------------------------------------------------------
                    // Computes a suboptimal solution. Good for cases with many forbidden assignments.
                    // --------------------------------------------------------------------------
                    void assignmentsuboptimal1(assignments_t& assignment, track_t& cost, const distMatrix_t& distMatrixIn, const size_t& nOfRows, const size_t& nOfColumns);
                    // --------------------------------------------------------------------------
                    // Computes a suboptimal solution. Good for cases with many forbidden assignments.
                    // --------------------------------------------------------------------------
                    void assignmentsuboptimal2(assignments_t& assignment, track_t& cost, const distMatrix_t& distMatrixIn, const size_t& nOfRows, const size_t& nOfColumns);
            };
        }
    }
}

#endif
