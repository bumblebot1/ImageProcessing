#include <stdio.h>
#include <opencv/cv.h>        //you may need to
#include <opencv/highgui.h>   //adjust import locations
#include <opencv/cxcore.h>    //depending on your machine setup

using namespace cv;

int main() {

    // Read image from file
    Mat image = imread("mandrill.jpg", 0);
    Mat result = Mat(image.rows, image.rows, CV_8UC1);
    Mat mask = Mat(3, 3, CV_8UC1);
    for(int a = 0; a < 3; a++) {
        for(int b = 0; b < 3; b++) {
            mask.at<uchar>(a,b) = 1;
        }
    }

    for(int x = 1; x < image.rows - 1; x++) {
        for(int y = 1; y < image.cols - 1; y++) {
            int pixel = 0;
            for(int i = -1; i <= 1 ; i++) {
                for(int j = -1; j<= 1; j++) {
                    pixel += image.at<uchar>(x-i, y-j) * mask.at<uchar>(i+1,j+1);
                }
            }
            result.at<uchar>(x,y) = pixel / 9;
        }
    }

    //Save thresholded image
    imwrite("convolution.jpg", result);

    return 0;
}
