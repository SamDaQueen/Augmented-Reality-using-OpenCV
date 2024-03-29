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

bool getLastCorners(Mat &src, vector<Point2f> &lastcorners) {
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
    return false;
  }

  return true;
}

int getPoints(vector<Vec3f> &points, int z) {
  // add the points to the vector
  points.clear();

  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 9; j++) {
      points.push_back(Vec3f(j, -i, z));
    }
  }

  return 0;
}

int writeParametersToCSV(Mat &cameraMatrix, Mat &distCoeffs) {
  // write the parameters to a csv file
  char filename[100] = "./data/parameters.csv";
  char buffer[100];
  FILE *fp;

  fp = fopen(filename, "w");
  if (fp == NULL) {
    printf("Unable to open file %s\n", filename);
    exit(-1);
  }

  // write the camera matrix
  for (unsigned int i = 0; i < 9; i++) {
    sprintf(buffer, "%.4f", cameraMatrix.at<double>(i));
    if (i < 8)
      fprintf(fp, "%s,", buffer);
    else
      fprintf(fp, "%s\n", buffer);  // EOL
  }
  // write the distortion coefficients
  for (unsigned int i = 0; i < 5; i++) {
    sprintf(buffer, "%.4f", distCoeffs.at<double>(i));
    if (i < 4)
      fprintf(fp, "%s,", buffer);
    else
      fprintf(fp, "%s\n", buffer);  // EOL
  }

  fclose(fp);
  return 0;
}

/*
  Utility function for reading one float value from a CSV file

  The value is stored in the v parameter

  The function returns true if it reaches the end of a line or the file
 */
int getFloat(FILE *fp, float *v) {
  char s[256];
  int p = 0;
  int eol = 0;

  for (;;) {
    char ch = fgetc(fp);
    if (ch == ',') {
      break;
    } else if (ch == '\n' || ch == EOF) {
      eol = 1;
      break;
    }

    s[p] = ch;
    p++;
  }
  s[p] = '\0';  // terminator
  *v = atof(s);

  return (eol);  // return true if eol
}

int readParametersFromCSV(vector<float> &cameraMatrix,
                          vector<float> &distCoeffs) {
  // read the camera matrix
  FILE *fp;
  float fval;
  char filename[100] = "./data/parameters.csv";

  fp = fopen(filename, "r");

  if (fp == NULL) {
    printf("Unable to open file %s\n", filename);
    exit(-1);
  }

  // read the camera matrix
  for (unsigned int i = 0; i < 9; i++) {
    if (getFloat(fp, &fval) == -1) {
      printf("Error reading camera matrix\n");
      exit(-1);
    }
    cameraMatrix.push_back(fval);
  }
  // read the distortion coefficients
  for (unsigned int i = 0; i < 5; i++) {
    if (getFloat(fp, &fval) == -1) {
      printf("Error reading distortion coefficients\n");
      exit(-1);
    }
    distCoeffs.push_back(fval);
  }
  return 0;
}

int getHarrisCorners(Mat &src) {
  Mat dst, dst_norm, dst_norm_scaled;
  dst = Mat::zeros(src.size(), CV_32FC1);
  // convert to grayscale
  Mat gray;
  cvtColor(src, gray, COLOR_BGR2GRAY);
  // detect corners
  cornerHarris(gray, dst, 2, 3, 0.04, BORDER_DEFAULT);
  // dilate(dst, dst, Mat(), Point(-1, -1));
  normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
  convertScaleAbs(dst_norm, dst_norm_scaled);

  for (int j = 0; j < dst_norm.rows; j++) {
    for (int i = 0; i < dst_norm.cols; i++) {
      if ((int)dst_norm.at<float>(j, i) > 200) {
        circle(src, Point(i, j), 5, Scalar(0, 0, 255), 2, 8, 0);
      }
    }
  }

  // src = dst_norm_scaled;

  return 0;
}