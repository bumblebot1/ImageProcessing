/*Opencv headers*/
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

/*System headers*/
#include "iostream"
#include "stdio.h"

/*Other headers*/
#include "headers/hough.h"
#include "headers/convolution.h"
#include "headers/utils.h"
#include "headers/detect.h"
#include "headers/processDetections.h"

int main( int argc, char** argv ){
  // LOADING THE IMAGE
  char* imageName = argv[1];

  Mat image;
  image = imread( imageName, 0 );
  Mat original;
  original = imread( imageName, 1);

  if( argc < 2 || !image.data )
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
  Mat thresholded = threshold(normalisedMag, 40);
  imshow("Thresholded",thresholded);




  /*calculate hough space circle transformation*/
  Mat hCircles = houghCircles(thresholded, dir, 20, 200);

  Mat houghSpaceCircles = visualiseHoughCircles(hCircles, 20, 200);
  imshow("houghCircles", normalise(houghSpaceCircles));




  /*calculate hough space line transformation in a circle-wise way*/
  Mat hLines2 = houghLines2(thresholded, dir, 0, 60);

  Mat houghSpaceLines2 = visualiseHoughLines2(hLines2, 0, 60);
  imshow("houghLines2", normalise(houghSpaceLines2));




  vector<Rect> hCircledetections = detectCircles(normalise(houghSpaceCircles), hCircles, 220, original);

  if(argc > 2){
  	//draw ground truth and Hough Circle detections(tp+fp) altogether on original image
    verifyDetections(original, hCircledetections, argv[2]);
  } else {
  	//draw only Hough Circle detections on original image
  	displayHCircleDetections(original, hCircledetections);
  }
  imshow("HCircleDetections", original);




  Mat houghSpaceLines = visualiseHoughLines(thresholded, dir);
  imshow("houghLines", normalise(houghSpaceLines));

  detectLines(houghSpaceLines, 55, original);
  imshow("HLineDetections", original);

  waitKey(0);
  return 0;
}
