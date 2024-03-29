#include <stdio.h>
#include <opencv/cv.h>        //you may need to
#include <opencv/highgui.h>   //adjust import locations
#include <opencv/cxcore.h>    //depending on your machine setup

using namespace cv;

int main( void ) {
 Mat image;
 image = imread( "car1.png", 0 );

 Mat blurred;
 blurred = imread("blur.jpg", 0);

 Mat recovered = Mat(image.rows, image.cols, CV_32SC1);
 Mat result = Mat(image.rows, image.cols, CV_8UC1);
 for(int i = 0; i < image.rows; i++) {
   for(int j = 0; j < image.cols; j++) {
     recovered.at<int>(i,j) = ((int) image.at<uchar>(i,j) )*2;
     recovered.at<int>(i,j) -= (int) blurred.at<uchar>(i,j);
   }
 }
 double min, max;
 cv::minMaxLoc(recovered, &min, &max);
 for(int i = 0; i < image.rows; i++) {
   for(int j = 0; j < image.cols; j++) {
     recovered.at<int>(i,j) -= (int) min;
   }
 }
 cv::minMaxLoc(recovered, &min, &max);
 double res = 255/max;
 for(int i = 0; i < image.rows; i++) {
   for(int j = 0; j < image.cols; j++) {
     recovered.at<int>(i,j) = (int)(recovered.at<int>(i,j)*res);
   }
 }

 for(int i = 0; i < image.rows; i++) {
   for(int j = 0; j < image.cols; j++) {
     result.at<uchar>(i,j) = (uchar)recovered.at<int>(i,j);
   }
 }

 imwrite("sharpened.png", result);

 return 0;
}

void GaussianBlur(cv::Mat &input, int size, cv::Mat &blurredOutput)
{
	// intialise the output using the input
	blurredOutput.create(input.size(), input.type());

	// create the Gaussian kernel in 1D
	cv::Mat kX = cv::getGaussianKernel(size, -1);
	cv::Mat kY = cv::getGaussianKernel(size, -1);

	// make it 2D multiply one by the transpose of the other
	cv::Mat kernel = kX * kY.t();

	//CREATING A DIFFERENT IMAGE kernel WILL BE NEEDED
	//TO PERFORM OPERATIONS OTHER THAN GUASSIAN BLUR!!!

	// we need to create a padded version of the input
	// or there will be border effects
	int kernelRadiusX = ( kernel.size[0] - 1 ) / 2;
	int kernelRadiusY = ( kernel.size[1] - 1 ) / 2;

	cv::Mat paddedInput;
	cv::copyMakeBorder( input, paddedInput,
		kernelRadiusX, kernelRadiusX, kernelRadiusY, kernelRadiusY,
		cv::BORDER_REPLICATE );

	// now we can do the convoltion
	for ( int i = 0; i < input.rows; i++ )
	{
		for( int j = 0; j < input.cols; j++ )
		{
			double sum = 0.0;
			for( int m = -kernelRadiusX; m <= kernelRadiusX; m++ )
			{
				for( int n = -kernelRadiusY; n <= kernelRadiusY; n++ )
				{
					// find the correct indices we are using
					int imagex = i + m + kernelRadiusX;
					int imagey = j + n + kernelRadiusY;
					int kernelx = m + kernelRadiusX;
					int kernely = n + kernelRadiusY;

					// get the values from the padded image and the kernel
					int imageval = ( int ) paddedInput.at<uchar>( imagex, imagey );
					double kernalval = kernel.at<double>( kernelx, kernely );

					// do the multiplication
					sum += imageval * kernalval;
				}
			}
			// set the output value as the sum of the convolution
			blurredOutput.at<uchar>(i, j) = (uchar) sum;
		}
	}
}
