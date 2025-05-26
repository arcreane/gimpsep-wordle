#include "image_morphology.h"
#include "face_detection.h"

using namespace cv;
using namespace std;

// apply complet detection (faces, smiles, cats)
cv::Mat applyDetection(const cv::Mat& input) {
    string face_xml = "data/haarcascades/haarcascade_frontalface_alt.xml";
    string smile_xml = "data/haarcascades/haarcascade_smile.xml";
    string cat_xml = "data/haarcascades/haarcascade_frontalcatface.xml";

    Detector detector;
    if (!detector.loadClassifiers(face_xml, smile_xml, cat_xml)) {
        cerr << "Error loading Haar cascade files." << endl;
        return input.clone();
    }

    return detector.applyDetection(input); // detect and return the modified image
}

// apply morphology from GUI 
cv::Mat applyMorphologyGUI(const cv::Mat& input, int size, bool dilate) {
    return Morphology::applyMorphology(input, size, dilate);
}
