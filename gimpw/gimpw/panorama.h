#include "panorama.h"

ImageStitcher::ImageStitcher() {
    stitcher = cv::Stitcher::create(cv::Stitcher::PANORAMA);
}

bool ImageStitcher::stitchImages(const QStringList& filePaths, cv::Mat& output) {
    std::vector<cv::Mat> images;

    //upload images from Qt paths
    for (const QString& path : filePaths) {
        cv::Mat img = cv::imread(path.toStdString());
        if (img.empty()) {
            return false; //error fail
        }
        images.push_back(img);
    }

    //do the panorama function 
    cv::Stitcher::Status status = stitcher->stitch(images, output);
    return status == cv::Stitcher::OK;
}
#pragma once
