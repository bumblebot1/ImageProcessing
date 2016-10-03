#include <stdio.h>
#include <opencv/cv.h>        //you may need to
#include <opencv/highgui.h>   //adjust import locations
#include <opencv/cxcore.h>    //depending on your machine setup

using namespace cv;

int main() {

  // Read image from file
  Mat image = imread("mandrill2.jpg", 1);
  Mat image2 = Mat(image.rows, image.rows, CV_8UC3);
  //Mat image2 = Mat(image.rows, image.rows, DataType<Vec3b>::type);

  //image2 = cv::Scalar::all(255) - image;
  for(int y=0; y<image.rows; y++) {
      for(int x=0; x<image.rows; x++) {
          uchar pixelBlue = image.at<Vec3b>(y,x)[0];
          uchar pixelGreen = image.at<Vec3b>(y,x)[1];
          uchar pixelRed = image.at<Vec3b>(y,x)[2];
          image2.at<Vec3b>(y,x)[0] = 255-pixelBlue;
          image2.at<Vec3b>(y,x)[1] = 255-pixelGreen;
          image2.at<Vec3b>(y,x)[2] = 255-pixelRed;
      }
  }
  std::cout<<image2.rows;
  //Save thresholded image
  imwrite("swap2.jpg", image2);

  return 0;
}
