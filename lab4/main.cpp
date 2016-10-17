// header inclusion
#include <stdio.h>
#include <opencv/cv.h>        //you may need to
#include <opencv/highgui.h>   //adjust import locations
#include <opencv/cxcore.h>    //depending on your machine setup


using namespace cv;
Mat convolution(Mat original, Mat mask);
void sobelCV(Mat image);

int main( int argc, char** argv )
{

 // LOADING THE IMAGE
 char* imageName = argv[1];

 Mat image;
 image = imread( imageName, 0 );

 if( argc != 2 || !image.data )
 {
   printf( " No image data \n " );
   return -1;
 }

 Mat maskDx = Mat(3, 3, CV_8SC1);
 maskDx.at<char>(0,0) = -1;
 maskDx.at<char>(0,1) =  0;
 maskDx.at<char>(0,2) =  1;
 maskDx.at<char>(1,0) = -2;
 maskDx.at<char>(1,1) =  0;
 maskDx.at<char>(1,2) =  2;
 maskDx.at<char>(2,0) = -1;
 maskDx.at<char>(2,1) =  0;
 maskDx.at<char>(2,2) =  1;

 Mat maskDy = Mat(3, 3, CV_8SC1);
 maskDy.at<char>(0,0) = -1;
 maskDy.at<char>(0,1) = -2;
 maskDy.at<char>(0,2) = -1;
 maskDy.at<char>(1,0) =  0;
 maskDy.at<char>(1,1) =  0;
 maskDy.at<char>(1,2) =  0;
 maskDy.at<char>(2,0) =  1;
 maskDy.at<char>(2,1) =  2;
 maskDy.at<char>(2,2) =  1;

 //sobelCV(image);
// imshow("Gradient X", convolution(image,maskDx));
 imshow("Gradient Y", convolution(image, maskDy));
 waitKey(0);
 return 0;
}

Mat convolution(Mat image, Mat mask){
  Mat grad = Mat(image.rows, image.cols, CV_32SC1);
  Mat result = Mat(image.rows, image.cols, CV_8UC1);
  for(int x = 1; x < image.rows - 1; x++) {
      for(int y = 1; y < image.cols - 1; y++) {
          int pixel = 0;
          for(int i = -1; i <= 1 ; i++) {
              for(int j = -1; j<= 1; j++) {
                  pixel += image.at<uchar>(x-i, y-j) * mask.at<char>(i+1,j+1);
              }
          }
          grad.at<int>(x,y) = pixel;
      }
  }
  double minVal, maxVal;
  cv::minMaxLoc(grad, &minVal, &maxVal);
  for(int x = 1; x < image.rows - 1; x++) {
      for(int y = 1; y < image.cols - 1; y++) {
        result.at<char>(x,y) = (grad.at<int>(x,y) - minVal) *255 /(maxVal-minVal);
      }
  }
  return result;
}

void sobelCV(Mat image){
  Mat grad_x;
  cv::Sobel(image, grad_x, CV_32F, 1, 0, 3);

  Mat grad_y;
  cv::Sobel(image, grad_y, CV_32F, 0, 1, 3);

  Mat draw_x;
  Mat draw_y;
  double minVal, maxVal;
  cv::minMaxLoc(grad_x, &minVal, &maxVal);
  grad_x.convertTo(draw_x, CV_8U, 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));
  cv::minMaxLoc(grad_y, &minVal, &maxVal);
  grad_y.convertTo(draw_y, CV_8U, 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));
  imshow("Original", image);
  imshow("Gradient X", draw_x);
  imshow("Gradient Y", draw_y);
  waitKey(0);
}
