#include <stdio.h>
#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"

using namespace std;

int is_TP(Rectangle groundTruth, Rectangle detected)
{
	Point resultTL, resultBR;
	resultTL.x = max(detected.top.x, groundTruth.top.x);
	resultTL.y = max(detected.top.y, groundTruth.top.y);

	resultBR.x = min(detected.bottom.x, groundTruth.bottom.x);
	resultBR.y = max(detected.bottom.y, groundTruth.bottom.y);

	double groundTruthArea =  (groundTruth.bottom.y - groundTruth.top.y)*(groundTruth.bottom.x - groundTruth.top.x);
	double resultArea 	   =  (resultBR.y - resultTL.y)*(resultBR.x - resultTL.x)

	if ( max(detected.top.x, groundTruth.top.x) > min(detected.bottom.x, groundTruth.bottom.x) 
	  || max(detected.top.y, groundTruth.top.y) > min(detected.bottom.y, groundTruth.bottom.y) )
		return 0;

	if ( resultArea*2 >= groundTruthArea )
		return 1;

	return 0; 
}