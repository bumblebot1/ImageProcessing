#ifndef ACCURACY_H
#define ACCURACY_H

#include <iostream>
#include <fstream>
#include <vector>
#include "opencv2/opencv.hpp"
using namespace cv;
using namespace std;

typedef struct {
  Point top;
  Point bottom;
} Rectangle;

bool is_TP(Rectangle groundTruth, Rectangle detected)
{
	Point resultTL, resultBR;
	resultTL.x = max(detected.top.x, groundTruth.top.x);
	resultTL.y = max(detected.top.y, groundTruth.top.y);

	resultBR.x = min(detected.bottom.x, groundTruth.bottom.x);
	resultBR.y = min(detected.bottom.y, groundTruth.bottom.y);

	double groundTruthArea =  (groundTruth.bottom.y - groundTruth.top.y)*(groundTruth.bottom.x - groundTruth.top.x);
	double resultArea 	   =  (resultBR.y - resultTL.y)*(resultBR.x - resultTL.x);
  double detectedArea    =  (detected.bottom.y - detected.top.y)*(detected.bottom.x - detected.top.x);

	if ( max(detected.top.x, groundTruth.top.x) > min(detected.bottom.x, groundTruth.bottom.x)
	  || max(detected.top.y, groundTruth.top.y) > min(detected.bottom.y, groundTruth.bottom.y) )
		return false;

  cout<<"Detection Area:"<<detectedArea<<" Ground Truth Area:"<<groundTruthArea;
  if(detectedArea >= groundTruthArea * 2)
    return false;
  cout<<" Intersection Area:"<<resultArea<<" Ground Truth Area:"<<groundTruthArea<<endl;
	if ( resultArea * 2 >= groundTruthArea )
		return true;

	return false;
}

vector<Rectangle> readFile(const string& fileName){
  ifstream input(fileName.c_str());
  int a,b,c,d;
  vector<Rectangle> groundTruth;
  while(input >> a >> b >> c >> d){
    Rectangle rect;
    rect.top = Point(a,b);
    rect.bottom = Point(c,d);
    groundTruth.push_back(rect);
  }

  for(int ii = 0; ii < groundTruth.size(); ii++){
    Rectangle rect = groundTruth[ii];
    cout<<rect.top<<" "<<rect.bottom<<endl;
  }
  return groundTruth;
}

#endif
