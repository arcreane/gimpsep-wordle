#include "face_detection.h"

using namespace cv;
using namespace std;

void detectFacesCats(Mat& image,
    CascadeClassifier& face_cascade,
    CascadeClassifier& smile_cascade,
    CascadeClassifier& cat_cascade)
{
    Mat gray;  // convert the image on grey scale
    cvtColor(image, gray, COLOR_BGR2GRAY);
    equalizeHist(gray, gray);  //use histogram egalisation to get a better image

    vector<Rect> faces; //detect the faces in the images
    face_cascade.detectMultiScale(gray, faces, 1.1, 4, 0, Size(30, 30));

    for (const Rect& face : faces) {    //repeat for all faces detected

        Mat faceROI = gray(face);  //get the face region 

        vector<Rect> smiles;  // detect the smiles
        smile_cascade.detectMultiScale(faceROI, smiles, 1.7, 22, 0, Size(25, 25));
        Scalar color = !smiles.empty() ? Scalar(0, 255, 255) : Scalar(0, 0, 255); // choose the color for a smile if there is 
        string label = !smiles.empty() ? "Smile" : "Face";
        rectangle(image, face, color, 2); //draw a rectangular shae around the face 
        putText(image, label, Point(face.x, face.y - 5), //display 'face' above the face 
            FONT_HERSHEY_SIMPLEX, 0.5, color, 1);
    }

    vector<Rect> cats; //detect if there is cat in the image 
    cat_cascade.detectMultiScale(gray, cats, 1.1, 3, 0, Size(50, 50));


    for (const Rect& cat : cats) { // if cat detected 
        rectangle(image, cat, Scalar(0, 255, 0), 2);  //draw a rectangular shape around the cat 
        putText(image, "Cat", Point(cat.x, cat.y - 5),  //display 'cat' above 
            FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 1);
    }
}

// apply detection on faces, smiles and cats
cv::Mat applyDetection(const cv::Mat& input) { //detection for smiles, faces and cats
    //paths for the haar files
    string face_xml = "data/haarcascades/haarcascade_frontalface_alt.xml";
    string smile_xml = "data/haarcascades/haarcascade_smile.xml";
    string cat_xml = "data/haarcascades/haarcascade_frontalcatface.xml";


    CascadeClassifier face_cascade, smile_cascade, cat_cascade; //haar classificator file
    if (!face_cascade.load(face_xml) ||
        !smile_cascade.load(smile_xml) ||
        !cat_cascade.load(cat_xml)) {
        cerr << "error loading haar cascade files" << endl;
        return input.clone(); // return image not modified in case of an error 
    }


    Mat image = input.clone(); // do a copy of the image to draw on it 
    detectFacesCats(image, face_cascade, smile_cascade, cat_cascade); //apply detection
    return image;// return the image modified
}