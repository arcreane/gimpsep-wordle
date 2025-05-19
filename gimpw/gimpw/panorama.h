#ifndef IMAGE_STITCHER_H
#define IMAGE_STITCHER_H

#include <opencv2/opencv.hpp>
#include <opencv2/stitching.hpp>
#include <QStringList>

//this class handles panorama stitching using OpenCV's Stitcher module
class ImageStitcher {
public:
    //constructor initializes the OpenCV stitcher
    ImageStitcher();

    //stitches a list of image paths from qt into a single panorama
    //returns true if successful or false otherwise
    bool stitchImages(const QStringList& filePaths, cv::Mat& output);

private:
    //the OpenCV stitcher instance
    cv::Ptr<cv::Stitcher> stitcher;
};

#endif *
