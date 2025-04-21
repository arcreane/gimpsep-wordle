#ifndef IMAGE_MORPHOLOGY_H
#define IMAGE_MORPHOLOGY_H

#include <opencv2/opencv.hpp>

cv::Mat applyMorphology(const cv::Mat& input, int size, bool dilate);

#endif
