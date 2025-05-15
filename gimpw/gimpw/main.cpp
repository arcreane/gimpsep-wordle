//#include "detection_and_morphology.h"
//#include "canny_edge_detector.h"
//#include <iostream>
//
//using namespace std;
//
//int main() {
//    char choice;
//
//    cout << "==== MAIN MENU ====\n";
//    cout << "1 - Face wDetection & Morphology Module\n";
//    cout << "2 - Face Recognition (Not available)\n";
//	cout << "3 - Canny Edge Detection\n";
//    cout << "Your choice: ";
//    cin >> choice;
//
//    switch (choice) {
//    case '1':
//        runDetectionDilate();
//        break;
//
//        // case '2':
//        //     runRecognition(); // Disabled until opencv_contrib is installed
//        //     break;
//
//	case '3': {
//		CannyEdgeDetector detector;
//		string imagePath;
//
//		cout << "Enter the path of the image you want to detect the edges: ";
//		cin >> imagePath;
//
//        detector.loadImage(imagePath);
//		detector.manipulateImage();
//        break;
//	}
//
//    default:
//        cout << "Invalid choice." << endl;
//        break;
//    }
//
//    return 0;
//}

#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QFileDialog>
#include <QPixmap>
#include <QImage>
#include <opencv2/opencv.hpp>

#include "detection_and_morphology.h"
#include "canny_edge_detector.h"

QImage matToQImage(const cv::Mat& mat) {
    if (mat.type() == CV_8UC3) {
        return QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888).rgbSwapped();
    }
    else if (mat.type() == CV_8UC1) {
        return QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);
    }
    return QImage();
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // 🌈 Style
    QString style = R"(
        QWidget {
            background-color: #f0f0f0;
            font-family: Segoe UI;
            font-size: 16px;
        }

        QPushButton {
            padding: 10px;
            font-weight: bold;
            border-radius: 8px;
            background-color: #3498db;
            color: white;
        }

        QPushButton:hover {
            background-color: #2980b9;
        }

        QLabel {
            color: #333;
        }
    )";
    app.setStyleSheet(style);

    // 🪟 Fenêtre
    QWidget window;
    window.setWindowTitle("Traitement d'images - OpenCV + Qt");

    QLabel* imageLabel = new QLabel;
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setStyleSheet("border: 1px solid #ccc; background: white;");
    imageLabel->setMinimumSize(600, 400);

    QLabel* statusLabel = new QLabel("Aucune image chargée");
    statusLabel->setAlignment(Qt::AlignCenter);

    QPushButton* loadButton = new QPushButton("📁 Charger une image");
    QPushButton* runDetectionButton = new QPushButton("🔍 Détection + Morphologie");
    QPushButton* runCannyButton = new QPushButton("🧠 Canny Edge");

    cv::Mat currentImage;

    // 📥 Charger image
    QObject::connect(loadButton, &QPushButton::clicked, [&]() {
        QString fileName = QFileDialog::getOpenFileName(&window, "Choisir une image", "", "Images (*.png *.jpg *.jpeg *.bmp)");
        if (!fileName.isEmpty()) {
            currentImage = cv::imread(fileName.toStdString());
            if (!currentImage.empty()) {
                QImage qimg = matToQImage(currentImage);
                imageLabel->setPixmap(QPixmap::fromImage(qimg).scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
                statusLabel->setText("✅ Image chargée");
            }
            else {
                statusLabel->setText("❌ Échec du chargement");
            }
        }
        });

    // 🧪 Détection + Morphologie
    QObject::connect(runDetectionButton, &QPushButton::clicked, [&]() {
        if (!currentImage.empty()) {
            cv::imwrite("temp_gui_input.jpg", currentImage);
            runDetectionDilate(); // lit "temp_gui_input.jpg", écrit "temp_gui_output.jpg"
            cv::Mat result = cv::imread("temp_gui_output.jpg");
            if (!result.empty()) {
                imageLabel->setPixmap(QPixmap::fromImage(matToQImage(result)).scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
                statusLabel->setText("✅ Détection + Morphologie appliquée");
            }
            else {
                statusLabel->setText("❌ Erreur : image de sortie manquante");
            }
        }
        else {
            statusLabel->setText("⚠️ Veuillez charger une image d'abord");
        }
        });

    // 🧠 Canny
    QObject::connect(runCannyButton, &QPushButton::clicked, [&]() {
        if (!currentImage.empty()) {
            CannyEdgeDetector detector;
            cv::imwrite("canny_gui_input.jpg", currentImage);
            detector.loadImage("canny_gui_input.jpg");
            detector.manipulateImage();
            cv::Mat result = cv::imread("canny_output.jpg");
            if (!result.empty()) {
                imageLabel->setPixmap(QPixmap::fromImage(matToQImage(result)).scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
                statusLabel->setText("✅ Canny appliqué");
            }
            else {
                statusLabel->setText("❌ Erreur : image de sortie manquante");
            }
        }
        else {
            statusLabel->setText("⚠️ Veuillez charger une image d'abord");
        }
        });

    // 🧱 Layouts
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(15);

    QHBoxLayout* topButtons = new QHBoxLayout;
    topButtons->addWidget(runDetectionButton);
    topButtons->addWidget(runCannyButton);

    mainLayout->addLayout(topButtons);
    mainLayout->addWidget(imageLabel);
    mainLayout->addWidget(statusLabel);
    mainLayout->addWidget(loadButton);

    window.setLayout(mainLayout);
    window.showMaximized();

    return app.exec();
}

