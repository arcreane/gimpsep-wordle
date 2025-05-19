#include "image_stitcher.h"
#include <opencv2/imgcodecs.hpp>
#include <QDebug>

ImageStitcher::ImageStitcher() {

    stitcher = cv::Stitcher::create(cv::Stitcher::PANORAMA);
}

bool ImageStitcher::stitchImages(const QStringList& filePaths, cv::Mat& output) {
    std::vector<cv::Mat> images;

    for (const QString& path : filePaths) {
        cv::Mat img = cv::imread(path.toStdString());
        if (img.empty()) {
            qWarning() << "failed to load image:" << path;
            return false;
        }
        images.push_back(img);
    }

    cv::Stitcher::Status status = stitcher->stitch(images, output);
    if (status != cv::Stitcher::OK) {
        qWarning() << "stitching failed with status code:" << static_cast<int>(status);
        return false;
    }

    return true;
}
