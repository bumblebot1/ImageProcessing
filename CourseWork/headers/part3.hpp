#ifndef PART3_HPP
#define PART3_HPP

#include "detector.hpp"

using namespace cv;
using namespace std;

vector<Rect> detectAndReturnBoxes(Mat greyImage, Mat colorImage){
  vector<Rect> allDetections = scaledCircleDetector(greyImage, colorImage, 220, 1.0);
  vector<Rect> circleDetections = scaledCircleDetector(greyImage, colorImage, 230, 0.5);
  for(int i = 0; i < allDetections.size(); i++){
    for(int j = 0; j < circleDetections.size(); j++){
      if(checkOverlap(allDetections[i], circleDetections[j])){
        circleDetections.erase(circleDetections.begin() + j);
        cout<<"i:"<<i<<" "<<"j:"<<j<<endl;
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
