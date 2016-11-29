#ifndef ACCURACY_H
#define ACCURACY_H

#include "iostream"
#include "fstream"
#include "vector"
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

bool is_TP(Rect groundTruth, Rect detected)
{
  Point resultT, resultB;
  resultT.x = max(detected.tl().x, groundTruth.tl().x);
  resultT.y = max(detected.tl().y, groundTruth.tl().y);

  resultB.x = min(detected.br().x, groundTruth.br().x);
  resultB.y = min(detected.br().y, groundTruth.br().y);

  double groundTruthArea =  (groundTruth.br().y - groundTruth.tl().y)*(groundTruth.br().x - groundTruth.tl().x);
  double resultArea 	   =  (resultB.y - resultT.y)*(resultB.x - resultT.x);
  double detectedArea    =  (detected.br().y - detected.tl().y)*(detected.br().x - detected.tl().x);

  if ( max(detected.tl().x, groundTruth.tl().x) > min(detected.br().x, groundTruth.br().x)
    || max(detected.tl().y, groundTruth.tl().y) > min(detected.br().y, groundTruth.br().y) )
    return false;

  cout<<"Detection Area:"<<detectedArea<<" Ground Truth Area:"<<groundTruthArea<<endl;
  if ( detectedArea >= groundTruthArea * 2 )
    return false;

  cout<<"Intersection Area:"<<resultArea<<" Ground Truth Area:"<<groundTruthArea<<endl;
  if ( resultArea * 2 >= groundTruthArea )
    return true;

  return false;
}

vector<Rect> readFile(const string& fileName){
  ifstream input(fileName.c_str());

  int a,b,c,d;
  vector<Rect> groundTruth;

  while(input >> a >> b >> c >> d){
    Point top = Point(a,b);
    Point bottom = Point(c,d);
    Rect rectangle = Rect(top, bottom);
    groundTruth.push_back(rectangle);
  }

  for( auto rect : groundTruth ){
    cout<<rect.tl()<<" "<<rect.br()<<endl;
  }

  return groundTruth;
}

#endif
