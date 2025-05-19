#ifndef IMAGE_STITCHER_H
#define IMAGE_STITCHER_H

#include <opencv2/opencv.hpp>
#include <opencv2/stitching.hpp>
#include <QStringList>

class ImageStitcher {
public:
    ImageStitcher();
    bool stitchImages(const QStringList& filePaths, cv::Mat& output);

private:
    cv::Ptr<cv::Stitcher> stitcher;
};

#endif 
