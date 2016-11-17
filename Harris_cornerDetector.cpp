#include <stdio.h>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include < opencv2\stitching\stitcher.hpp >  

using namespace cv;

/** @function main */
int main(int argc){
	int thresh = 200;
	// Load the images
	Mat image1 = imread("p2.jpg", 1);//imgae1
	Mat image2 = imread("p1.jpg", 1);//image2
	Mat gray_image1;
	Mat gray_image2;


	// Convert to Grayscale
	cvtColor(image1, gray_image1, CV_RGB2GRAY);
	cvtColor(image2, gray_image2, CV_RGB2GRAY);




	Mat dst, dst_norm, dst_norm_scaled;
	Mat dst2, dst_norm2, dst_norm_scaled2;
	dst = Mat::zeros(image1.size(), CV_32FC1);
	dst2 = Mat::zeros(image1.size(), CV_32FC1);
	/// Detector parameters
	int blockSize = 2;
	int apertureSize = 5;
	double k = 0.05;

	/// Detecting corners
	cornerHarris(gray_image1, dst, blockSize, apertureSize, k, BORDER_DEFAULT);
	cornerHarris(gray_image2, dst2, blockSize, apertureSize, k, BORDER_DEFAULT);

	/// Normalizing
	normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
	normalize(dst2, dst_norm2, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
	convertScaleAbs(dst_norm, dst_norm_scaled);
	convertScaleAbs(dst_norm2, dst_norm_scaled2);

	/// Drawing a circle around corners
	for (int j = 0; j < dst_norm.rows; j++){
		for (int i = 0; i < dst_norm.cols; i++){
			if ((int)dst_norm.at<float>(j, i) > thresh){
				circle(image1, Point(i, j), 6, CV_RGB(255, 0, 0), 1, 8, 0);
			}
		}
	}
	for (int j = 0; j < dst_norm2.rows; j++){
		for (int i = 0; i < dst_norm2.cols; i++){
			if ((int)dst_norm2.at<float>(j, i) > thresh){
				circle(image2, Point(i, j), 6, CV_RGB(255, 0, 0), 1, 8, 0);
			}
		}
	}
	/// Showing the result
	imshow("Harris-p2", image1);
	imshow("Harris-p1", image2);

	waitKey(0);
	return 0;
}
