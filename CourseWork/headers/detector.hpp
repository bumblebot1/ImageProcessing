#ifndef DETECTOR_HPP
#define DETECTOR_HPP

#include "hough.hpp"
#include "convolution.hpp"
#include "utils.hpp"
#include "detect.hpp"

using namespace cv;
using namespace std;

String debugLocation = "debugOutput/";
/*initialise convolution masks*/
char maskX[9] = { -1, 0, 1, -2, 0, 2, -1, 0, 1};
Mat maskDx = Mat(3, 3, CV_8SC1, maskX);

char maskY[9] = { -1, -2, -1, 0, 0, 0, 1, 2, 1};
Mat maskDy = Mat(3, 3, CV_8SC1, maskY);

vector<Rect> intersectionDetector(Mat greyImage, Mat colorImage){
  //vector<Rect> boxes;
  /*calculate gradient in x direction*/
  Mat grad_x = convolution3D(greyImage, maskDx);
  Mat normalisedGrad_x = normalise(grad_x);
  imwrite(debugLocation + "Gradient X_Intersections.jpg", normalisedGrad_x);

  /*calculate gradient in y direction*/
  Mat grad_y = convolution3D(greyImage, maskDy);
  Mat normalisedGrad_y = normalise(grad_y);
  imwrite(debugLocation + "Gradient Y_Intersections.jpg", normalisedGrad_y);

  /*calculate magnitude*/
  Mat mag = magnitude(grad_x, grad_y);
  Mat normalisedMag = normalise(mag);
  imwrite(debugLocation + "Magnitude_Intersections.jpg", normalisedMag);

  /*calculate gradient direction*/
  Mat dir = gradient_direction(grad_x, grad_y);
  Mat normalisedDir = normalise(dir);
  imwrite(debugLocation + "Phase_Intersections.jpg", normalisedDir);

  /*threshold magnitude*/
  Mat thresholded = threshold(normalisedMag, 40);
  imwrite(debugLocation + "Thresholded_Intersections.jpg",thresholded);

  /*calculate hough space circle transformation*/
  Mat hSpace = generateHoughSpace(thresholded, dir, 0, 50, (90 * M_PI) / 180);
  Mat houghSpace = visualiseHoughSpace(hSpace, 0, 50);
  imwrite(debugLocation + "HoughSpace_Intersections.jpg", normalise(houghSpace));

  vector<Point3i> detections = detectLinesIntersection(normalise(houghSpace), hSpace, 165, colorImage, dir, thresholded);

  imwrite(debugLocation + "intersectionOriginal.jpg", colorImage);

  vector<Rect> boxes = convertToBoxes(detections);

  /*free memory from Matrices*/
  grad_x.release();
  grad_y.release();
  normalisedGrad_x.release();
  normalisedGrad_y.release();
  mag.release();
  normalisedMag.release();
  dir.release();
  normalisedDir.release();
  thresholded.release();
  hSpace.release();
  houghSpace.release();
  /***************************/

  return boxes;
}

vector<Rect> scaledCircleDetector(Mat greyImage, Mat colorImage, int circleThreshold, float factor){
  Mat testGrey = scaleHeight(greyImage, factor);
  Mat testColor = scaleHeight(colorImage, factor);
  Mat sharper;
  vector<Rect> boxes;

  /*calculate gradient in x direction*/
  Mat grad_x = convolution3D(testGrey, maskDx);
  Mat normalisedGrad_x = normalise(grad_x);
  imwrite(debugLocation + "Gradient X_Circle.jpg", normalisedGrad_x);

  /*calculate gradient in y direction*/
  Mat grad_y = convolution3D(testGrey, maskDy);
  Mat normalisedGrad_y = normalise(grad_y);
  imwrite(debugLocation + "Gradient Y_Circle.jpg", normalisedGrad_y);

  /*calculate magnitude*/
  Mat mag = magnitude(grad_x, grad_y);
  Mat normalisedMag = normalise(mag);
  imwrite(debugLocation + "Magnitude_Circle.jpg", normalisedMag);

  /*calculate gradient direction*/
  Mat dir = gradient_direction(grad_x, grad_y);
  Mat normalisedDir = normalise(dir);
  imwrite(debugLocation + "Phase_Circle.jpg", normalisedDir);

  /*threshold magnitude*/
  Mat thresholded = threshold(normalisedMag, 40);
  imwrite(debugLocation + "Thresholded_Circle.jpg",thresholded);

  /*calculate hough space circle transformation*/
  Mat hCircles = generateHoughSpace(thresholded, dir, 20, 200, 0);

  Mat houghSpaceCircles = visualiseHoughSpace(hCircles, 20, 200);
  imwrite(debugLocation + "HoughSpace_Circle.jpg", normalise(houghSpaceCircles));

  vector<Point3i> hCircledetections = detectCircles(normalise(houghSpaceCircles), hCircles, circleThreshold, testColor);

  for(int i = 0; i < hCircledetections.size(); i++){
    hCircledetections[i].y = (int) (hCircledetections[i].y / factor);
    //hCircledetections[i].z = (int) (hCircledetections[i].z / factor);
    int yRadius = (int) (hCircledetections[i].z / factor);
    int xRadius = (int) (hCircledetections[i].z);
    Point tl = Point(hCircledetections[i].x - xRadius, hCircledetections[i].y - yRadius);
    Point br = Point(hCircledetections[i].x + xRadius, hCircledetections[i].y + yRadius);
    boxes.push_back(Rect(tl,br));
  }

  imwrite(debugLocation + "ScaledOriginal_Circle.jpg", testColor);

  /*free memory from Matrices*/
  testColor.release();
  testGrey.release();
  grad_x.release();
  grad_y.release();
  normalisedGrad_x.release();
  normalisedGrad_y.release();
  mag.release();
  normalisedMag.release();
  dir.release();
  normalisedDir.release();
  thresholded.release();
  hCircles.release();
  houghSpaceCircles.release();
  /***************************/

  return boxes;
}

#endif
