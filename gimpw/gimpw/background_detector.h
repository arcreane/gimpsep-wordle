#ifndef BACKGROUND_DETECTOR_H
#define BACKGROUND_DETECTOR_H

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

class BackgroundDetector {
    private:
        Mat resultMask;

    public:
        BackgroundDetector();

        void apply(const Mat& image, const Rect& rect);
        Mat getForeground(const Mat& image) const;
        Mat& getMaskRef();
        void saveForegroundAsPNG(const Mat& originalImage, const string& outputPath) const;
};

void runBackgroundDetection();

#endif 
