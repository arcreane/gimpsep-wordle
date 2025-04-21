#include "detection_and_morphology.h"
#include <iostream>

using namespace std;

int main() {
    char choice;

    cout << "==== MAIN MENU ====\n";
    cout << "1 - Face wDetection & Morphology Module\n";
    cout << "2 - Face Recognition (Not available)\n";
    cout << "Your choice: ";
    cin >> choice;

    switch (choice) {
    case '1':
        runDetectionDilate();
        break;

        // case '2':
        //     runRecognition(); // Disabled until opencv_contrib is installed
        //     break;

    default:
        cout << "Invalid choice." << endl;
        break;
    }

    return 0;
}
