#ifndef DETECTIONS_H
#define DETECTIONS_H

#include "iostream"
#include "vector"
#include "opencv2/opencv.hpp"
#include "headers/accuracyMeasurement.h"

using namespace std;
using namespace cv;

/*function headers*/
void verifyDetections(Mat frame, vector<Rect> detectionOutput, String groundTruthImage);
void drawGroundTruth(Mat frame, vector<Rectangle> groundTruth);
tuple<double, double, double> calculateRates(Mat frame, vector<Rectangle> groundTruth, vector<Rect> detectionOutput);
double calculateF1Score(double tp, double fp, double fn);


void verifyDetections(Mat frame, vector<Rect> detectionOutput, String groundTruthImage){
  //read GroundTruth textFile and draw boxes for it
  vector<Rectangle> groundTruth = readFile(groundTruthImage);

  drawGroundTruth(frame, groundTruth);
  tuple<double, double, double> rates = calculateRates(frame, groundTruth, detectionOutput);
  double tp(get<0>(rates));
  double fp(get<1>(rates));
  double fn(get<2>(rates));

  cout<<"True positives: "<< tp <<endl;
  cout<<"False positives: "<< fp <<endl;
  cout<<"False negatives: "<< fn <<endl;

  cout<<"F1 score:"<<calculateF1Score(tp, fp, fn)<<endl;
}

void drawGroundTruth(Mat frame, vector<Rectangle> groundTruth){
  for(auto box : groundTruth){
    //draw groundTruth rectangle with red color
    rectangle(frame, box.top, box.bottom, Scalar(0,0,255),2 );
  }
}

tuple<double, double, double> calculateRates(Mat frame, vector<Rectangle> groundTruth, vector<Rect> detectionOutput){
  //initialise the three rates (true positives false positives and false negatives with 0)
  double tp(0),fp(0),fn(0);

  for(auto truthBox : groundTruth){
    bool detected = false;
    for(auto detection : detectionOutput){
      Rectangle rect;
      Point p0 = Point(detection.x, detection.y);
      Point p1 = Point(detection.x + detection.width, detection.y + detection.height);
      rect.top = p0;
      rect.bottom = p1;
      bool result = is_TP(truthBox, rect);
      if(result){
        //draw the true positives with blue on the frame
        rectangle(frame, p0, p1, Scalar( 255, 153, 51 ), 2);
        detected = true;
        break;
      }
    }
    if(detected){
      tp++;
    } else {
      fn++;
    }
  }
  fp = detectionOutput.size() - tp;

  return tuple<double, double, double>(tp, fp, fn);
}

double calculateF1Score(double tp, double fp, double fn){
  return 2 * tp / (2 * tp + fp + fn);
}

#endif
