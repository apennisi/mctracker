/*
 * Written by Andrea Pennisi
 */

#ifndef _UTILS_H_
#define _UTILS_H_

#include <iostream>
#include <memory>

#include "detection.h"

using namespace mctracker;
using namespace mctracker::tracker;

typedef std::vector<Detection> Detections;
typedef std::vector<uint> UIntVec;
typedef std::vector<UIntVec> UIntMat;
typedef std::vector<bool> BoolVec;
typedef std::vector<cv::Point> Points;
typedef std::vector<cv::Mat> MatVec;

#endif
