#ifndef BACKGROUND_SEPARATOR_H
#define BACKGROUND_SEPARATOR_H

#include <opencv2/opencv.hpp>
#include <QString>
#include <QImage>
#include <QPoint>
#include <QRect>

using namespace cv;
using namespace std;

class BackgroundSeparator {
private:
    Mat image, resultMask;
    Rect selectedRect;

public:
    BackgroundSeparator();

    bool loadImage(const QString& path);
    QImage getOriginalQImage() const;
    void computeAutoSelection();
    void apply();
    Mat createImageWithAlpha() const;
    QImage getResultQImage() const;
    bool saveResult(const QString& path) const;
};

#endif
