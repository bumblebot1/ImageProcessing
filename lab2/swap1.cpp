#include <stdio.h>
#include <opencv/cv.h>        //you may need to
#include <opencv/highgui.h>   //adjust import locations
#include <opencv/cxcore.h>    //depending on your machine setup

using namespace cv;

int main() {

  // Read image from file
  Mat image = imread("mandrill1.jpg", 1);
  Mat image2 = Mat(image.rows, image.rows, CV_8UC3);
  for(int y=0; y<image.rows; y++) {
      for(int x=0; x<image.rows; x++) {
          uchar pixelBlue = image.at<Vec3b>(y,x)[0];
          uchar pixelGreen = image.at<Vec3b>(y,x)[1];
          uchar pixelRed = image.at<Vec3b>((y-32)%255,(x-32)%255)[2];
          image2.at<Vec3b>(y,x)[0] = pixelBlue;
          image2.at<Vec3b>(y,x)[1] = pixelGreen;
          image2.at<Vec3b>(y,x)[2] = pixelRed;
      }
  }
  //Save thresholded image
  imwrite("swap1.jpg", image2);

  return 0;
}
