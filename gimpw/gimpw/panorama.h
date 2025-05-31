#ifndef PANORAMA_H
#define PANORAMA_H

#include <opencv2/opencv.hpp>
#include <opencv2/stitching.hpp>
#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace cv;

class Panorama {
private:
    vector<Mat> images;
    Mat panoramaResult;

public:
    bool loadImages(const string& path);
    size_t getImagesLoadedCount() const;
    void clearImages();
    bool computePanorama();
    Mat getPanoramaImage() const;
	bool savePanoramaImage(const string& path) const;
};

#endif
