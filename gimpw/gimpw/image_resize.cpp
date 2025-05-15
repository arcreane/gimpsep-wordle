#include "image_resize.h"
#include <opencv2/imgproc.hpp>  // Pour cv::resize

cv::Mat ImageResizer::resize(const cv::Mat& input, double fx, double fy) {
    cv::Mat output;
    cv::resize(input, output, cv::Size(), fx, fy);
    return output;
}