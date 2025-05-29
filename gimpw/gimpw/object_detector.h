#ifndef OBJECT_DETECTOR_H
#define OBJECT_DETECTOR_H

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <string>
#include <vector>

class ObjectDetector {
public:
    ObjectDetector(const std::string& classFile, const std::string& configFile, const std::string& weightsFile);
    bool charger();
    void detecter(cv::Mat& image);
    std::vector<std::string> getClasses() const;

private:
    std::string classFile;
    std::string configFile;
    std::string weightsFile;
    std::vector<std::string> classes;
    cv::dnn::Net net;

    void chargerClasses();
};

#endif // OBJECT_DETECTOR_H
