cor.csv : 4 coordinates to run perspective correction.
stationary.csv : baseline values for method 1,2,3,4.
dense.csv : baseline values for dynamic density.
extra.txt : contains information about sparse and dense optical flow.
Information about other files is in the README in the analysis folder.
All files are generated in the same directory.

List of Make commands:

make / make default : runs method 1,2,3,4 and generates plots using vid.mp4 as input assuming cor.csv and stationary.csv are generated.
make runx : one command for each method, x is the method number, runs the method and generates plots using vid.mp4 as input assuming cor.csv and stationary.csv are generated.
make baseline : generates stationary.csv.
make incor : generates cor.csv.
make dense : runs dense optical flow on vid.mp4 and generates dense.csv, assuming cor.csv generated.
make sparse : runs sparse optical flow on vid.mp4 and generates extra.txt, assuming cor.csv generated.
make extra : runs dense and sparse optical flow on vid.mp4 and generates extra.txt, assuming cor.csv generated.
make runextra FILENAME=<file> : runs dense and sparse optical flow on passed file and generates extra.txt.
make fresh : takes input to generate cor.csv and stationary.csv, runs method 1,2,3,4 and generates plots using vid.mp4 as input.
make run FILENAME=<file> : takes input to generate cor.csv and stationary.csv, runs method 1,2,3,4 and generates plots using passed file as input.
make build : generates all executables.
make plot : generates all plots.
make clean : deletes all executables.
make cleanall : deletes all executables, plots and csv files.
make help : prints this file. 

This project requires numpy and matplotlib. 
If you do not have them installed, call make install before running anything else. 
