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

String debugLocation = "debugOutput/";
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

  vector<Rect> allDetections;
  vector<float> scalingFactors = {1};//, 0.5};
  cout<<scalingFactors.size()<<endl;

  int iteration = 0;
  for(auto factor : scalingFactors){
    Mat testImage = scaleHeight(image, factor);
    Mat testOriginal = scaleHeight(original, factor);

    /*calculate gradient in x direction*/
    Mat grad_x = convolution3D(testImage, maskDx);
    Mat normalisedGrad_x = normalise(grad_x);
    imwrite(debugLocation + "Gradient X" + to_string(iteration) + ".jpg", normalisedGrad_x);

    /*calculate gradient in y direction*/
    Mat grad_y = convolution3D(testImage, maskDy);
    Mat normalisedGrad_y = normalise(grad_y);
    imwrite(debugLocation + "Gradient Y" + to_string(iteration) + ".jpg", normalisedGrad_y);

    /*calculate magnitude*/
    Mat mag = magnitude(grad_x, grad_y);
    Mat normalisedMag = normalise(mag);
    imwrite(debugLocation + "Magnitude" + to_string(iteration) + ".jpg", normalisedMag);

    /*calculate gradient direction*/
    Mat dir = gradient_direction(grad_x, grad_y);
    Mat normalisedDir = normalise(dir);
    imwrite(debugLocation + "Phase" + to_string(iteration) + ".jpg", normalisedDir);

    /*threshold magnitude*/
    Mat thresholded = threshold(normalisedMag, 40);
    imwrite(debugLocation + "Thresholded" + to_string(iteration) + ".jpg",thresholded);




    /*calculate hough space circle transformation*/
    Mat hCircles = generateHoughSpace(thresholded, dir, 0, 50, (90 * M_PI) / 180);

    Mat houghSpaceCircles = visualiseHoughSpace(hCircles, 0, 50);
    imwrite(debugLocation + "houghCircles" + to_string(iteration) + ".jpg", normalise(houghSpaceCircles));

    vector<Point3i> hCircledetections = detectLinesIntersection(normalise(houghSpaceCircles), hCircles, 165, testOriginal, dir, thresholded);

    for(int i = 0; i < hCircledetections.size(); i++){
      if(factor < 1){
        cout<<"before rescalling"<<hCircledetections[i]<<endl;
        hCircledetections[i].y = (int) (hCircledetections[i].y / factor);
        int yRadius = (int) (hCircledetections[i].z / factor);
        int xRadius = (int) (hCircledetections[i].z);
        Point tl = Point(hCircledetections[i].x - xRadius, hCircledetections[i].y - yRadius);
        Point br = Point(hCircledetections[i].x + xRadius, hCircledetections[i].y + yRadius);
        allDetections.push_back(Rect(tl,br));
      }
      else{
        Point tl = Point(hCircledetections[i].x - hCircledetections[i].z, hCircledetections[i].y - hCircledetections[i].z);
        Point br = Point(hCircledetections[i].x + hCircledetections[i].z, hCircledetections[i].y + hCircledetections[i].z);
        allDetections.push_back(Rect(tl, br));
      }
    }
    imwrite(debugLocation + "scaledOriginal" + to_string(iteration) + ".jpg", testOriginal);
    iteration++;

  }
  for(int i = 0; i < allDetections.size(); i++){
    cout<<allDetections[i]<<endl;
  }

  if(argc > 2){
    //draw ground truth and Hough Circle detections(tp+fp) altogether on original image
    verifyDetections(original, allDetections, argv[2]);
  } else {
    //draw only Hough Circle detections on original image
    displayHCircleBoundingBoxes(original, allDetections);
  }

  imshow("detections", original);
  waitKey(0);
  return 0;
}
