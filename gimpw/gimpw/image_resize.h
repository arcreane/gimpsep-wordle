#ifndef IMAGE_RESIZE_H
#define IMAGE_RESIZE_H

#include <opencv2/opencv.hpp>
#include <QImage>

class ImageResizer {
public:
    void setImage(const cv::Mat& image);
    void resize(double scaleFactor);
    QImage getQImage() const;
    cv::Mat getResizedMat() const;
    void setScaleFactor(double scale); // <== dans public


private:
    cv::Mat originalImage;     // image originale non modifiée
    cv::Mat displayImage;      // image réduite pour affichage
    cv::Mat resizedImage;      // résultat final (à partir de displayImage)
    double displayScaleFactor = 1.0; // facteur appliqué à displayImage vs original
    double scaleFactor = 1.0;
};


#endif // IMAGE_RESIZE_H
