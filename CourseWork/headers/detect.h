#ifndef DETECT_H
#define DETECT_H

using namespace cv;
void detectCircles(Mat houghSpace, Mat H, int maxRadius, int threshold, Mat original){
  //std::cout<<H;
  for(int y = 0; y < houghSpace.rows; y++){
    for(int x = 0; x < houghSpace.cols; x++){
      if(houghSpace.at<uchar>(y,x) >= threshold){
        Point center = Point(x,y);
        //minimum number of votes for the maximum radius to be considered correct
        const int minVotes = 5;
        for(int radius = H.size[2] - 1; radius >= 0; radius--){
          if(H.at<float>(y,x,radius)>minVotes){
            circle(original,center,radius,Scalar(255,255,0),2);
            break;
          }
        }
      }
    }
  }
  imshow("detections", original);
}


#endif
