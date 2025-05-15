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
        int threshold1, threshold2;

    public:
        CannyEdgeDetector(const string& imagePath = "", int t1 = 50, int t2 = 150);

        void setThresholds(int t1, int t2);
        bool loadImage(const string& path);
        bool saveImage(const string& path);
        Mat detectEdges();
        QImage getPreview() const;
};

#endif