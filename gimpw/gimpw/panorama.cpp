#include "image_stitcher.h"
#include <opencv2/imgcodecs.hpp>
#include <QDebug>

ImageStitcher::ImageStitcher() {
    //create the OpenCV stitcher in panorama mode
    stitcher = cv::Stitcher::create(cv::Stitcher::PANORAMA);
}

bool ImageStitcher::stitchImages(const QStringList& filePaths, cv::Mat& output) {
    std::vector<cv::Mat> images;

    //load each image from the Qt file list
    for (const QString& path : filePaths) {
        cv::Mat img = cv::imread(path.toStdString());
        if (img.empty()) {
            qWarning() << "failed to load image:" << path;
            return false; //if any image fails we stop
        }
        images.push_back(img);
    }

    //try to stitch all the images into a panorama
    cv::Stitcher::Status status = stitcher->stitch(images, output);
    if (status != cv::Stitcher::OK) {
        qWarning() << "stitching failed with status code:" << static_cast<int>(status);
        return false; //return false if stitching didn't work
    }

    return true; // success
}
