/**
 * @file create_object.cpp
 * @author Samreen
 * @brief uses the parameters stored in the csv file to draw 3-D object on
 * chessboard.
 * @version 0.1
 * @date 2022-03-27
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include "calibrate.hpp"

using namespace std;
using namespace cv;

bool cornersExist = false;
vector<Point2f> lastcorners;
vector<vector<Point2f>> allcorners;
vector<Vec3f> points;
vector<vector<Vec3f>> allpoints;
double RPerror;

int main(int argc, char *argv[]) {
  // read the parameters from the csv file
  vector<float> cameraMatrixVector, distCoeffs;
  Mat cameraMatrix = (Mat_<float>(3, 3) << 1, 0, 0, 0, 1, 0, 0, 0, 1);
  Mat rvecs, tvecs;

  readParametersFromCSV(cameraMatrixVector, distCoeffs);
  // print the camera matrix
  cout << "Camera Matrix: " << endl;
  for (int i = 0; i < cameraMatrixVector.size(); i++) {
    cout << cameraMatrixVector[i] << ", ";
    if ((i + 1) % 3 == 0) {
      cout << endl;
    }
    cameraMatrix.at<float>(i / 3, i % 3) = cameraMatrixVector[i];
  }
  cout << endl;
  // print the distortion coefficients
  cout << "Distortion Coefficients: " << endl;
  for (int i = 0; i < distCoeffs.size(); i++) {
    cout << distCoeffs[i] << ", ";
  }
  cout << endl;

  // get the points for the chessboard
  getPoints(points, 0);

  VideoCapture *capdev;

  // open the video device
  capdev = new VideoCapture(2);
  if (!capdev->isOpened()) {
    printf("Unable to open video device\n");
    return (-1);
  }

  // get some properties of the image
  Size refS((int)capdev->get(CAP_PROP_FRAME_WIDTH),
            (int)capdev->get(CAP_PROP_FRAME_HEIGHT));
  printf("Expected size: %d %d\n", refS.width, refS.height);

  namedWindow("Video", 1);  // identifies a window
  Mat frame;

  for (;;) {
    *capdev >> frame;  // get a new frame from the camera, treat as a stream
    if (frame.empty()) {
      printf("frame is empty\n");
      break;
    }

    // see if there is a waiting keystroke
    char key = waitKey(10);

    // end program when "q" is pressed
    if (key == 'q' || getWindowProperty("Video", WND_PROP_VISIBLE) < 1) {
      break;
    }

    // get the last corners
    cornersExist = getLastCorners(frame, lastcorners);

    if (cornersExist) {
      // get rotation and translation vectors using solvePnP
      solvePnP(points, lastcorners, cameraMatrix, distCoeffs, rvecs, tvecs,
               false, SOLVEPNP_EPNP);

      // print the rotation and translation vectors
      // for (int i = 0; i < rvecs.rows; i++) {
      //   cout << "Rotation Vector: ";
      //   cout << rvecs.at<float>(i, 0) << ", " << rvecs.at<float>(i, 1) << ",
      //   "
      //        << rvecs.at<float>(i, 2);
      //   cout << "; Translation Vector: ";
      //   cout << tvecs.at<float>(i, 0) << ", " << tvecs.at<float>(i, 1) << ",
      //   "
      //        << tvecs.at<float>(i, 2) << endl;
      // }

      // draw the projected corner points
      circle(frame, lastcorners[0], 5, Scalar(0, 0, 255), -1);
      circle(frame, lastcorners[8], 5, Scalar(0, 255, 0), -1);
      circle(frame, lastcorners[45], 5, Scalar(255, 0, 0), -1);
      circle(frame, lastcorners[53], 5, Scalar(255, 255, 0), -1);

      // draw the axes
      // vector<Point2f> axisPoints;
      // vector<Point3f> axisPoints3D = {Point3f(0, 0, 0), Point3f(1, 0, 0),
      //                                 Point3f(0, 1, 0), Point3f(0, 0, 1)};
      // projectPoints(axisPoints3D, rvecs, tvecs, cameraMatrix, distCoeffs,
      //               axisPoints);
      // line(frame, axisPoints[0], axisPoints[1], Scalar(0, 0, 255), 2);
      // putText(frame, "X", axisPoints[1], FONT_HERSHEY_SIMPLEX, 0.5,
      //         Scalar(0, 0, 255), 1, 8);
      // line(frame, axisPoints[0], axisPoints[2], Scalar(0, 255, 0), 2);
      // putText(frame, "Y", axisPoints[2], FONT_HERSHEY_SIMPLEX, 0.5,
      //         Scalar(0, 255, 0), 1, 8);
      // line(frame, axisPoints[0], axisPoints[3], Scalar(255, 0, 0), 2);
      // putText(frame, "Z", axisPoints[3], FONT_HERSHEY_SIMPLEX, 0.5,
      //         Scalar(255, 0, 0), 1, 8);

      // draw a star on the chessboard

      vector<Point2f> starPoints;
      vector<Point3f> starPoints3D = {
          Point3f(4, -1, 0), Point3f(4, -1, 2), Point3f(2, -2, 0),
          Point3f(2, -2, 2), Point3f(6, -2, 0), Point3f(6, -2, 2),
          Point3f(2, -4, 0), Point3f(2, -4, 2), Point3f(6, -4, 0),
          Point3f(6, -4, 2), Point3f(4, -5, 0), Point3f(4, -5, 2)};
      // project the points onto the image
      projectPoints(starPoints3D, rvecs, tvecs, cameraMatrix, distCoeffs,
                    starPoints);
      // draw the projected starPoints3D
      line(frame, starPoints[0], starPoints[1], Scalar(0, 0, 255), 2);
      line(frame, starPoints[2], starPoints[3], Scalar(0, 255, 0), 2);
      line(frame, starPoints[4], starPoints[5], Scalar(255, 0, 0), 2);
      line(frame, starPoints[6], starPoints[7], Scalar(255, 255, 0), 2);
      line(frame, starPoints[8], starPoints[9], Scalar(0, 0, 255), 2);
      line(frame, starPoints[10], starPoints[11], Scalar(0, 255, 0), 2);
      line(frame, starPoints[0], starPoints[8], Scalar(255, 0, 0), 2);
      line(frame, starPoints[1], starPoints[9], Scalar(255, 255, 0), 2);
      line(frame, starPoints[0], starPoints[6], Scalar(0, 0, 255), 2);
      line(frame, starPoints[1], starPoints[7], Scalar(0, 255, 0), 2);
      line(frame, starPoints[6], starPoints[8], Scalar(255, 0, 0), 2);
      line(frame, starPoints[7], starPoints[9], Scalar(255, 255, 0), 2);
      line(frame, starPoints[2], starPoints[4], Scalar(0, 0, 255), 2);
      line(frame, starPoints[3], starPoints[5], Scalar(0, 255, 0), 2);
      line(frame, starPoints[2], starPoints[10], Scalar(255, 0, 0), 2);
      line(frame, starPoints[3], starPoints[11], Scalar(255, 255, 0), 2);
      line(frame, starPoints[4], starPoints[10], Scalar(0, 0, 255), 2);
      line(frame, starPoints[5], starPoints[11], Scalar(0, 255, 0), 2);
    }
    imshow("Video", frame);
  }

  delete capdev;
  return (0);
}
