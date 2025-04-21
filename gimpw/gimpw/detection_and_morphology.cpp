#include "detection_and_morphology.h"
#include "image_morphology.h"
#include <iostream>
using namespace cv;
using namespace std;

void detectFacesCats(Mat& image,  // detect faces, smiles and cats
    CascadeClassifier& face_cascade,
    CascadeClassifier& smile_cascade,
    CascadeClassifier& cat_cascade)
{
    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);  // convert to grayscale
    equalizeHist(gray, gray);  // histogram equalization for better contrast

    vector<Rect> faces;
    face_cascade.detectMultiScale(gray, faces, 1.1, 4, 0, Size(30, 30));  // detect faces

    for (const Rect& face : faces) {
        Mat faceROI = gray(face);  // get face region
        vector<Rect> smiles;
        smile_cascade.detectMultiScale(faceROI, smiles, 1.7, 22, 0, Size(25, 25));  // detect smiles inside face

        if (!smiles.empty()) {
            rectangle(image, face, Scalar(0, 255, 255), 2);  // yellow rectangle for smile
            putText(image, "Smile", Point(face.x, face.y - 5),
                FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 255), 1);  // label: smile
        }
        else {
            rectangle(image, face, Scalar(0, 0, 255), 2);  // red rectangle for neutral face
            putText(image, "Face", Point(face.x, face.y - 5),
                FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 1);  // label: face
        }
    }

    vector<Rect> cats;
    cat_cascade.detectMultiScale(gray, cats, 1.1, 3, 0, Size(50, 50));  // detect cats

    for (const Rect& cat : cats) {
        rectangle(image, cat, Scalar(0, 255, 0), 2);  // green rectangle for cat
        putText(image, "Cat", Point(cat.x, cat.y - 5),
            FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 1);  // label: cat
    }
}
void runDetectionDilate() {
    string face_xml = "data/haarcascades/haarcascade_frontalface_alt.xml";
    string smile_xml = "data/haarcascades/haarcascade_smile.xml";
    string cat_xml = "data/haarcascades/haarcascade_frontalcatface.xml";

    CascadeClassifier face_cascade, smile_cascade, cat_cascade;
    if (!face_cascade.load(face_xml) ||
        !smile_cascade.load(smile_xml) ||
        !cat_cascade.load(cat_xml)) {
        cerr << "Error with the Haar cascade files." << endl;  // error loading xmls
        return;
    }
    string image_path;
    cout << "Enter the path to your image ! ";
    cin.ignore();  // clean input buffer
    getline(cin, image_path);  // get full image path

    Mat image = imread(image_path);
    if (image.empty()) {
        cerr << "Failed to load image." << endl;  // image not found or invalid
        return;
    }

    char option;
    cout << "Choose an operation:\n";
    cout << "1 - Detect Faces /Cats/ Smiles\n";
    cout << "2 - Apply Morphology (Dilation or Erosion)\n";
    cout << "Your choice: ";
    cin >> option;

    if (option == '1') {
        detectFacesCats(image, face_cascade, smile_cascade, cat_cascade);  // run detection
        imshow("Detection Result", image);  // show result
    }
    else if (option == '2') {
        int morph_size;
        char morph_type;
        cout << "Enter size (1 to 10): ";
        cin >> morph_size;
        cout << "Dilation (1) or Erosion (2)? ";
        cin >> morph_type;

        Mat result = applyMorphology(image, morph_size, (morph_type == '1'));  // run morphology
        imshow("Morphology Result", result);  // show result
    }
    else {
        cerr << "Option invalid." << endl;  // invalid option
        return;
    }
    waitKey(0);  // wait for key press
    destroyAllWindows();  // close all windows
}
