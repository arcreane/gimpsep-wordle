#include "panorama.h"

bool Panorama::loadImages(const string& path) {
    Mat img = imread(path);
    if (img.empty()) {
        cerr << "Error: Could not open or find the image!" << endl;
        return false;
    }
    images.push_back(img);
    return true;
}

size_t Panorama::getImagesLoadedCount() const {
    return images.size();
}

void Panorama::clearImages() {
    images.clear();
}

bool Panorama::computePanorama() {
    if (images.size() < 2) return false;

    Ptr<Stitcher> stitcher = Stitcher::create(Stitcher::PANORAMA);
    stitcher->setExposureCompensator(detail::ExposureCompensator::createDefault(detail::ExposureCompensator::GAIN_BLOCKS));
    stitcher->setPanoConfidenceThresh(0.5);

    for (const auto& img : images) {
        if (img.empty()) return false;
    }

    Stitcher::Status status = stitcher->stitch(images, panoramaResult);

    if (status != Stitcher::OK) {
        panoramaResult.release();
        return false;
    }

    return true;
}

Mat Panorama::getPanoramaImage() const {
    return panoramaResult;
}

bool Panorama::savePanoramaImage(const string& path) const {
    if (panoramaResult.empty()) return false;
  
    bool success = imwrite(path, panoramaResult);
   
    return success;
}
