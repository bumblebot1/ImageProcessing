#ifndef HOUGH_HPP
#define HOUGH_HPP

using namespace cv;
using namespace std;

Mat generateHoughSpace(Mat magnitude, Mat orientation, int minRadius, int maxRadius, float rotationAngle){
  int dims[] = {magnitude.rows, magnitude.cols, maxRadius-minRadius};
  Mat H(3, dims, CV_32FC1, Scalar::all(0));

  for(int y = 0; y < magnitude.rows; y++){
    for(int x = 0; x < magnitude.cols; x++){
      if(magnitude.at<uchar>(y,x) == 255){
        for(int k = minRadius; k < maxRadius; k++){
          int y0 = y + k*std::sin( orientation.at<float>(y,x) + rotationAngle );
          int x0 = x + k*std::cos( orientation.at<float>(y,x) + rotationAngle );
          if(x0 >= 0 && y0 >= 0 && x0 < magnitude.cols && y0 < magnitude.rows){
            H.at<float>(y0,x0,k-minRadius)++;
          }

          y0 = y - k*std::sin( orientation.at<float>(y,x) + rotationAngle );
          x0 = x - k*std::cos( orientation.at<float>(y,x) + rotationAngle );
          if(x0 >= 0 && y0 >= 0 && x0 < magnitude.cols && y0 < magnitude.rows){
            H.at<float>(y0,x0,k-minRadius)++;
          }
        }
      }
    }
  }
  return H;
}

Mat visualiseHoughSpace(Mat H, int minRadius, int maxRadius){
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

#endif
