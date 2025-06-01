#include "object_detector.h"
#include <fstream>
#include <iostream>

using namespace cv;
using namespace dnn;
using namespace std;

ObjectDetector::ObjectDetector(const string& classFile, const string& configFile, const string& weightsFile)
    : classFile(classFile), configFile(configFile), weightsFile(weightsFile) {
}

void ObjectDetector::chargerClasses() {
    ifstream ifs(classFile);
    if (!ifs.is_open()) {
        cerr << "Erreur : impossible d'ouvrir le fichier des classes." << endl;
        return;
    }
    string line;
    while (getline(ifs, line)) {
        classes.push_back(line);
    }
}

bool ObjectDetector::charger() {
    chargerClasses();
    if (classes.empty()) return false;

    net = readNetFromDarknet(configFile, weightsFile);
    if (net.empty()) return false;

    net.setPreferableBackend(DNN_BACKEND_OPENCV);
    net.setPreferableTarget(DNN_TARGET_CPU);
    return true;
}

void ObjectDetector::detecter(Mat& image) {
    Mat blob;
    blobFromImage(image, blob, 1 / 255.0, Size(416, 416), Scalar(), true, false);
    net.setInput(blob);

    vector<Mat> sorties;
    net.forward(sorties, net.getUnconnectedOutLayersNames());

    float seuilConfiance = 0.5;
    float seuilNMS = 0.4;
    vector<int> indices;
    vector<Rect> boites;
    vector<float> confiances;
    vector<int> classesIds;

    for (auto& sortie : sorties) {
        for (int i = 0; i < sortie.rows; ++i) {
            float* data = (float*)sortie.ptr(i);
            float confidence = data[4];

            if (confidence > seuilConfiance) {
                float* scores = data + 5;
                int classId = max_element(scores, scores + classes.size()) - scores;
                float score = scores[classId];

                if (score > seuilConfiance) {
                    int centerX = (int)(data[0] * image.cols);
                    int centerY = (int)(data[1] * image.rows);
                    int width = (int)(data[2] * image.cols);
                    int height = (int)(data[3] * image.rows);
                    int left = centerX - width / 2;
                    int top = centerY - height / 2;

                    boites.push_back(Rect(left, top, width, height));
                    confiances.push_back(score);
                    classesIds.push_back(classId);
                }
            }
        }
    }

    dnn::NMSBoxes(boites, confiances, seuilConfiance, seuilNMS, indices);

    for (int idx : indices) {
        Rect box = boites[idx];
        rectangle(image, box, Scalar(0, 255, 0), 2);

        string label = classes[classesIds[idx]] + format(": %.2f", confiances[idx]);
        int baseline = 0;
        Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseline);
        int top = max(box.y, labelSize.height);
        rectangle(image, Point(box.x, top - labelSize.height - 10), Point(box.x + labelSize.width, top), Scalar(0, 255, 0), FILLED);
        putText(image, label, Point(box.x, top - 5), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0), 2);
    }
}

vector<string> ObjectDetector::getClasses() const {
    return classes;
}
