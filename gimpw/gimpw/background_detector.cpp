#include "background_detector.h"

BackgroundDetector::BackgroundDetector() {}

bool BackgroundDetector::loadImage(const QString& path) {
    image = imread(path.toStdString(), IMREAD_UNCHANGED);

    if (image.empty()) {
        cerr << "Error: Could not open or find the image!" << endl;
        return false;
    }

    return true;
}

QImage BackgroundDetector::getOriginalQImage() const {
    if (image.empty()) return QImage();
    Mat rgb;
    cvtColor(image, rgb, COLOR_BGR2RGB);
    return QImage(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888).copy();
}

QImage BackgroundDetector::getResultQImage() const {
    if (image.empty() || resultMask.empty()) return QImage();
    Mat foreground;
    image.copyTo(foreground, resultMask);

    Mat alpha;
    resultMask.copyTo(alpha);

    std::vector<Mat> bgrChannels;
    split(foreground, bgrChannels);
    bgrChannels.push_back(alpha);

    Mat outputWithAlpha;
    merge(bgrChannels, outputWithAlpha);

    Mat rgba;
    cvtColor(outputWithAlpha, rgba, COLOR_BGRA2RGBA);
    return QImage(rgba.data, rgba.cols, rgba.rows, rgba.step, QImage::Format_RGBA8888).copy();
}

void BackgroundDetector::setSelection(const QPoint& p1, const QPoint& p2) {
    selectedRect = Rect(Point(p1.x(), p1.y()), Point(p2.x(), p2.y()));
}

void BackgroundDetector::apply() {
    if (image.empty() || selectedRect.width <= 0 || selectedRect.height <= 0) return;

    Mat bgModel, fgModel;
    Mat mask(image.size(), CV_8UC1, Scalar(GC_BGD));
    mask(selectedRect).setTo(Scalar(GC_PR_FGD));

    grabCut(image, mask, selectedRect, bgModel, fgModel, 5, GC_INIT_WITH_RECT);
    resultMask = (mask == GC_FGD) | (mask == GC_PR_FGD);
    resultMask.convertTo(resultMask, CV_8UC1, 255);
}

bool BackgroundDetector::saveResult(const QString& path) const {
    if (image.empty() || resultMask.empty()) return false;

    Mat foreground;
    image.copyTo(foreground, resultMask);

    Mat alpha;
    resultMask.copyTo(alpha);

    std::vector<Mat> bgrChannels;
    split(foreground, bgrChannels);
    bgrChannels.push_back(alpha);

    Mat outputWithAlpha;
    merge(bgrChannels, outputWithAlpha);

    return imwrite(path.toStdString(), outputWithAlpha);
}

QRect BackgroundDetector::computeAutoSelection() const {
    if (image.empty()) return QRect();

    Mat gray, binary;
    cvtColor(image, gray, COLOR_BGR2GRAY);
    threshold(gray, binary, 10, 255, THRESH_BINARY);

    std::vector<std::vector<Point>> contours;
    findContours(binary, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    if (contours.empty()) return QRect();

    Rect bbox = boundingRect(contours[0]);
    for (size_t i = 1; i < contours.size(); ++i) {
        bbox |= boundingRect(contours[i]);
    }

    return QRect(bbox.x, bbox.y, bbox.width, bbox.height);
}
