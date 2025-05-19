#ifndef BACKGROUND_DETECTOR_H
#define BACKGROUND_DETECTOR_H

#include <opencv2/opencv.hpp>
#include <QString>
#include <QImage>
#include <QPoint>
#include <QRect>

using namespace cv;
using namespace std;

class BackgroundDetector {
    private:
        Mat image, resultMask;
        Rect selectedRect;

    public:
        BackgroundDetector();

        bool loadImage(const QString& path);
        QImage getOriginalQImage() const;
        QImage getResultQImage() const;

        void setSelection(const QPoint& p1, const QPoint& p2);
        void apply();
        bool saveResult(const QString& path) const;

        QRect computeAutoSelection() const;
};

#endif 
