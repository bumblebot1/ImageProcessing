#ifndef CONV_HPP
#define CONV_HPP

using namespace cv;

Mat convolution3D(Mat image, Mat mask){
  Mat grad = Mat(image.rows, image.cols, CV_32FC1, Scalar(0));
  for(int y = 1; y < image.rows - 1; y++) {
      for(int x = 1; x < image.cols - 1; x++) {
          float pixel = 0;
          for(int j = -1; j <= 1 ; j++) {
              for(int i = -1; i<= 1; i++) {
                  pixel += image.at<uchar>(y-j, x-i) * mask.at<char>(j+1,i+1);
              }
          }
          grad.at<float>(y,x) = pixel;
      }
  }
  return grad;
}

Mat convolution5D(Mat image, Mat mask){
  Mat grad = Mat(image.rows, image.cols, CV_32FC1, Scalar(0));
  for(int y = 2; y < image.rows - 2; y++) {
      for(int x = 2; x < image.cols - 2; x++) {
          float pixel = 0;
          for(int j = -2; j <= 2 ; j++) {
              for(int i = -2; i<= 2; i++) {
                  pixel += image.at<uchar>(y-j, x-i) * mask.at<char>(j + 2, i + 2);
              }
          }
          grad.at<float>(x,y) = pixel;
      }
  }
  return grad;
}
#endif
