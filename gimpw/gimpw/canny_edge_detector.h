#ifndef CANNY_EDGE_DETECTOR_H
#define CANNY_EDGE_DETECTOR_H

#include <opencv2/opencv.hpp>
#include <QString>
#include <QImage>

using namespace std;
using namespace cv;

class CannyEdgeDetector {
    private:
        Mat image, edges;
        int lowerThreshold, upperThreshold;

    public:
        CannyEdgeDetector(const string& imagePath = "", int lowerT = 50, int upperT = 150);

        bool loadImage(const string& path);
        void setThresholds(int lowerT, int upperT);
        Mat computeEdges();
        QImage getEdgesImage() const;
        bool saveEdgesImage(const string& path);
};

#endif