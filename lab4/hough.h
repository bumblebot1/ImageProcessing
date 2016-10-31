#ifndef HOUGH_H
#define HOUGH_H

#include <opencv/cv.h>        //you may need to
#include <opencv/highgui.h>   //adjust import locations
#include <opencv/cxcore.h>    //depending on your machine setup

using namespace cv;

Mat hough(Mat magnitude, Mat orientation, int minRadius, int maxRadius, int Th){
  int dims[] = {magnitude.rows, magnitude.cols, maxRadius-minRadius +1};
  Mat H(3, dims, CV_32FC1, Scalar::all(0));

  for(int x = 0; x < magnitude.rows; x++){
    for(int y = 0; y < magnitude.cols; y++){
      if(magnitude.at<uchar>(x,y) == 255){
        for(int k = minRadius; k < maxRadius; k++){
          int x0 = x + k*std::sin(orientation.at<float>(x,y));
          int y0 = y + k*std::cos(orientation.at<float>(x,y));
          if(x0 >= 0 && y0 >= 0 && x0 < magnitude.rows && y0 < magnitude.cols)
            H.at<float>(x0,y0,k-minRadius)++;

          x0 = x - k*std::sin(orientation.at<float>(x,y));
          y0 = y - k*std::cos(orientation.at<float>(x,y));
          if(x0 >= 0 && y0 >= 0 && x0 < magnitude.rows && y0 < magnitude.cols)
            H.at<float>(x0,y0,k-minRadius)++;
        }
      }
    }
  }


  Mat houghSpace = Mat(magnitude.rows, magnitude.cols, CV_64FC1);
  for(int x = 0; x < magnitude.rows; x++){
    for(int y = 0; y < magnitude.cols; y++){
      houghSpace.at<double>(x,y) = 0;
      for(int k = minRadius; k < maxRadius; k++){
        houghSpace.at<double>(x,y) += H.at<float>(x,y,k-minRadius);
      }
    }
  }
  return houghSpace;
}

#endif
