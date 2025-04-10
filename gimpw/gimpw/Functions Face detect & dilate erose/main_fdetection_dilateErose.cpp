#include <iostream>
#include <opencv2/opencv.hpp>
#include "functions_fdetection_dilateErose.h"

using namespace std;
using namespace cv;

int main()
{
    string face_xml = "data/haarcascades/haarcascade_frontalface_alt.xml";
    string smile_xml = "data/haarcascades/haarcascade_smile.xml";
    string cat_xml = "data/haarcascades/haarcascade_frontalcatface.xml";




    CascadeClassifier face_cascade, smile_cascade, cat_cascade;
    if (!face_cascade.load(face_xml) ||
        !smile_cascade.load(smile_xml) ||
        !cat_cascade.load(cat_xml)) {
        cerr << "Error in functions of face detection." << endl; //if file not found
        return -1;
    }

    string image_path;
    cout << "Enter the path of your image! "; //copy path of the image we change
    getline(cin, image_path);

    Mat image = imread(image_path);
    if (image.empty()) {
        cerr << "Error upload image." << endl; //tell if error 
        return -1;
    }
    char operation;
    cout << "What do we do ? (1: Detection of being-- 2: Erosion/Dilatation): "; // type of function we want to do 
    cin >> operation;

    if (operation == '2') { // erosion or dilate
        int morph_size;
        char choice;
        cout << "Size of the erosion or dilatation ? (1-10): ";
        cin >> morph_size;
        cout << "Dilatation (D) or Erosion (E)? (D/E): ";
        cin >> choice;

        Mat result = morphImage(image, morph_size, (choice == 'D' || choice == 'd'));
        imshow("The result :", result);
    }
    else if (operation == '1') { //face, cats, smile detection
        detectFacesCats(image, face_cascade, smile_cascade, cat_cascade);
        imshow("Detection of being. Humain, cats.", image);
    }
    else {
        cerr << "Invalid" << endl;
        return -1;
    }
    waitKey(0);
    destroyAllWindows();

    return 0;
}
