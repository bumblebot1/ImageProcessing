#ifndef DETECT_H
#define DETECT_H

using namespace cv;
void detectCircles(Mat houghSpace, Mat H, int maxRadius, int threshold, Mat original){
  const int windowSize = 250;
  for(int y = 0; y <= houghSpace.rows; y += windowSize){
    for(int x = 0; x <= houghSpace.cols; x += windowSize){
      Point center = Point(0, 0);
      int max = 0;
      for(int a = 0; a < windowSize; a++){
        for(int b = 0; b < windowSize; b++){
          if(x + b < houghSpace.cols
            && y + a < houghSpace.rows
            && houghSpace.at<uchar>(y + a, x + b) >= threshold
            && houghSpace.at<uchar>(y + a, x + b) > max){


            center = Point(x + b, y + a);
            max = houghSpace.at<uchar>(y + a, x + b);
          }
        }
      }

      if(max == 0){
        continue;
      }

      const int minVotes = 5;
      for(int radius = H.size[2] - 1; radius >= 0; radius--){
        if(H.at<float>(center.y,center.x,radius)>minVotes){
          Point p0 = Point(center.x - radius, center.y - radius);
          Point p1 = Point(center.x + radius, center.y + radius);
          rectangle(original, p0, p1, Scalar(255,255,0), 2);
          break;
        }
      }
    }
  }

  imshow("detections", original);
}


#endif
