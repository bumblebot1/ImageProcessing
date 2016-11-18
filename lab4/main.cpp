// header inclusion
#include "stdio.h"

/*opencv includes*/
#include "opencv/cv.h"        //you may need to
#include "opencv/highgui.h"   //adjust import locations
#include "opencv/cxcore.h"    //depending on your machine setup

/*helper functions*/
#include "convolution.h"
#include "utils.h"
#include "hough.h"


using namespace cv;
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

 /*initialise convolution masks*/
 char maskX[9] = { -1, 0, 1, -2, 0, 2, -1, 0, 1};
 Mat maskDx = Mat(3, 3, CV_8SC1, maskX);

 char maskY[9] = { -1, -2, -1, 0, 0, 0, 1, 2, 1};
 Mat maskDy = Mat(3, 3, CV_8SC1, maskY);


 /*calculate gradient in x direction*/
 Mat grad_x = convolution3D(image, maskDx);
 Mat normalisedGrad_x = normalise(grad_x);
 imshow("Gradient X", normalisedGrad_x);

 /*calculate gradient in y direction*/
 Mat grad_y = convolution3D(image, maskDy);
 Mat normalisedGrad_y = normalise(grad_y);
 imshow("Gradient Y", normalisedGrad_y);

 /*calculate magnitude*/
 Mat mag = magnitude(grad_x, grad_y);
 Mat normalisedMag = normalise(mag);
 imshow("Magnitude", normalisedMag);

 /*calculate gradient direction*/
 Mat dir = gradient_direction(grad_x, grad_y);
 Mat normalisedDir = normalise(dir);
 imshow("Phase", normalisedDir);

 /*threshold magnitude*/
 Mat thresholded = threshold(normalisedMag, 50);
 imshow("Thresholded",thresholded);

 /*calculate hough space transformation*/
 Mat houghSpace = hough(thresholded, dir, 20, 100, 10);
 imshow("houghSpace", normalise(houghSpace));

 waitKey(0);
 return 0;
}

/* sobel transformation using only opencv functions
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
  Mat mag = magnitude(grad_x, grad_y);
  imshow("magnitude", normalise(mag));
  Mat phase = gradient_direction(grad_x, grad_y);
  imshow("phase", normalise(phase));
  waitKey(0);
}
*/
