/**
 * @file calibrate.cpp
 * @author Samreen
 * @brief implements calibrate.hpp to find the chessboard corners and calibrate
 * the camera
 * @version 0.1
 * @date 2022-03-27
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "calibrate.hpp"

using namespace cv;
using namespace std;

int getLastCorners(Mat &src, vector<Point2f> &lastcorners) {
  // convert to greyscale
  Mat dst;
  cvtColor(src, dst, COLOR_BGR2GRAY);
  // find the chessboard corners
  if (findChessboardCorners(
          dst, Size(9, 6), lastcorners,
          CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE)) {
    // improve the accuracy of the corners found
    cornerSubPix(
        dst, lastcorners, Size(11, 11), Size(-1, -1),
        TermCriteria(TermCriteria::EPS | TermCriteria::COUNT, 30, 0.1));
    // print the improved corners
  } else {
    return -1;
  }

  return 0;
}

int getPoints(vector<Vec3f> &points) {
  // add the points to the vector
  points.clear();

  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 9; j++) {
      points.push_back(Vec3f(j, -i, 0));
    }
  }

  return 0;
}
