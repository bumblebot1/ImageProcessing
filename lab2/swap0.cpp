#include <stdio.h>
#include <opencv/cv.h>        //you may need to
#include <opencv/highgui.h>   //adjust import locations
#include <opencv/cxcore.h>    //depending on your machine setup

using namespace cv;

int main() {

  // Read image from file
  Mat image = imread("mandrillRGB.jpg", 1);
  Mat image2 = Mat(image.rows, image.rows, DataType<uchar>::type);
  for(int y=0; y<image.rows; y++) {
      for(int x=0; x<image.rows; x++) {
          uchar pixelBlue = image.at<Vec3b>(y,x)[0];
          uchar pixelGreen = image.at<Vec3b>(y,x)[1];
          uchar pixelRed = image.at<Vec3b>(y,x)[2];
          image2.at<Vec3b>(y,x)[0] = pixelRed;
          image2.at<Vec3b>(y,x)[1] = pixelBlue;
          image2.at<Vec3b>(y,x)[2] = pixelGreen;
      }
  }
  //Save thresholded image
  imwrite("swap0.jpg", image);

  return 0;
}
