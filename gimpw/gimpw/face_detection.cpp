#include "face_detection.h"

using namespace cv;
using namespace std;

void detectFacesAndCats(Mat& image,  // detection of faces, smiles and cats 
    CascadeClassifier& face_cascade,
    CascadeClassifier& smile_cascade,
    CascadeClassifier& cat_cascade)
{
    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);  // convert to grayscale
    equalizeHist(gray, gray);  // improve contrast

    vector<Rect> faces;
    face_cascade.detectMultiScale(gray, faces, 1.1, 4, 0, Size(30, 30));  // detect faces

    for (const Rect& face : faces) {
        Mat faceROI = gray(face);  // region of interest: face
        vector<Rect> smiles;
        smile_cascade.detectMultiScale(faceROI, smiles, 1.7, 22, 0, Size(25, 25));  // detect smiles

        Scalar color = !smiles.empty() ? Scalar(0, 255, 255) : Scalar(0, 0, 255);  // yellow if smiling, red if not
        string label = !smiles.empty() ? "Smile" : "Face";  // label text

        rectangle(image, face, color, 2);  // draw rectangle around face
        putText(image, label, Point(face.x, face.y - 5),
            FONT_HERSHEY_SIMPLEX, 0.5, color, 1);  // display label
    }

    vector<Rect> cats;
    cat_cascade.detectMultiScale(gray, cats, 1.1, 3, 0, Size(50, 50));  // detect cats
    for (const Rect& cat : cats) {
        rectangle(image, cat, Scalar(0, 255, 0), 2);  // green rectangle for cats
        putText(image, "Cat", Point(cat.x, cat.y - 5),
            FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 1);  // label: cat
    }
}
