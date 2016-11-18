#ifndef CONV_H
#define CONV_H

#include "opencv/cv.h"        //you may need to
#include "opencv/highgui.h"   //adjust import locations
#include "opencv/cxcore.h"    //depending on your machine setup

using namespace cv;

Mat convolution3D(Mat image, Mat mask){
  Mat grad = Mat(image.rows, image.cols, CV_32FC1);
  for(int x = 1; x < image.rows - 1; x++) {
      for(int y = 1; y < image.cols - 1; y++) {
          float pixel = 0;
          for(int i = -1; i <= 1 ; i++) {
              for(int j = -1; j<= 1; j++) {
                  pixel += image.at<uchar>(x-i, y-j) * mask.at<char>(i+1,j+1);
              }
          }
          grad.at<float>(x,y) = pixel;
      }
  }
  return grad;
}

Mat convolution5D(Mat image, Mat mask){
  Mat grad = Mat(image.rows, image.cols, CV_32FC1);
  for(int x = 2; x < image.rows - 2; x++) {
      for(int y = 2; y < image.cols - 2; y++) {
          float pixel = 0;
          for(int i = -2; i <= 2 ; i++) {
              for(int j = -2; j<= 2; j++) {
                  pixel += image.at<uchar>(x-i, y-j) * mask.at<char>(i+1,j+1);
              }
          }
          grad.at<float>(x,y) = pixel;
      }
  }
  return grad;
}
#endif
