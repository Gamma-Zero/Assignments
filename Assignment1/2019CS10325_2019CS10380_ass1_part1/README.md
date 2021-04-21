Execute with empty.jpg as input in current directory using make or make default.
Create executable A1 with make build.
To run with other input, either run ./A1 (filepath) or make run FILENAME=(filepath).

To generate cropped and transformed images after running the executable, use mouse left clicks to select the 4 points required by the homography. If more than 4 points are clicked, first 4 will be considered. Press ESC after selecting the points to generate required images. 

make help to print this file. 
make clean removes the executable.
make cleanall removes the executable and output images.

Created by Ananjan Nandi(2019CS10325) and Parth Gupta(2019CS10380)
