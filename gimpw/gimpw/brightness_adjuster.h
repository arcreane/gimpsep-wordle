#ifndef BRIGHTNESS_ADJUSTER_H
#define BRIGHTNESS_ADJUSTER_H

#include <opencv2/opencv.hpp>

class BrightnessAdjuster {
public:
    BrightnessAdjuster(double factor = 1.0);
    cv::Mat adjust(const cv::Mat& image) const;

private:
    double factor_;
};

#endif