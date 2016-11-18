#ifndef HOUGH_H
#define HOUGH_H

#include "opencv/cv.h"        //you may need to
#include "opencv/highgui.h"   //adjust import locations
#include "opencv/cxcore.h"    //depending on your machine setup

using namespace cv;

Mat houghCircles(Mat magnitude, Mat orientation, int minRadius, int maxRadius, int Th){
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

Mat houghLines(Mat magnitude, Mat orientation, int Th){
  int dims[] = { 360, 2*(magnitude.rows + magnitude.cols)};
  Mat H(2, dims, CV_32FC1, Scalar::all(0));

  for(int x = 0; x < magnitude.rows; x++){
    for(int y = 0; y < magnitude.cols; y++){
      if(magnitude.at<uchar>(x,y) == 255){
        int angle = orientation.at<float>(x,y) * 180 / M_PI;

        for(int delta = -10; delta < 10; delta++){
            float theta = (angle + delta + 360) % 360;
            theta = theta * M_PI / 180;

            float ro = x* std::cos(theta) + y*std::sin(theta) + magnitude.rows + magnitude.cols;

              theta = theta * 180 / M_PI;
              H.at<float>((int) theta, (int) ro) += 1;

        }
      }
    }
  }

  Mat toReturn;
  cv:: Size size(H.rows, H.cols/3);
  cv::resize(H,toReturn, size);
  return toReturn;
}

#endif
