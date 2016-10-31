#ifndef UTILS_H
#define UTILS_H

#include <opencv/cv.h>        //you may need to
#include <opencv/highgui.h>   //adjust import locations
#include <opencv/cxcore.h>    //depending on your machine setup

using namespace cv;

Mat normalise(Mat src){
  Mat dst;
  normalize(src, dst, 0.0, 255.0, cv::NORM_MINMAX, CV_8U);
  return dst;
}

Mat threshold(Mat original, uchar value){
 Mat result = Mat(original.rows, original.cols, CV_8UC1);
 for(int x = 0; x < original.rows; x++){
   for(int y = 0; y < original.cols; y++){
     if(original.at<uchar>(x,y) > value)
        result.at<uchar>(x,y) = 255;
     else
        result.at<uchar>(x,y) = 0;
   }
 }
 return result;
}

Mat magnitude(Mat grad_x, Mat grad_y){
  Mat result = Mat(grad_x.rows, grad_x.cols, CV_32FC1);
  for(int x = 0; x < grad_x.rows; x++){
    for(int y = 0; y < grad_y.cols; y++){
      float i = grad_x.at<float>(x,y);
      float j = grad_y.at<float>(x,y);
      result.at<float>(x,y) = std::floor(std::sqrt(i*i + j*j));
    }
  }
  return result;
}

Mat gradient_direction(Mat grad_x, Mat grad_y) {
  Mat result = Mat(grad_x.rows, grad_x.cols, CV_32FC1);
  for(int x = 0; x < grad_x.rows; x++){
    for(int y = 0; y < grad_y.cols; y++){
      float i = grad_x.at<float>(x,y);
      float j = grad_y.at<float>(x,y);
      result.at<float>(x,y) = std::atan2(j,i);
     }
   }
   return result;
 }

#endif
