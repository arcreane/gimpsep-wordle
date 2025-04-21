#ifndef DETECTION_AND_MORPHOLOGY_H
#define DETECTION_AND_MORPHOLOGY_H

#include <opencv2/opencv.hpp>

cv::Mat morphImage(const cv::Mat& input, int size, bool dilate); // erosion or dilate


void detectFacesCats(cv::Mat& image, // detect faces, smiles or cats
    cv::CascadeClassifier& face_cascade,
    cv::CascadeClassifier& smile_cascade,
    cv::CascadeClassifier& cat_cascade);


void runDetectionDilate(); // my main function 

#endif
