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
#include "headers/detector.hpp"

String imagesPath = "Test Images/dart";
String groundTruthPath = "groundTruth/image";

int main( int argc, char** argv ){
  // LOADING THE IMAGE
  double totalTP(0), totalFP(0), totalFN(0), f1sum(0);
  for(int i = 0; i < 16; i++){
    stringstream testStream;
    testStream<<imagesPath<<i<<".jpg";
    String testImage = testStream.str();

    stringstream groundTruthStream;
    groundTruthStream<<groundTruthPath<<i<<".txt";
    String groundTruthImage = groundTruthStream.str();
    cout<<testImage<<" "<<groundTruthImage<<endl;

    Mat greyImage;
    greyImage = imread( testImage, 0 );

    Mat colorImage;
    colorImage = imread( testImage, 1);

    if( !greyImage.data )
    {
      printf( " No image data \n " );
      return -1;
    }

    vector<Rect> allDetections = intersectionDetector(greyImage, colorImage);
    //vector<Rect> circleDetections = scaledCircleDetector(greyImage, colorImage, 0.5);
    //imshow("sharpened", strengthenEdges(greyImage));
    cout<<"Number of detections"<<allDetections.size()<<endl;
    //allDetections.insert(allDetections.end(), circleDetections.begin(), circleDetections.end());

    //calculate tpr, fpr, fnr and f1 score for each detection
    tuple<double, double, double, double> results = verifyDetections(colorImage, allDetections, groundTruthImage);
    totalTP += (get<0>(results));
    totalFP += (get<1>(results));
    totalFN += (get<2>(results));
    f1sum   += (get<3>(results));
    stringstream detectionName;
    detectionName<<"houghDetections/detectedHough"<<i<<".jpg";
    imwrite(detectionName.str(), colorImage);
  }

  cout<<endl<<"==================FINAL RESULTS==================="<<endl;
  cout<<"Total number of true positives: " << totalTP << endl;
  cout<<"Total number of false positives: " << totalFP << endl;
  cout<<"Total number of false negatives: " << totalFN << endl;
  cout<<"Final F1 score is: " << calculateF1Score(totalTP, totalFP, totalFN)<<endl;
  return 0;
}
