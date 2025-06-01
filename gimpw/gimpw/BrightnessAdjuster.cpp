#include "BrightnessAdjuster.h"

BrightnessAdjuster::BrightnessAdjuster(double factor) : factor_(factor) {}

cv::Mat BrightnessAdjuster::adjust(const cv::Mat& image) const {
    cv::Mat result;
    image.convertTo(result, -1, factor_, 0);
    return result;
}