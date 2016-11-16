/////////////////////////////////////////////////////////////////////////////
//
// COMS30121 - face.cpp
//
/////////////////////////////////////////////////////////////////////////////

// header inclusion
// header inclusion
#include <stdio.h>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <accuracyMeasurement.h>

using namespace std;
using namespace cv;

/** Function Headers */
vector<Rect> detectAndDisplay( Mat frame );
void verifyDetections(Mat frame, vector<Rect> detectionOutput, String groundTruthImage);
void drawGroundTruth(Mat frame, vector<Rectangle> groundTruth);
tuple<double, double, double> calculateRates(Mat frame, vector<Rectangle> groundTruth, vector<Rect> detectionOutput);
double calculateF1Score(double tp, double fp, double fn);

/** Global variables */
String cascade_name = "dartcascade/cascade.xml";
CascadeClassifier cascade;
String imagesPath = "Test Images/";
String groundTruthPath = "groundTruth/";

/** @function main */
int main( int argc, const char** argv )
{
  stringstream testStream;
  testStream<<imagesPath<<"dart"<<argv[1]<<".jpg";

  stringstream groundTruthStream;
  groundTruthStream<<groundTruthPath<<"image"<<argv[1]<<".txt";

  String testImage = testStream.str();
  String groundTruthImage = groundTruthStream.str();

  // 1. Read Input Image
	Mat frame = imread(testImage, CV_LOAD_IMAGE_COLOR);

	// 2. Load the Strong Classifier in a structure called `Cascade'
	if( !cascade.load( cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };

	// 3. Detect Faces and Display Result
	vector<Rect> detections = detectAndDisplay( frame );

  verifyDetections(frame, detections, groundTruthImage);

	// 4. Save Result Image
	imwrite( "detected.jpg", frame );

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
    Point p0 = Point(faces[i].x, faces[i].y);
    Point p1 = Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height);
		rectangle(frame, Point(faces[i].x, faces[i].y), Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height), Scalar( 0, 255, 0 ), 2);
	}

  return faces;
}

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
