#ifndef UTILS_HPP
#define UTILS_HPP

using namespace cv;

Mat normalise(Mat src){
  Mat dst;
  normalize(src, dst, 0.0, 255.0, cv::NORM_MINMAX, CV_8U);
  return dst;
}

Mat threshold(Mat original, uchar value){
  Mat result = Mat(original.rows, original.cols, CV_8UC1);
  for(int y = 0; y < original.rows; y++){
    for(int x = 0; x < original.cols; x++){
      if(original.at<uchar>(y,x) > value){
        result.at<uchar>(y,x) = 255;
      }
      else{
        result.at<uchar>(y,x) = 0;
      }
    }
  }
  return result;
}

Mat magnitude(Mat grad_x, Mat grad_y){
  Mat result = Mat(grad_x.rows, grad_x.cols, CV_32FC1);
  for(int y = 0; y < grad_x.rows; y++){
    for(int x = 0; x < grad_x.cols; x++){
      float i = grad_x.at<float>(y,x);
      float j = grad_y.at<float>(y,x);
      result.at<float>(y,x) = std::floor(std::sqrt(i*i + j*j));
    }
  }
  return result;
}

Mat gradient_direction(Mat grad_x, Mat grad_y) {
  Mat result = Mat(grad_x.rows, grad_x.cols, CV_32FC1);
  for(int y = 0; y < grad_x.rows; y++){
    for(int x = 0; x < grad_x.cols; x++){
      float a = grad_x.at<float>(y,x);
      float b = grad_y.at<float>(y,x);
      result.at<float>(y,x) = std::atan2(b,a);
    }
  }
  return result;
}

Mat scaleHeight(Mat original, float factor){
  if(factor == 1){
    //dont resize to same size
    return original;
  }

  Mat resized;
  Size size(original.cols, (int)(original.rows * factor));
  resize(original, resized, size);
  return resized;
}

vector<Rect> convertToBoxes(vector<Point3i> pointRepresentation){
  vector<Rect> boundingBoxes;
  for(auto detection : pointRepresentation){
    Point p0 = Point(detection.x - detection.z, detection.y - detection.z);
    Point p1 = Point(detection.x + detection.z, detection.y + detection.z);
    boundingBoxes.push_back(Rect(p0,p1));
  }
  return boundingBoxes;
}

Mat strengthenEdges(Mat original){
  Mat blurred; /*used to hold the blurred image used in sharpening*/
  Mat sharpEdges;
  GaussianBlur(original, blurred, cv::Size(0, 0), 3);
  addWeighted(original, 1.5, blurred, -0.5, 0, sharpEdges);
  return sharpEdges;
}

bool checkOverlap(Rect first, Rect second){
  Point resultT, resultB;
  resultT.x = max(first.tl().x, second.tl().x);
  resultT.y = max(first.tl().y, second.tl().y);

  resultB.x = min(first.br().x, second.br().x);
  resultB.y = min(first.br().y, second.br().y);
  long resultArea = (resultB.y - resultT.y)*(resultB.x - resultT.x);
  return resultArea > 0;
}

#endif
