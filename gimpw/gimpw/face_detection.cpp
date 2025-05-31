#include "face_detection.h"

using namespace cv;
using namespace std;

// charge files haar cascades
bool Detector::loadClassifiers(const string& face_xml,
    const string& smile_xml,
    const string& cat_xml) {
    return face_cascade.load(face_xml) &&
        smile_cascade.load(smile_xml) &&
        cat_cascade.load(cat_xml);
}

// apply detection on faces, smiles and cats

void Detector::detectFacesCats(Mat& image)
{
    Mat gray;  // convert the image on grey scale
    cvtColor(image, gray, COLOR_BGR2GRAY);
    equalizeHist(gray, gray);  // use histogram egalisation to get a better image

    vector<Rect> faces; // detect the faces in the images
    face_cascade.detectMultiScale(gray, faces, 1.1, 4, 0, Size(30, 30));

    for (const Rect& face : faces) {    // repeat for all faces detected

        Mat faceROI = gray(face);  // get the face region 

        vector<Rect> smiles;  // detect the smiles
        smile_cascade.detectMultiScale(faceROI, smiles, 1.7, 28, 0, Size(25, 25));
        Scalar color = !smiles.empty() ? Scalar(0, 255, 255) : Scalar(0, 0, 255); // choose the color for a smile if there is 
        string label = !smiles.empty() ? "Smile" : "Face";
        rectangle(image, face, color, 2); // draw a rectangular shae around the face 
        putText(image, label, Point(face.x, face.y - 5), // display 'face' above the face 
            FONT_HERSHEY_SIMPLEX, 0.5, color, 1);
    }

    vector<Rect> cats; // detect if there is cat in the image 
    cat_cascade.detectMultiScale(gray, cats, 1.1, 3, 0, Size(50, 50));

    for (const Rect& cat : cats) { // if cat detected 
        rectangle(image, cat, Scalar(0, 255, 0), 2);  // draw a rectangular shape around the cat 
        putText(image, "Cat", Point(cat.x, cat.y - 5),  // display 'cat' above 
            FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 1);
    }
}

// function calling, and cloning of the original image
Mat Detector::applyDetection(const Mat& input) {
    Mat image = input.clone(); // do a copy of the image to draw on it 
    detectFacesCats(image); // apply detection
    return image; // return the image modified
}
