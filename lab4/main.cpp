// header inclusion
#include <stdio.h>
#include <cmath>
#include <opencv/cv.h>        //you may need to
#include <opencv/highgui.h>   //adjust import locations
#include <opencv/cxcore.h>    //depending on your machine setup


using namespace cv;
Mat convolution(Mat original, Mat mask);
void sobelCV(Mat image);
Mat magnitude(Mat grad_x, Mat grad_y);
Mat normalise(Mat src);
Mat gradient_direction(Mat grad_x, Mat grad_y);
Mat threshold(Mat original);

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
 Mat grad_x = convolution(image, maskDx);
 imshow("Gradient X", normalise(grad_x));
 Mat grad_y = convolution(image, maskDy);
 imshow("Gradient Y", normalise(grad_y));

 Mat mag = magnitude(grad_x, grad_y);
 imshow("Magnitude", normalise(mag));

 Mat dir = gradient_direction(grad_x, grad_y);
 imshow("Phase", normalise(dir));

 imshow("thresholded", threshold(normalise(magnitude(grad_x,grad_y))));
 waitKey(0);
 return 0;
}

Mat convolution(Mat image, Mat mask){
  Mat grad = Mat(image.rows, image.cols, CV_32FC1);
  for(int x = 1; x < image.rows - 1; x++) {
      for(int y = 1; y < image.cols - 1; y++) {
          float pixel = 0;
          for(int i = -1; i <= 1 ; i++) {
              for(int j = -1; j<= 1; j++) {
                  pixel += image.at<uchar>(x-i, y-j) * mask.at<char>(i+1,j+1);
              }
          }
          grad.at<float>(x,y) = pixel;
      }
  }
  return grad;
}

Mat normalise(Mat src){
  Mat dst;
  normalize(src, dst, 0.0, 255.0, cv::NORM_MINMAX, CV_8U);
  return dst;
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

Mat threshold(Mat original){
   Mat result = Mat(original.rows, original.cols, CV_8UC1);
   for(int x = 0; x < original.rows; x++){
     for(int y = 0; y < original.cols; y++){
       if(original.at<uchar>(x,y) > 128)
          result.at<uchar>(x,y) = 255;
       else
          result.at<uchar>(x,y) = 0;
     }
   }
   return result;
 }
