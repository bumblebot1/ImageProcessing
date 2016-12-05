#ifndef PART4_HPP
#define PART4_HPP

#include "detector.hpp"

using namespace cv;
using namespace std;

vector<Rect> detectAndReturnBoxes(Mat greyImage, Mat colorImage){
  vector<Rect> allDetections = intersectionDetector(greyImage, colorImage);
  vector<Rect> circleDetections = scaledCircleDetector(greyImage, colorImage, 230, 0.5);
  for(int i = 0; i < allDetections.size(); i++){
    for(int j = 0; j < circleDetections.size(); j++){
      if(checkOverlap(allDetections[i], circleDetections[j])){
        circleDetections.erase(circleDetections.begin() + j);
        j--;
      }
    }
  }
  if(circleDetections.size() != 0){
    allDetections.insert(allDetections.end(), circleDetections.begin(), circleDetections.end());
  }
  return allDetections;
}

#endif
