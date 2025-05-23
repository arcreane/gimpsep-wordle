#include "background_separator.h"

// constructor: initializes the background separator object
BackgroundSeparator::BackgroundSeparator() {}

// loads an image from the given path 
bool BackgroundSeparator::loadImage(const QString& path) {
    image = imread(path.toStdString(), IMREAD_UNCHANGED);

    if (image.empty()) {
        cerr << "Error: Could not open or find the image!" << endl;
        return false;
    }

    return true;
}

// returns the original image as a QImage (converted from BGR to RGB)
QImage BackgroundSeparator::getOriginalQImage() const {
    if (image.empty()) return QImage();
    Mat rgb;
    cvtColor(image, rgb, COLOR_BGR2RGB);
    return QImage(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888).copy();
}

// automatically computes a rectangular selection around the main foreground object
void BackgroundSeparator::computeAutoSelection() {
    if (image.empty()) {
        selectedRect = Rect();
        return;
    }

    Mat gray, binary;
    cvtColor(image, gray, COLOR_BGR2GRAY);
    threshold(gray, binary, 10, 255, THRESH_BINARY);

    std::vector<std::vector<Point>> contours;
    findContours(binary, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    if (contours.empty()) {
        selectedRect = Rect();
        return;
    }

    Rect bbox = boundingRect(contours[0]);
    for (size_t i = 1; i < contours.size(); ++i) {
        bbox |= boundingRect(contours[i]);
    }

    selectedRect = bbox;
}

// applies the grabCut algorithm to separate the foreground from the background
void BackgroundSeparator::apply() {
    if (image.empty() || selectedRect.width <= 0 || selectedRect.height <= 0) return;

    Mat bgModel, fgModel;
    Mat mask(image.size(), CV_8UC1, Scalar(GC_BGD));
    mask(selectedRect).setTo(Scalar(GC_PR_FGD));

    grabCut(image, mask, selectedRect, bgModel, fgModel, 5, GC_INIT_WITH_RECT);
    resultMask = (mask == GC_FGD) | (mask == GC_PR_FGD);
    resultMask.convertTo(resultMask, CV_8UC1, 255);
}

// constructs a Mat with alpha channel from the original image and result mask
Mat BackgroundSeparator::createImageWithAlpha() const {
    Mat foreground;
    image.copyTo(foreground, resultMask);

    Mat alpha;
    resultMask.copyTo(alpha);

    std::vector<Mat> bgrChannels;
    split(foreground, bgrChannels);
    bgrChannels.push_back(alpha);

    Mat outputWithAlpha;
    merge(bgrChannels, outputWithAlpha);

    return outputWithAlpha;
}

// returns the processed image with background removed and alpha channel added
QImage BackgroundSeparator::getResultQImage() const {
    if (image.empty() || resultMask.empty()) return QImage();

    Mat rgba;
    cvtColor(createImageWithAlpha(), rgba, COLOR_BGRA2RGBA);
    return QImage(rgba.data, rgba.cols, rgba.rows, rgba.step, QImage::Format_RGBA8888).copy();
}

// saves the result image (with transparency) to the specified path
bool BackgroundSeparator::saveResult(const QString& path) const {
    if (image.empty() || resultMask.empty()) return false;

    return imwrite(path.toStdString(), createImageWithAlpha());
}
