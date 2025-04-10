#include "functions_fdetection_dilateErose.h"
using namespace cv;
using namespace std;

Mat morphImage(const Mat& input, int size, bool dilate)
{
    Mat output;
    Mat element = getStructuringElement(MORPH_RECT,
        Size(2 * size + 1, 2 * size + 1),
        Point(size, size));
    if (dilate)
        cv::dilate(input, output, element); //function for dilatation
    else
        cv::erode(input, output, element);// function for erosion
    return output;
}

void detectFacesCats(Mat& image,
    CascadeClassifier& face_cascade,
    CascadeClassifier& smile_cascade,
    CascadeClassifier& cat_cascade)
{
    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);
    equalizeHist(gray, gray);

   
    vector<Rect> faces; //detection for humain faces and if smile
    face_cascade.detectMultiScale(gray, faces, 1.1, 4, 0, Size(30, 30));

    for (const Rect& face : faces) {
        Mat faceROI = gray(face);
        vector<Rect> smiles;
        smile_cascade.detectMultiScale(faceROI, smiles, 1.7, 22, 0, Size(25, 25));

        if (!smiles.empty()) {
            rectangle(image, face, Scalar(0, 255, 255), 2);
            putText(image, "Smile", Point(face.x, face.y - 5),
                FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 255), 1);
        }
        else {
            rectangle(image, face, Scalar(0, 0, 255), 2);
            putText(image, "Face", Point(face.x, face.y - 5),
                FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 1);
        }
    }

    vector<Rect> cats; //detection for cats with xml haarcascade_frontalcatfac
    cat_cascade.detectMultiScale(gray, cats, 1.1, 3, 0, Size(50, 50));

    for (const Rect& cat : cats) {
        rectangle(image, cat, Scalar(0, 255, 0), 2);
        putText(image, "Cat", Point(cat.x, cat.y - 5),
            FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 1);
    }
}
