#ifndef DETECTION_AND_MORPHOLOGY_H
#define DETECTION_AND_MORPHOLOGY_H

#include <opencv2/opencv.hpp>

// Appliquer d�tection (faces, chats, sourires) et retourner l'image modifi�e
cv::Mat applyDetection(const cv::Mat& input);

// Appliquer morphologie (�rosion/dilatation)
cv::Mat applyMorphologyGUI(const cv::Mat& input, int size, bool dilate);

#endif
