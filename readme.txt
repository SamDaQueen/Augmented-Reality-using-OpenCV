CS5330 PRCV Project 4 README

Khoury Wiki URL: https://wiki.khoury.northeastern.edu/x/7dp5Bg

Operating system used: Windows 11
IDE used: Visual Studio Code, using the MinGW compiler

Running the executables:
Tasks 1-3:
./bin/calibrate_camera.exe
Tasks 4-6:
./bin/create_object.exe
Task 7:
./bin/harris_corners.exe

Project Tree:
project1
	->bin: executables
	->data: images
	->build: CMake files
	->include: libraries
	->src: main.cpp


Usage:
calibrate_camera.exe
Press 's' to save the images in data folder, and save the 2D and corresponding 3D points in arrays.
Press 's' more than 5 times on valid frames(frames with chessboards) to calibrate the camera.
Press 'w' to write the camera matrix and distortion coefficients to the csv file paramaters.csv

create_object.exe
Shows the axes, the corner points, and the 3D star object on the chessboard at different angles.

harris_corners.exe
Draws the detected corners in the frame of the video and draws them as red circles over the frame.

Extensions:
Used multiple devices to check re-projection error and the quality of the projection.


Time travel days:
Requested professor and used 3 late days for this project.



