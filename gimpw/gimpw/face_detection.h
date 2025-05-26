#ifndef FACE_DETECTION_H
#define FACE_DETECTION_H

#include <opencv2/opencv.hpp>

class Detector {
private:
    cv::CascadeClassifier face_cascade;
    cv::CascadeClassifier smile_cascade;
    cv::CascadeClassifier cat_cascade;

public:
    // charge les fichiers Haar cascade
    bool loadClassifiers(const std::string& face_xml,
        const std::string& smile_xml,
        const std::string& cat_xml);

    // détection pour sourires, visages et chats
    void detectFacesCats(cv::Mat& image);

    // appliquer la détection complète sur une image
    cv::Mat applyDetection(const cv::Mat& input);
};

#endif
