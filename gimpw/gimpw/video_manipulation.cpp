#include "video_manipulation.h"
#include <opencv2/imgproc.hpp>

bool VideoManipulation::loadVideo(const std::string& path) {
    video.open(path);
    if (!video.isOpened()) return false;
    totalFrames = static_cast<int>(video.get(cv::CAP_PROP_FRAME_COUNT));
    currentFrame = 0;
    video.set(cv::CAP_PROP_POS_FRAMES, currentFrame);
    video.read(currentFrameMat);
    return !currentFrameMat.empty();
}

bool VideoManipulation::nextFrame() {
    if (!video.isOpened()) return false;
    if (!video.read(currentFrameMat)) return false;
    currentFrame++;
    return true;
}

bool VideoManipulation::goToFrame(int frameNumber) {
    if (!video.isOpened()) return false;
    video.set(cv::CAP_PROP_POS_FRAMES, frameNumber);
    currentFrame = frameNumber;
    return video.read(currentFrameMat);
}

void VideoManipulation::setSpeedFactor(double factor) {
    speedFactor = factor;
}

QImage VideoManipulation::getCurrentFrameQImage() const {
    return matToQImage(currentFrameMat);
}

int VideoManipulation::getTotalFrames() const { return totalFrames; }
int VideoManipulation::getCurrentFrame() const { return currentFrame; }
double VideoManipulation::getSpeedFactor() const { return speedFactor; }

QImage VideoManipulation::matToQImage(const cv::Mat& mat) const {
    if (mat.empty()) return QImage();
    cv::Mat rgb;
    if (mat.channels() == 3)
        cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
    else
        cv::cvtColor(mat, rgb, cv::COLOR_GRAY2RGB);
    return QImage((const uchar*)rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888).copy();
}
