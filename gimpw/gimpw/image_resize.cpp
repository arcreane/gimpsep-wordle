#include "image_resize.h"
#include <opencv2/imgproc.hpp>
#include <QImage>
#include <iostream>  // pour debug

void ImageResizer::setImage(const cv::Mat& image) {
    originalImage = image.clone();  // Vraie image d'origine (non réduite)

    // Réduction uniquement pour affichage Qt
    int maxWidth = 800;
    int maxHeight = 600;
    displayImage = image.clone();

    if (image.cols > maxWidth || image.rows > maxHeight) {
        displayScaleFactor = std::min((double)maxWidth / image.cols, (double)maxHeight / image.rows);
        cv::resize(image, displayImage, cv::Size(), displayScaleFactor, displayScaleFactor);
    } else {
        displayScaleFactor = 1.0;
    }

    resizedImage = displayImage.clone();
}


void ImageResizer::resize(double factor) {
    scaleFactor = factor;  // sauvegarde du facteur de redimensionnement pour getResizedMat

    if (displayImage.empty()) return;

    // Redimensionne uniquement pour l'affichage dans la GUI
    cv::resize(displayImage, resizedImage, cv::Size(), factor, factor);
}


QImage ImageResizer::getQImage() const {
    if (resizedImage.empty()) return QImage();
    cv::Mat rgb;
    cv::cvtColor(resizedImage, rgb, cv::COLOR_BGR2RGB);
    return QImage(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888).copy();
}

cv::Mat ImageResizer::getResizedMat() const {
    double totalFactor = scaleFactor;
    cv::Mat final;
    cv::resize(originalImage, final, cv::Size(), totalFactor, totalFactor);
    return final;
}


void ImageResizer::setScaleFactor(double scale) {
    scaleFactor = scale;
}
