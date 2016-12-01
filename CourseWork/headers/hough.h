#ifndef HOUGH_H
#define HOUGH_H

#include "opencv/cv.h"        //you may need to
#include "opencv/highgui.h"   //adjust import locations
#include "opencv/cxcore.h"    //depending on your machine setup

using namespace cv;
using namespace std;

Mat houghCircles(Mat magnitude, Mat orientation, int minRadius, int maxRadius){
  int dims[] = {magnitude.rows, magnitude.cols, maxRadius-minRadius};
  Mat H(3, dims, CV_32FC1, Scalar::all(0));

  for(int y = 0; y < magnitude.rows; y++){
    for(int x = 0; x < magnitude.cols; x++){
      if(magnitude.at<uchar>(y,x) == 255){
        for(int k = minRadius; k < maxRadius; k++){
          int y0 = y + k*std::sin(orientation.at<float>(y,x));
          int x0 = x + k*std::cos(orientation.at<float>(y,x));
          if(x0 >= 0 && y0 >= 0 && x0 < magnitude.cols && y0 < magnitude.rows)
            H.at<float>(y0,x0,k-minRadius)++;

          y0 = y - k*std::sin(orientation.at<float>(y,x));
          x0 = x - k*std::cos(orientation.at<float>(y,x));
          if(x0 >= 0 && y0 >= 0 && x0 < magnitude.cols && y0 < magnitude.rows)
            H.at<float>(y0,x0,k-minRadius)++;
        }
      }
    }
  }
  return H;
}

Mat visualiseHoughCircles(Mat H, int minRadius, int maxRadius){
  Mat houghSpace = Mat(H.size[0], H.size[1], CV_64FC1, Scalar::all(0));
  for(int y = 0; y < houghSpace.rows; y++){
    for(int x = 0; x < houghSpace.cols; x++){

      for(int k = minRadius; k < maxRadius; k++){
        houghSpace.at<double>(y,x) += H.at<float>(y,x,k-minRadius);
      }
    }
  }
  return houghSpace;
}

Mat visualiseHoughLines(Mat magnitude, Mat orientation){
  int roSize = (int)std::sqrt(magnitude.rows * magnitude.rows + magnitude.cols * magnitude.cols);
  int dims[] = {360, roSize};
  Mat H(2, dims, CV_32FC1, Scalar::all(0));

  //cout<<"Ro size = "<<roSize<<endl;
  //cout<<"H.rows = "<<H.rows<<endl;
  //cout<<"H.cols = "<<H.cols<<endl;
  
  for(int y = 0; y < magnitude.rows; y++){
    for(int x = 0; x < magnitude.cols; x++){
      if(magnitude.at<uchar>(y,x) == 255){

        //convert radians (gradient direction value) to degrees (angle) 
        //so we can calculate all theta values in the delta interval
        int angle = orientation.at<float>(y,x) * 180 / M_PI;

        for(int delta = -10; delta < 10; delta++){
            float theta = (angle + delta + 360) % 360;

            //convert back to radians to calculate ro
            theta = theta * M_PI / 180;

            float ro = x*std::cos(theta) + y*std::sin(theta);

            //convert again radians to degrees to store integer values of theta in the 2D Hough Space
            theta = theta * 180 / M_PI;
              H.at<float>((int) theta, (int) ro)++;
        }
      }
    }
  }

  Mat toReturn;
  cv:: Size size(H.rows, H.cols);
  cv::resize(H,toReturn, size);
  //cout<<"return.rows = "<<toReturn.rows<<endl;
  //cout<<"return.cols = "<<toReturn.cols<<endl;

  return toReturn;
  //return H;
}

#endif
