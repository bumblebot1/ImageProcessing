#include <stdio.h>
#include <opencv/cv.h>        //you may need to
#include <opencv/highgui.h>   //adjust import locations
#include <opencv/cxcore.h>    //depending on your machine setup

using namespace cv;

int main() {

  // Read image from file
  Mat image = imread("mandrill3.jpg", CV_LOAD_IMAGE_UNCHANGED);
  cvtColor( image, image, CV_HSV2BGR );
  //Save thresholded image
  imwrite("swap3.jpg", image);

  return 0;
}
