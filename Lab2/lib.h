#pragma once
#ifndef LIB_H
#define LIB_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp> 
#include <opencv2/core/core.hpp>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <cmath>

using namespace cv;
using namespace std;

bool compare(const char* a, string b);
float char2float(char* argv[], int n);

#endif