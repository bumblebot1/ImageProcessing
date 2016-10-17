#include <stdio.h>
#include <opencv/cv.h>        //you may need to
#include <opencv/highgui.h>   //adjust import locations
#include <opencv/cxcore.h>    //depending on your machine setup

using namespace cv;

int main( void ) {
 Mat image;
 image = imread( "car1.png", 0 );
 imwrite("test.png", image);
 return 0;
}
