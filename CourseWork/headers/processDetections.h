#ifndef DETECTIONS_H
#define DETECTIONS_H

#include "iostream"
#include "vector"
#include "opencv2/opencv.hpp"
#include "accuracyMeasurement.h"

using namespace std;
using namespace cv;

/*function headers*/
void verifyDetections(Mat frame, vector<Rect> detectionOutput, String groundTruthImage);
void drawGroundTruth(Mat frame, vector<Rect> groundTruth);
tuple<double, double, double> calculateRates(Mat frame, vector<Rect> groundTruth, vector<Rect> detectionOutput);
double calculateF1Score(double tp, double fp, double fn);


void verifyDetections(Mat frame, vector<Rect> detectionOutput, String groundTruthImage){
  //read GroundTruth textFile & store predicted dartboards
  vector<Rect> groundTruth = readFile(groundTruthImage);

  //draw boxes for predicted/GroundTruth dartboards
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

void drawGroundTruth(Mat frame, vector<Rect> groundTruth){
  for(auto box : groundTruth){
    //draw groundTruth rectangle with red color
    rectangle(frame, box.tl(), box.br(), Scalar(0,0,255),2 );
  }
}

tuple<double, double, double> calculateRates(Mat frame, vector<Rect> groundTruth, vector<Rect> detectionOutput){
  //initialise the three rates (true positives false positives and false negatives with 0)
  double tp(0),fp(0),fn(0);

  for(auto truthBox : groundTruth){
    bool detected = false;

    for(auto detection : detectionOutput){
      bool result = is_TP(truthBox, detection);

      if(result){
        //draw the true positives with blue on the frame
        rectangle(frame, detection.tl(), detection.br(), Scalar( 255, 153, 51 ), 2);
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
