#include <stdio.h>
#include <opencv/cv.h>        //you may need to
#include <opencv/highgui.h>   //adjust import locations
#include <opencv/cxcore.h>    //depending on your machine setup

using namespace cv;

int main( int argc, char** argv )
{

 // LOADING THE IMAGE
 char* imageName = argv[1];

 Mat image;
 image = imread( imageName, 0 );

 if( argc != 2 || !image.data )
 {
   printf( " No image data \n " );
   return -1;
 }

 Mat recovered = Mat(image.rows, image.cols, CV_8UC1);

 //nicest way to do it in one line cv::medianBlur(image, recovered, 5);

 for(int x = 2; x < image.rows-2; x++) {
   for(int y = 2; y < image.cols-2; y++) {
     std::vector<uchar> vec;
     for(int i = -2; i <= 2; i++) {
       for(int j = -2; j <= 2; j++) {
         vec.push_back(image.at<uchar>(x+i, y+j));
       }
     }
     std::sort(vec.begin(), vec.end());
     recovered.at<uchar>(x,y) = vec[12];
   }
 }

 imwrite( "medianfilter.png", recovered );

 return 0;
}
