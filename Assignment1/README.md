PS: IMPLEMENT OPTICAL FLOW FOR MOVING DENSITY, AS AN ALTERNATIVE. WE WILL USE THE BETTER ALTERNATIVE BETWEEN ABSDIFF AND OPTICAL FLOW.

Execute with vid.mp4 as input in current directory using make or make default.
Create executable A1_density with make build.
To run with other input, either run ./A1 (filepath) or make run FILENAME=(filepath).

To generate cropped and transformed images after running the executable, use mouse left clicks to select the 4 points required by the homography. If more than 4 points are clicked, first 4 will be considered. Press ESC after selecting the points to generate required images. Graph is stored in stationary.png.

make help to print this file. 
make clean removes the executable.
make cleanall removes the executable and output images.
