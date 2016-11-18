#ifndef UTILS_H
#define UTILS_H

#include "opencv/cv.h"        //you may need to
#include "opencv/highgui.h"   //adjust import locations
#include "opencv/cxcore.h"    //depending on your machine setup

using namespace cv;

Mat normalise(Mat src){
  Mat dst;
  normalize(src, dst, 0.0, 255.0, cv::NORM_MINMAX, CV_8U);
  return dst;
}

Mat threshold(Mat original, uchar value){
 Mat result = Mat(original.rows, original.cols, CV_8UC1);
 for(int y = 0; y < original.rows; y++){
   for(int x = 0; x < original.cols; x++){
     if(original.at<uchar>(y,x) > value)
        result.at<uchar>(y,x) = 255;
     else
        result.at<uchar>(y,x) = 0;
   }
 }
 return result;
}

Mat magnitude(Mat grad_x, Mat grad_y){
  Mat result = Mat(grad_x.rows, grad_x.cols, CV_32FC1);
  for(int y = 0; y < grad_x.rows; y++){
    for(int x = 0; x < grad_x.cols; x++){
      float i = grad_x.at<float>(y,x);
      float j = grad_y.at<float>(y,x);
      result.at<float>(y,x) = std::floor(std::sqrt(i*i + j*j));
    }
  }
  return result;
}

Mat gradient_direction(Mat grad_x, Mat grad_y) {
  Mat result = Mat(grad_x.rows, grad_x.cols, CV_32FC1);
  for(int y = 0; y < grad_x.rows; y++){
    for(int x = 0; x < grad_x.cols; x++){
      float a = grad_x.at<float>(y,x);
      float b = grad_y.at<float>(y,x);
      result.at<float>(y,x) = std::atan2(b,a);
     }
   }
   return result;
 }

#endif
