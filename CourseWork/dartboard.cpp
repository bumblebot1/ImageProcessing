/*Opencv headers*/
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"

/*System headers*/
#include "iostream"
#include "stdio.h"

/*Other headers*/
#include "headers/processDetections.hpp"
#include "headers/part4.hpp"

int main( int argc, char** argv ){
  // LOADING THE IMAGE
  char* imageName = argv[1];

  Mat greyImage;
  greyImage = imread( imageName, 0 );

  Mat colorImage;
  colorImage = imread( imageName, 1);

  if( !greyImage.data )
  {
    printf( " No image data \n " );
    return -1;
  }

  vector<Rect> allDetections = detectAndReturnBoxes(greyImage, colorImage);
  cout<<"Number of detections"<<allDetections.size()<<endl;


  if(argc > 2){
    //draw ground truth and Hough Circle detections(tp+fp) altogether on original image
    verifyDetections(colorImage, allDetections, argv[2]);
  } else {
    //draw only Hough Circle detections on original image
    displayHCircleBoundingBoxes(colorImage, allDetections);
  }
  imwrite("detected.jpg", colorImage);

  waitKey(0);
  return 0;
}
