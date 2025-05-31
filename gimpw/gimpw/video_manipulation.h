#ifndef VIDEO_MANIPULATION_H
#define VIDEO_MANIPULATION_H

#include <opencv2/opencv.hpp>
#include <QImage>

class VideoManipulation {
public:
    bool loadVideo(const std::string& path);
    bool nextFrame();
    bool goToFrame(int frameNumber);
    void setSpeedFactor(double factor);

    QImage getCurrentFrameQImage() const;
    int getTotalFrames() const;
    int getCurrentFrame() const;
    double getSpeedFactor() const;

    bool saveVideo(const std::string& outputPath);


private:
    cv::VideoCapture video;
    cv::Mat currentFrameMat;
    int totalFrames = 0;
    int currentFrame = 0;
    double speedFactor = 1.0;

    QImage matToQImage(const cv::Mat& mat) const;
};

#endif
