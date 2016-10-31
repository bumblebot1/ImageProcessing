// header inclusion
#include <stdio.h>
#include <opencv/cv.h>        //you may need to
#include <opencv/highgui.h>   //adjust import locations
#include <opencv/cxcore.h>    //depending on your machine setup


using namespace cv;

int x_kernel[3][3] = {
						{-1, 0, 1},
						{-2, 0, 2},
						{-1, 0, 1}
					 };

int main()
{
  	  Mat image = imread("coins1.jpg", 1);
  	  sobel(image);
}


void sobel(cv::Mat &image)
{
	for(int i=0; i<image.rows; i++)
	{
		for(int j=0; j<image.cols; j++)
		{
			for(int r=0; r<3; r++)
			{
				int index_r = 3-1-r;

				for(int c=0; c<3; c++)
				{
					int index_c = 3-1-c;

					int ii = i + r - 3/2;
					int jj = j + c - 3/2;

					if(ii>=0 && ii<image.rows && jj>=0 && jj<image.cols)
						image[i][j]* = x_kernel[index_r][index_c];
				}
			}
		}
	}

	imwrite("coins1.jpg", image);
}