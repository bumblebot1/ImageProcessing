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

int is_TP(Rectangle groundTruth, Rectangle detected)
{
	Point resultTL, resultBR;
	resultTL.x = max(detected.top.x, groundTruth.top.x);
	resultTL.y = max(detected.top.y, groundTruth.top.y);

	resultBR.x = min(detected.bottom.x, groundTruth.bottom.x);
	resultBR.y = min(detected.bottom.y, groundTruth.bottom.y);

	double groundTruthArea =  (groundTruth.bottom.y - groundTruth.top.y)*(groundTruth.bottom.x - groundTruth.top.x);
	double resultArea 	   =  (resultBR.y - resultTL.y)*(resultBR.x - resultTL.x);

	if ( max(detected.top.x, groundTruth.top.x) > min(detected.bottom.x, groundTruth.bottom.x)
	  || max(detected.top.y, groundTruth.top.y) > min(detected.bottom.y, groundTruth.bottom.y) )
		return 0;

	if ( resultArea*2 >= groundTruthArea )
		return 1;

	return 0;
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