#ifndef DETECT_H
#define DETECT_H

#include "vector"

using namespace cv;
using namespace std;

vector<Rect> detectCircles(Mat houghSpace, Mat H, int threshold, Mat original){
  vector<Rect> hCircleDetections;

  const int windowSize = 250;
  for(int y = 0; y < houghSpace.rows; y += windowSize){
    for(int x = 0; x < houghSpace.cols; x += windowSize){
      Point center = Point(0, 0);
      int max = 0;
      for(int a = 0; a < windowSize && y + a < houghSpace.rows; a++){
        for(int b = 0; b < windowSize && x + b < houghSpace.cols; b++){
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
        if(H.at<float>(center.y,center.x,radius) > minVotes){
          //circle(original, center, radius, Scalar(255,255, 0), 2);
          Point p0 = Point(center.x - radius, center.y - radius);
          Point p1 = Point(center.x + radius, center.y + radius);
          Rect detected = Rect(p0, p1);
          hCircleDetections.push_back(detected);
          break;
        }
      }
    }
  }

  return hCircleDetections;
}

void displayHCircleDetections(Mat frame, vector<Rect> hCircleDetections)
{
  for(auto detected : hCircleDetections)
  {
    rectangle(frame, detected.tl(), detected.br(), Scalar( 255, 255, 0 ), 2);
  }
}


void detectLines(Mat houghLinesSpace, int threshold, Mat original)
{
  vector< pair<Point, Point> > lines;

  for(int r=0; r<houghLinesSpace.rows; r++)
  {
    for(int t=0; t<houghLinesSpace.cols; t++)
    {
      if(houghLinesSpace.at<float>(r,t) > threshold)
      {
        Point point1 = Point(0,0);
        Point point2 = Point(0,0);
        
        float angle_rad = t * M_PI / 180; 

        int x1 = 0;
        int y1 = r/std::sin(angle_rad);
        point1.x = x1;
        point1.y = y1;

        int x2 = r/std::cos(angle_rad);
        int y2 = 0;
        point2.x = x2;
        point2.y = y2;

        line(original, point1, point2, Scalar(0,0,255), 2); 

        lines.push_back( pair<Point, Point>(point1, point2) );

        //x1 = r * std::cos(angle_rad);
        //y1 = r * std::sin(angle_rad);

        /*if(x>=45 && x<=135)
        {

          //y = (r - x cos(t)) / sin(t)
          x1 = 0;  
          y1 = (y - ((x1 - (_img_w/2) ) * cos(t * DEG2RAD))) / sin(t * DEG2RAD) + (_img_h / 2);  
          x2 = _img_w - 0;  
          y2 = ((double)(r-(_accu_h/2)) - ((x2 - (_img_w/2) ) * cos(t * DEG2RAD))) / sin(t * DEG2RAD) + (_img_h / 2);    
        }*/
        //cout<<houghLinesSpace.at<float>(y,x)<<" ";
      }
    }
    //cout<<endl;
  }

}


#endif
