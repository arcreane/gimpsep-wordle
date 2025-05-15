#include "detection_and_morphology.h"
#include "canny_edge_detector.h"
#include "background_detector.h" 

int main() {
    char choice;

    cout << "==== MAIN MENU ====\n";
    cout << "1 - Face Detection & Morphology Module\n";
    cout << "2 - Face Recognition (Not available)\n";
    cout << "3 - Canny Edge Detection\n";
    cout << "4 - Background Separation\n"; 
    cout << "Your choice: ";
    cin >> choice;

    switch (choice) {
    case '1':
        runDetectionDilate();
        break;

        // case '2':
        //     runRecognition(); // Disabled until opencv_contrib is installed
        //     break;

    case '3': {
        CannyEdgeDetector detector;
        string imagePath;

        cout << "Enter the path of the image you want to detect the edges: ";
        cin >> imagePath;

        if (detector.loadImage(imagePath)) {
            detector.manipulateImage();
        }
        break;
    }

    case '4': {
        runBackgroundDetection(); 
        break;
    }

    default:
        cout << "Invalid choice." << endl;
        break;
    }

    return 0;
}
