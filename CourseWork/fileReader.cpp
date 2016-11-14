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

int main(void){
  vector<Rectangle> groundTruth = readFile("file.txt");
  return 0;
}
