/**
 * @file calibrate.hpp
 * @author Samreen
 * @brief find the chessboard corners and calibrate the camera
 * @version 0.1
 * @date 2022-03-27
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <cstdio>
#include <cstring>
#include <iostream>
#include <map>
#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

int getLastCorners(Mat &src, vector<Point2f> &lastcorners);

int getPoints(vector<Vec3f> &points);