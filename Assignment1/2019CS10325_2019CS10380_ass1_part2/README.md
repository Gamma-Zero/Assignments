Execute with vid.mp4 as input in current directory using make or make default.
Create executable A1_density with make build.
To run with other input, either run ./A1_density (filepath) or make run FILENAME=(filepath).

To do angle correction, use mouse left clicks to select the 4 points required by the homography. 
If more than 4 points are selected, homography is calculated using first 4 points.
Press ESC after selecting the points to generate required images. 
Graph is stored in stationary.png and can be independently generated using python3 plotter.py.
It is generated using moving.csv and stationary.csv.
out.txt contains output obtained on our computer.

make help to print this file. 
make clean removes the executable.
make cleanall removes the executable and output files.

This project requires numpy and matplotlib. 
If you do not have them installed, call make install before running anything else. 
