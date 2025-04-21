#ifndef FACE_DETECTION_H
#define FACE_DETECTION_H

#include <opencv2/opencv.hpp>

void detectFacesAndCats(cv::Mat& image,
    cv::CascadeClassifier& face_cascade,
    cv::CascadeClassifier& smile_cascade,
    cv::CascadeClassifier& cat_cascade);

#endif
