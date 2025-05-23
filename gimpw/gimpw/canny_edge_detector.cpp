#include "canny_edge_detector.h"

// constructor: initializes thresholds and optionally loads an image
CannyEdgeDetector::CannyEdgeDetector(const string& path, int lowerT, int upperT)
    : lowerThreshold(lowerT), upperThreshold(upperT)
{
    if (!path.empty()) {
        loadImage(path);
    }
}

// loads an image from the given file path
bool CannyEdgeDetector::loadImage(const string& path) {
    image = imread(path);

    if (image.empty()) {
        cerr << "Error: Could not open or find the image!" << endl;
        return false;
    }

    return true;
}

// sets the lower and upper thresholds used for edge detection
void CannyEdgeDetector::setThresholds(int lowerT, int upperT) {
    lowerThreshold = lowerT;
    upperThreshold = upperT;
}

// computes the edge map using the canny edge detection algorithm
Mat CannyEdgeDetector::computeEdges() {
    if (image.empty()) return Mat();
    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);
    Canny(gray, edges, lowerThreshold, upperThreshold);
    return edges;
}

// returns the current edge map as a QImage in grayscale format
QImage CannyEdgeDetector::getEdgesImage() const {
    if (edges.empty()) return QImage();
    return QImage(edges.data, edges.cols, edges.rows, edges.step, QImage::Format_Grayscale8).copy();
}

// saves the current edge map to the specified file path
bool CannyEdgeDetector::saveEdgesImage(const string& path) {
    if (edges.empty()) return false;
    return imwrite(path, edges);
}
