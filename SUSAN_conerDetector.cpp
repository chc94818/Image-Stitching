#define _CRT_SECURE_NO_WARNINGS
#include<math.h>  
#include<stdlib.h>  
#include<stdio.h>  
#include "cv.h"  
#include "highgui.h"  

int main(){
	int OffSetX[37] =
	{	-1,  0,  1,
		-2, -1,  0, 1, 2,
		-3, -2, -1, 0, 1, 2, 3,
		-3, -2, -1, 0, 1, 2, 3,
		-3, -2, -1, 0, 1, 2, 3,
		-2, -1,  0, 1, 2,
		-1,  0,  1 };
	int OffSetY[37] =
	{	-3, -3, -3,
		-2, -2, -2, -2, -2,
		-1, -1, -1, -1, -1, -1, -1,
		 0,  0,  0,  0,  0,  0,  0,
		 1,  1,  1,  1,  1,  1,  1,
		 2,  2,  2,  2,  2,
		 3,  3,  3
	};




	//load img
	IplImage* Img = cvLoadImage("b2.jpg", 0);
	IplImage* nimg = cvLoadImage("b2.jpg", 1);
	int step = Img->widthStep / sizeof(uchar);
	uchar *data0 = (uchar*)Img->imageData;


	//find corner
	int max = data0[0];
	int min = data0[0];
	for (int i = 3; i<Img->height - 3; i += 4)
		for (int j = 3; j < Img->width - 3; j += 4){
			int same = 0;
			int sum = 0;
			for (int k = 0; k < 37; k++){
				sum += data0[(i + OffSetY[k])*step + (j + OffSetX[k])];
				float thresh = (float)sum / 37;
				float data_fabs;
				data_fabs = (float)(data0[(i + OffSetY[k])*step + (j + OffSetX[k])] - data0[i*step + j]);
				if (fabs(data_fabs) <= thresh)same++;
			}
			//if corner then draw circle
			if (same < 18){
				cvCircle(nimg, cvPoint(j, i),6, CV_RGB(255, 0, 0), 1, CV_AA, 0);
			}
		}

	//show corner 
	cvShowImage("SUSAN", nimg);
	cvWaitKey(0);
	return 0;
}
