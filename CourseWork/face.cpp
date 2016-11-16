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
#include <stdio.h>
#include <accuracyMeasurement.h>

using namespace std;
using namespace cv;

typedef struct{
  int tp;
  int fp;
  int fn;
} t_rate;

/** Function Headers */
void detectAndDisplay( Mat frame );
void drawGroundTruth(Mat frame, vector<Rectangle> groundTruth);
t_rate calculateRates(Mat frame, vector<Rectangle> groundTruth, vector<Rect> detectionOutput);

/** Global variables */
String cascade_name = "dartcascade/cascade.xml";
CascadeClassifier cascade;


/** @function main */
int main( int argc, const char** argv )
{
       // 1. Read Input Image
	Mat frame = imread(argv[1], CV_LOAD_IMAGE_COLOR);

	// 2. Load the Strong Classifier in a structure called `Cascade'
	if( !cascade.load( cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };

	// 3. Detect Faces and Display Result
	detectAndDisplay( frame );

	// 4. Save Result Image
	imwrite( "detected.jpg", frame );

	return 0;
}

/** @function detectAndDisplay */
void detectAndDisplay( Mat frame )
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


  //read GroundTruth textFile and draw boxes for it
  vector<Rectangle> groundTruth = readFile("groundTruth/image8.txt");
  drawGroundTruth(frame,groundTruth);
  t_rate rates = calculateRates(frame, groundTruth, faces);

  cout<<"True positives: "<<rates.tp<<endl;
  cout<<"False positives: "<<rates.fp<<endl;
  cout<<"False negatives: "<<rates.fn<<endl;
}

void drawGroundTruth(Mat frame, vector<Rectangle> groundTruth){
  for(auto box : groundTruth){
    //draw groundTruth rectangle with red color
    rectangle(frame, box.top, box.bottom, Scalar(0,0,255),2 );
  }
}

t_rate calculateRates(Mat frame, vector<Rectangle> groundTruth, vector<Rect> detectionOutput){
  t_rate result;
  result.tp = 0;
  result.fp = 0;
  result.fn = 0;
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
        //display the true positives with yellow
        rectangle(frame, p0, p1, Scalar( 102, 255, 255 ), 2);
        detected = true;
        break;
      }
    }
    if(detected){
      result.tp++;
    } else {
      result.fn++;
    }
  }
  result.fp = detectionOutput.size() - result.tp;

  return result;
}
