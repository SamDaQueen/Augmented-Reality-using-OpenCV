/**
 * @file vidDisplay.cpp
 * @author Samreen
 * @brief starts a video and uses filters from filter.hpp for effects on
 * different keypresses
 * @version 0.1
 * @date 2022-01-28
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

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

  // VideoWriter writer("data/video.avi", VideoWriter::fourcc('M', 'J', 'P',
  // 'G'),
  //                    15, Size(refS.width, refS.height), true);

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

    imshow("Video", frame);
  }

  delete capdev;
  // writer.release();
  destroyAllWindows();
  return (0);
}
