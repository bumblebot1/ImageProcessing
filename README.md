To compile and run detector you will need C++ 11.
###To compile the detector run the command: make dartboard
###To compile the viola jones detector run the command: make face

To run our detector run: ./dartboard "Input_Image" <br />
If you also want to verify the detection correctness vs the ground truth run: ./dartboard "Input_Image" "Ground_Truth_TextFile" <br />
If you want to run the detector from task 3, modify the header included in dartboard.cpp from headers/part4.hpp to headers/part3.hpp. <br />
