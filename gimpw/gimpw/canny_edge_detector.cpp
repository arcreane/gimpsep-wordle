#include "canny_edge_detector.h"

CannyEdgeDetector::CannyEdgeDetector(const string& path, int t1, int t2)
    : threshold1(t1), threshold2(t2)
{
    if (!path.empty()) {
        loadImage(path);
    }
}

bool CannyEdgeDetector::loadImage(const string& path) {
    image = imread(path);

    if (image.empty()) {
        cerr << "Error: Could not open or find the image!" << endl;
        return false;
    }
    
    return true;
}

void CannyEdgeDetector::setThresholds(int t1, int t2) {
    threshold1 = t1;
    threshold2 = t2;
}

Mat CannyEdgeDetector::detectEdges() {
    if (image.empty()) return Mat();
    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);
    Canny(gray, edges, threshold1, threshold2);
    return edges;
}

bool CannyEdgeDetector::saveImage(const string& path) {
    if (edges.empty()) return false;
    return imwrite(path, edges);
}

QImage CannyEdgeDetector::getPreview() const {
    if (edges.empty()) return QImage();
    return QImage(edges.data, edges.cols, edges.rows, edges.step, QImage::Format_Grayscale8).copy();
}