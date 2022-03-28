/**
 * @file calibrate_camera.cpp
 * @author Samreen
 * @brief starts the video stream and calibrates the camera and saves the
 * parameters in a csv file.
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
  VideoCapture *capdev;

  // open the video device
  capdev = new VideoCapture(1);
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

  // get the points for the chessboard
  getPoints(points, 0);

  Mat cameraMatrix = (Mat_<float>(3, 3) << 1, 0, frame.cols / 2, 0, 1,
                      frame.rows / 2, 0, 0, 1);
  Mat distCoeffs, rvecs, tvecs;

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

    if (key == 's' && cornersExist) {
      // add corner to list of corners
      allcorners.push_back(vector<Point2f>(lastcorners));
      // print number of frames saved
      printf("Saving frame %d\n", allcorners.size());
      drawChessboardCorners(frame, Size(9, 6), lastcorners, true);
      string path = "data/image_" + to_string(allcorners.size()) + ".jpg ";
      imwrite(path, frame);

      // get the points
      allpoints.push_back(vector<Vec3f>(points));

      // callibrate the camera if at least 5 calibration frames exist
      if (allcorners.size() > 5) {
        cout << "Calibrating the camera..." << endl;

        RPerror = calibrateCamera(allpoints, allcorners, refS, cameraMatrix,
                                  distCoeffs, rvecs, tvecs, 0);
        // cout << "Camera matrix: " << cameraMatrix << endl;
        // cout << "Distortion coefficients: " << distCoeffs << endl;
        cout << "Reprojection error: " << RPerror << endl;
        // cout << "Rvecs: " << rvecs << endl;
        // cout << "Tvecs: " << tvecs << endl;
      }
    } else if (key == 'w' && allcorners.size() > 5) {
      // write the parameters to a csv file
      cout << "Writing parameters to csv file..." << endl;
      writeParametersToCSV(cameraMatrix, distCoeffs, rvecs, tvecs);
    }

    if (cornersExist) {
      drawChessboardCorners(frame, Size(9, 6), lastcorners, true);
    }

    imshow("Video", frame);
  }

  delete capdev;
  return (0);
}
