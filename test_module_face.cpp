#include "detection_and_morphology.h"
#include "image_morphology.h"
#include "face_detection.h"

using namespace cv;
using namespace std;

cv::Mat applyDetection(const cv::Mat& input) {
    string face_xml = "data/haarcascades/haarcascade_frontalface_alt.xml";
    string smile_xml = "data/haarcascades/haarcascade_smile.xml";
    string cat_xml = "data/haarcascades/haarcascade_frontalcatface.xml";

    CascadeClassifier face_cascade, smile_cascade, cat_cascade;
    if (!face_cascade.load(face_xml) ||
        !smile_cascade.load(smile_xml) ||
        !cat_cascade.load(cat_xml)) {
        cerr << "Error loading Haar cascade files." << endl;
        return input.clone();
    }

    Mat image = input.clone();
    detectFacesCats(image, face_cascade, smile_cascade, cat_cascade);
    return image;
}

cv::Mat applyMorphologyGUI(const cv::Mat& input, int size, bool dilate) {
    return applyMorphology(input, size, dilate);
}
