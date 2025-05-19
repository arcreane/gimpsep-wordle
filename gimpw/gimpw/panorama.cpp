#include "image_stitcher.h"
#include <opencv2/imgcodecs.hpp>

ImageStitcher::ImageStitcher() {
   //initiate stich in panorama mode 
    stitcher = cv::Stitcher::create(cv::Stitcher::PANORAMA);
}

bool ImageStitcher::stitchImages(const QStringList& filePaths, cv::Mat& output) {
    std::vector<cv::Mat> images;
    for (const QString& path : filePaths) {  //upload images from Qt paths
        cv::Mat img = cv::imread(path.toStdString());
        if (img.empty()) {
            qWarning("failed to load image: %s", qUtf8Printable(path));
            return false;
        }
        images.push_back(img);
    }

    cv::Stitcher::Status status = stitcher->stitch(images, output); //start the stitching function 

    return status == cv::Stitcher::OK;
}
