#include <opencv2/opencv.hpp>
#include "luminosite.h"

cv::Mat ajusterLuminosite(const cv::Mat& image, double facteur) {
    cv::Mat resultat;
    image.convertTo(resultat, -1, facteur, 0);
    return resultat;
}
