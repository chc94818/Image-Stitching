#define _CRT_SECURE_NO_WARNINGS
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
int main(){	
	// Load the images
	char t[30];
	char in1[30];
	char in2[30];
	printf("img name:");

	scanf("%s %s", in2,in1);
	std::cin.getline(t, 20);

	//std::cin >> in1;
	// std::cin >> in2;
	
	Mat image1;
	Mat image2; 
	
	image1 = imread((string)in1 + ".jpg", 1);
	image2 = imread((string)in2 + ".jpg", 1);
	

	printf("Calculating ... \n");

	if (!image1.data || !image2.data){
		std::cout << "Error reading images " << std::endl; 
		std::cin.getline(t, 20);
		return -1;
	}

	
	Mat gray_image1;
	Mat gray_image2;


	// Convert to Grayscale
	cvtColor(image1, gray_image1, CV_RGB2GRAY);
	cvtColor(image2, gray_image2, CV_RGB2GRAY);

	if (!gray_image1.data || !gray_image2.data){
		std::cout << " --(!) Error reading images " << std::endl; return -1;
	}

	//imshow("first image", image2);
	//imshow("second image", image1);



	if (!gray_image1.data || !gray_image2.data){
		std::cout << " --(!) Error reading images " << std::endl; return -1;
	}

	//-- Step 1: Detect the keypoints using SURF Detector
	int minHessian = 400;
	SurfFeatureDetector detector(minHessian);
	std::vector< KeyPoint > keypoints_object, keypoints_scene;

	detector.detect(gray_image1, keypoints_object);
	detector.detect(gray_image2, keypoints_scene);

	
	cv::Mat M1 = image1.clone();
	cv::Mat M2 = image2.clone();
	
	for (int i = 0; i < keypoints_object.size(); i++){
		circle(M1, keypoints_object[i].pt, 6, CV_RGB(255, 0, 0), 1, 8, 0);
	}
	for (int i = 0; i < keypoints_scene.size(); i++){
		circle(M2, keypoints_scene[i].pt, 6, CV_RGB(255, 0, 0), 1, 8, 0);
	}
	imwrite((string)in1 + "_temp1.jpg", M1);
	imwrite((string)in2 + "_temp2.jpg", M2);


	//-- Step 2: Calculate descriptors (feature vectors)
	SurfDescriptorExtractor extractor;
	Mat descriptors_object, descriptors_scene;

	extractor.compute(gray_image1, keypoints_object, descriptors_object);
	extractor.compute(gray_image2, keypoints_scene, descriptors_scene);
	

	//-- Step 3: Matching descriptor vectors using FLANN matcher
	FlannBasedMatcher matcher;
	std::vector< DMatch > matches;
	matcher.match(descriptors_object, descriptors_scene, matches);
	double max_dist = 0; double min_dist = 100;

	//-- Quick calculation of max and min distances between keypoints
	for (int i = 0; i < descriptors_object.rows; i++){
		double dist = matches[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}


	//-- Use only "good" matches (i.e. whose distance is less than 3*min_dist )
	std::vector< DMatch > good_matches;
	for (int i = 0; i < descriptors_object.rows; i++){
		if (matches[i].distance < 3 * min_dist){
			good_matches.push_back(matches[i]);
		}
	}

	std::vector< Point2f > obj;
	std::vector< Point2f > scene;
	for (int i = 0; i < good_matches.size(); i++){
		//-- Get the keypoints from the good matches
		obj.push_back(keypoints_object[good_matches[i].queryIdx].pt);
		scene.push_back(keypoints_scene[good_matches[i].trainIdx].pt);
	}



	// Find the Homography Matrix
	Mat H = findHomography(obj, scene, CV_RANSAC);
	
	// Use the Homography Matrix to warp the images
	cv::Mat result;
	warpPerspective(image1, result, H, cv::Size(image1.cols + image2.cols, image1.rows));


	cv::Mat half(result, cv::Rect(0, 0, image2.cols, image2.rows));
	image2.copyTo(half);

	//imshow("Thresh = 2000", result);
	imwrite("result(" +(string)in1 +"_"+(string)in2 +").jpg" , result);
	printf("Done.\n");
	std::cin.getline(t, 20);
	waitKey(0);
	return 0;
}
