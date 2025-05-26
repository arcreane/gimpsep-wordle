#ifndef IMAGE_MORPHOLOGY_H
#define IMAGE_MORPHOLOGY_H

#include <opencv2/opencv.hpp>

class Morphology {
public:
    // appliquer morphologie (érosion ou dilatation)
    static cv::Mat applyMorphology(const cv::Mat& input, int size, bool dilate);
};

#endif
