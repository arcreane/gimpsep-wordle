#include "panorama.h"
#include <opencv2/stitching.hpp>

cv::Mat Panorama::stitchImages(const std::vector<cv::Mat>& images, bool& success) {
    if (images.size() < 2) {
        success = false;
        return cv::Mat(); // need at laest two images 
    }

    cv::Mat pano;
    cv::Ptr<cv::Stitcher> stitcher = cv::Stitcher::create(cv::Stitcher::PANORAMA);

    cv::Stitcher::Status status = stitcher->stitch(images, pano);
    success = (status == cv::Stitcher::OK);

    if (!success) {
        std::cerr << "Panorama failed, error: " << int(status) << std::endl;
        return cv::Mat();
    }

    return pano;
}
