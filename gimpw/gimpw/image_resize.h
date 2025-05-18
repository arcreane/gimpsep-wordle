//
// Created by Alexandre Brugger on 15/05/2025.
//

#ifndef IMAGE_RESIZE_H
#define IMAGE_RESIZE_H

#include <opencv2/opencv.hpp>

class ImageResizer {
  public:
    cv::Mat resize(const cv::Mat& input, double fx, double fy);
};



#endif //IMAGE_RESIZE_H
