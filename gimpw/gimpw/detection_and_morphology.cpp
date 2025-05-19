#include "detection_and_morphology.h"
#include "image_morphology.h"
#include "face_detection.h"

using namespace cv;
using namespace std;

// applique la détection des visages, sourires et chats sur l'image
cv::Mat applyDetection(const cv::Mat& input) {
    // chemins vers les fichiers cascade haar
    string face_xml = "data/haarcascades/haarcascade_frontalface_alt.xml";
    string smile_xml = "data/haarcascades/haarcascade_smile.xml";
    string cat_xml = "data/haarcascades/haarcascade_frontalcatface.xml";

    // chargement des classificateurs haar
    CascadeClassifier face_cascade, smile_cascade, cat_cascade;
    if (!face_cascade.load(face_xml) ||
        !smile_cascade.load(smile_xml) ||
        !cat_cascade.load(cat_xml)) {
        cerr << "error loading haar cascade files" << endl;
        return input.clone(); // retourne une copie inchangé en cas d'erreurs
    }

    
    Mat image = input.clone(); // faire une copie de l'image pour dessiner dessus
    detectFacesCats(image, face_cascade, smile_cascade, cat_cascade); // appliquer la détection sur l'image
    return image;// retourner l'image modifiée
}

// applique une opération morphologique (dilatatio/érosion)
cv::Mat applyMorphologyGUI(const cv::Mat& input, int size, bool dilate) {
    return applyMorphology(input, size, dilate); // utiliser la fonction déjà existante
}
