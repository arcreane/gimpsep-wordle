#ifndef FUNCTIONS_FDETECTION_DILATEEROSE_H
#define FUNCTIONS_FDETECTION_DILATEEROSE_H

#include <opencv2/opencv.hpp>

cv::Mat morphImage(const cv::Mat& input, int size, bool dilate); // dilate or erosion of the image

void detectFacesCats(cv::Mat& image, // detection cat and human with smile or not
    cv::CascadeClassifier& face_cascade,
    cv::CascadeClassifier& smile_cascade,
    cv::CascadeClassifier& cat_cascade);

#endif
