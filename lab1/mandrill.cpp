#include <stdio.h>
#include <opencv/cv.h>        //you may need to
#include <opencv/highgui.h>   //adjust import locations
#include <opencv/cxcore.h>    //depending on your machine setup

using namespace cv;           //make available OpenCV namespace

int function(uchar pixel, double value);

int main() {

  //declare a matrix container to hold an image
  Mat image;

  //load image from a file into the container
  image = imread("mandrillRGB.jpg", CV_LOAD_IMAGE_UNCHANGED);
  //construct a window for image display
  namedWindow("Display window", CV_WINDOW_AUTOSIZE);

  for(int i=0; i<image.rows; i++){
    for(int j=0; j<image.cols; j++) {
      image.at<Vec3b>(i,j)[0] = function(image.at<Vec3b>(i,j)[2], 200);
      image.at<Vec3b>(i,j)[1] = function(image.at<Vec3b>(i,j)[2], 200);
      image.at<Vec3b>(i,j)[2] = function(image.at<Vec3b>(i,j)[2], 200);
    }
  }

  /*for(int i=0; i<image.rows; i++){
    for(int j=0; j<image.cols; j++) {
      image.at<Vec3b>(i,j)[0] = function(image.at<Vec3b>(i,j)[0], 128);
      image.at<Vec3b>(i,j)[1] = function(image.at<Vec3b>(i,j)[1], 128);
      image.at<Vec3b>(i,j)[2] = function(image.at<Vec3b>(i,j)[2], 128);
    }
  }*/

  //visualise the loaded image in the window
  imshow("Display window", image);

  //wait for a key press until returning from the program
  waitKey(0);

  //free memory occupied by image
  image.release();

  return 0;
}

int function(uchar pixel, double value) {
  if(pixel > value) {
      return 255;
  } else {
      return 0;
  }
}
