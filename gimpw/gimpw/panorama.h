#ifndef PANORAMA_H
#define PANORAMA_H

#include <opencv2/opencv.hpp>
#include <vector>

//class to assemble multiple images for panorama 
class Panorama {
public:
    // stiching function from images data base 
    static cv::Mat stitchImages(const std::vector<cv::Mat>& images, bool& success);
};

#endif
