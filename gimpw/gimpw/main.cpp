#include "detection_and_morphology.h"
#include "canny_edge_detector.h"
#include <iostream>

using namespace std;

int main() {
    char choice;

    cout << "==== MAIN MENU ====\n";
    cout << "1 - Face wDetection & Morphology Module\n";
    cout << "2 - Face Recognition (Not available)\n";
	cout << "3 - Canny Edge Detection\n";
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

        detector.loadImage(imagePath);
		detector.manipulateImage();
        break;
	}

    default:
        cout << "Invalid choice." << endl;
        break;
    }

    return 0;
}
