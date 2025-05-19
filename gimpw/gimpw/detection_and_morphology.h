#ifndef DETECTION_AND_MORPHOLOGY_H
#define DETECTION_AND_MORPHOLOGY_H

#include <opencv2/opencv.hpp>

// Appliquer détection (faces, chats, sourires) et retourner l'image modifiée
cv::Mat applyDetection(const cv::Mat& input);

// Appliquer morphologie (érosion/dilatation)
cv::Mat applyMorphologyGUI(const cv::Mat& input, int size, bool dilate);

#endif
