/////////////////////////////////////////////////////////////////////////////
//
// COMS30121 - face.cpp
//
/////////////////////////////////////////////////////////////////////////////

// header inclusion
// header inclusion
#include "stdio.h"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "iostream"
#include "sstream"
#include "stdio.h"
#include "headers/processDetections.hpp"

using namespace std;
using namespace cv;

/** Function Headers */
vector<Rect> detectAndDisplay( Mat frame );

/** Global variables */
String cascade_name = "dartcascade/cascade.xml";
CascadeClassifier cascade;
String imagesPath = "Test Images/dart";
String groundTruthPath = "groundTruth/image";

/** @function main */
int main( int argc, const char** argv )
{
  double totalTP(0), totalFP(0), totalFN(0), f1sum(0);
  for(int i = 0; i < 16; i++){
    stringstream testStream;
    testStream<<imagesPath<<i<<".jpg";
    String testImage = testStream.str();

    stringstream groundTruthStream;
    groundTruthStream<<groundTruthPath<<i<<".txt";
    String groundTruthImage = groundTruthStream.str();
    cout<<testImage<<" "<<groundTruthImage<<endl;

    //read image
    Mat frame = imread(testImage, CV_LOAD_IMAGE_COLOR);
    if( !cascade.load( cascade_name ) ){
      printf("--(!)Error loading\n");
      return -1;
    };

    //calculate detections and display them
    vector<Rect> detections = detectAndDisplay( frame );

    //calculate tpr, fpr, fnr and f1 score for each detection
    tuple<double, double, double, double> results = verifyDetections(frame, detections, groundTruthImage);
    totalTP += (get<0>(results));
    totalFP += (get<1>(results));
    totalFN += (get<2>(results));
    f1sum   += (get<3>(results));
    stringstream detectionName;
    detectionName<<"violaDetections/detectedViola"<<i<<".jpg";
    imwrite(detectionName.str(), frame);
  }

  //calculate overall f1 score and display sum of tp, fp, fn
  cout<<endl<<"==================FINAL RESULTS==================="<<endl;
  cout<<"Total number of true positives: " << totalTP << endl;
  cout<<"Total number of false positives: " << totalFP << endl;
  cout<<"Total number of false negatives: " << totalFN << endl;
  cout<<"Final F1 score is: " << calculateF1Score(totalTP, totalFP, totalFN)<<endl;
  return 0;
}

/** @function detectAndDisplay */
vector<Rect> detectAndDisplay( Mat frame )
{
  std::vector<Rect> faces;
  Mat frame_gray;

  // 1. Prepare Image by turning it into Grayscale and normalising lighting
  cvtColor( frame, frame_gray, CV_BGR2GRAY );
  equalizeHist( frame_gray, frame_gray );

  // 2. Perform Viola-Jones Object Detection
  cascade.detectMultiScale( frame_gray, faces, 1.1, 1, 0|CV_HAAR_SCALE_IMAGE, Size(50, 50), Size(500,500) );

       // 3. Print number of Faces found
  std::cout << faces.size() << std::endl;
  // 4. Draw box around faces found
  for( int i = 0; i < faces.size(); i++ )
  {
    rectangle(frame, Point(faces[i].x, faces[i].y), Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height), Scalar( 0, 255, 0 ), 2);
  }

  return faces;
}
