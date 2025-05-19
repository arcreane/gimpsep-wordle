#include "face_detection.h"

using namespace cv;
using namespace std;

void detectFacesCats(Mat& image,
    CascadeClassifier& face_cascade,
    CascadeClassifier& smile_cascade,
    CascadeClassifier& cat_cascade)
{
    // convertir l'image en niveaux de gris
    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);
    equalizeHist(gray, gray);  // améliorer le contraste avec une égalisation d'histogramme

    vector<Rect> faces; // détecter les visages dans l'image
    face_cascade.detectMultiScale(gray, faces, 1.1, 4, 0, Size(30, 30));

    for (const Rect& face : faces) {    // pour chaque visage détecté

        Mat faceROI = gray(face);  // extraire la région du visage

        vector<Rect> smiles;  // détecter les sourires dans le visage
        smile_cascade.detectMultiScale(faceROI, smiles, 1.7, 22, 0, Size(25, 25));

        // choisir la couleur et l'étiquette selon s'il y a un sourire ou non
        Scalar color = !smiles.empty() ? Scalar(0, 255, 255) : Scalar(0, 0, 255);
        string label = !smiles.empty() ? "Smile" : "Face";

        // dessiner un rectangle autour du visage
        rectangle(image, face, color, 2);
        putText(image, label, Point(face.x, face.y - 5), // afficher  au-dessus du visage
            FONT_HERSHEY_SIMPLEX, 0.5, color, 1);
    }

    // détecter les chats dans l'image
    vector<Rect> cats;
    cat_cascade.detectMultiScale(gray, cats, 1.1, 3, 0, Size(50, 50));

    
    for (const Rect& cat : cats) { // pour chaque chat détecté
        rectangle(image, cat, Scalar(0, 255, 0), 2);  // dessiner un rectangle autour du chat
        putText(image, "Cat", Point(cat.x, cat.y - 5),  // afficher "Cat" au-dessus
            FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 1);
    }
}
