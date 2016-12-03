#ifndef DETECT_HPP
#define DETECT_HPP

using namespace cv;
using namespace std;

vector<Point3i> detectCircles(Mat houghSpace, Mat H, int threshold, Mat original){
  vector<Point3i> hCircleDetections;

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
            && houghSpace.at<uchar>(y + a, x + b) > max
            && x + b != 0
            && y + a != 0){

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
          Point3i detected = Point3i(center.x, center.y, radius);
          hCircleDetections.push_back(detected);
          break;
        }
      }
    }
  }
  return hCircleDetections;
}

void displayHCircleBoundingBoxes(Mat frame, vector<Rect> hCircleDetections){
  for(auto detected : hCircleDetections)
  {
    rectangle(frame, detected.tl(), detected.br(), Scalar( 255, 255, 0 ), 2);
  }
}


void drawHCircleDetections(Mat frame, vector<Point3i> hCircleDetections){
  for(auto detected : hCircleDetections)
  {
    int radius = detected.z;
    Point tl = Point(detected.x - radius, detected.y - radius);
    Point br = Point(detected.x + radius, detected.y + radius);

    //draw detections with light blue on original frame
    rectangle(frame, tl, br, Scalar( 255, 255, 0 ), 2);
  }
}

vector<Point3i> detectLinesIntersection(Mat houghSpace, Mat H, int threshold, Mat original, Mat orientation, Mat magnitude)
{
  vector<Point3i> hIntersectionDetections;

  const int windowSize = 250;

  for(int y=0; y<houghSpace.rows; y+=windowSize)
  {
    for(int x=0; x<houghSpace.cols; x+=windowSize)
    {
      Point center = Point(0, 0);
      int max = 0;
      for(int a = 0; a < windowSize && y + a < houghSpace.rows; a++)
      {
        for(int b = 0; b < windowSize && x + b < houghSpace.cols; b++)
        {
          if(houghSpace.at<uchar>(y + a, x + b) >= threshold && houghSpace.at<uchar>(y + a, x + b) > max
            && y + a < houghSpace.rows && x + b < houghSpace.cols 
            && x + b != 0 && y + a != 0)
          {
            center = Point(x + b, y + a);
            max = houghSpace.at<uchar>(y + a, x + b);
          }
        }
      }

      if(max == 0)
      {
        continue;
      }

      int windowMaxRadius = 60;
      int distMax = 0;
      int thresholdPixelMaxRadius = 3;

      for(int a = -windowMaxRadius; a<=windowMaxRadius; a++){
        for(int b = -windowMaxRadius; b<=windowMaxRadius; b++){
          if( center.y + a < houghSpace.rows && center.x + b < houghSpace.cols
            && center.y + a >= 0 && center.x + b >= 0 && magnitude.at<uchar>(center.y + a, center.x + b) == 255 ){

              int dist = std::sqrt(a*a + b*b);
              int y0 = (center.y+a) + dist*std::sin( orientation.at<float>(center.y+a, center.x+b) + (90*M_PI / 180) );
              int x0 = (center.x+b) + dist*std::cos( orientation.at<float>(center.y+a, center.x+b) + (90*M_PI / 180) );

              //cout<<"("<<center.x<<","<<center.y<<") - ("<<x0<<","<<y0<<")"<<endl;

              if(x0 == center.x && y0 == center.y && dist > distMax){
                distMax = dist;
              }

              y0 = (center.y+a) - dist*std::sin( orientation.at<float>(center.y+a, center.x+b) + (90*M_PI / 180) );
              x0 = (center.x+b) - dist*std::cos( orientation.at<float>(center.y+a, center.x+b) + (90*M_PI / 180) );

              if(x0 == center.x && y0 == center.y && dist > distMax){
                distMax = dist;
              }
          }
        }
      }

      Point3i detected = Point3i(center.x, center.y, distMax);
      hIntersectionDetections.push_back(detected);
    }
  }
  return hIntersectionDetections;
}

#endif
