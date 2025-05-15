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
#include <QSlider>
#include <QWidget>
#include <QFileDialog>
#include <QPixmap>
#include <QImage>
#include <QStackedWidget>
#include <QSpacerItem>
#include <opencv2/opencv.hpp>

#include "canny_edge_detector.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

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
        QLabel#TitleLabel {
            font-size: 32px;
            font-weight: bold;
            padding: 20px;
        }
    )";
    app.setStyleSheet(style);

    QWidget window;
    window.setWindowTitle("Traitement d'images - Qt");

    QStackedWidget* stackedWidget = new QStackedWidget;

    // ==== Page d'accueil ====
    QWidget* homePage = new QWidget;
    QVBoxLayout* homeLayout = new QVBoxLayout;
    QLabel* titleLabel = new QLabel("Menu Principal");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setObjectName("TitleLabel");

    QPushButton* goToCanny = new QPushButton("🔍 Canny Edge Detection");
    QPushButton* goToDetection = new QPushButton("🧬 Détection + Morphologie");

    QSpacerItem* spacerTop = new QSpacerItem(20, 100, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QSpacerItem* spacerBottom = new QSpacerItem(20, 100, QSizePolicy::Minimum, QSizePolicy::Expanding);

    homeLayout->addWidget(titleLabel);
    homeLayout->addItem(spacerTop);
    homeLayout->addWidget(goToCanny);
    homeLayout->addWidget(goToDetection);
    homeLayout->addItem(spacerBottom);
    homePage->setLayout(homeLayout);

    // ==== Page Canny ====
    QWidget* cannyPage = new QWidget;
    QVBoxLayout* mainLayout = new QVBoxLayout;

    QLabel* imageLabel = new QLabel;
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setStyleSheet("border: 1px solid #ccc; background: white;");
    imageLabel->setMinimumSize(600, 400);

    QLabel* statusLabel = new QLabel("Aucune image chargée");
    statusLabel->setAlignment(Qt::AlignCenter);

    QPushButton* loadButton = new QPushButton("📁 Charger une image");
    QPushButton* saveButton = new QPushButton("💾 Sauvegarder l'image");
    QPushButton* backButton = new QPushButton("⬅️ Retour au menu");

    QSlider* sliderT1 = new QSlider(Qt::Horizontal);
    QSlider* sliderT2 = new QSlider(Qt::Horizontal);
    sliderT1->setRange(0, 255);
    sliderT1->setValue(50);
    sliderT2->setRange(0, 255);
    sliderT2->setValue(150);

    QLabel* t1Label = new QLabel("Seuil 1 : 50");
    QLabel* t2Label = new QLabel("Seuil 2 : 150");

    CannyEdgeDetector detector;

    auto updateCanny = [&]() {
        detector.setThresholds(sliderT1->value(), sliderT2->value());
        detector.detectEdges();
        imageLabel->setPixmap(QPixmap::fromImage(detector.getPreview()).scaled(imageLabel->size(), Qt::KeepAspectRatio));
        statusLabel->setText("🔄 Mise à jour auto : seuils " + QString::number(sliderT1->value()) + ", " + QString::number(sliderT2->value()));
    };

    QObject::connect(loadButton, &QPushButton::clicked, [&]() {
        QString fileName = QFileDialog::getOpenFileName(&window, "Choisir une image", "", "Images (*.png *.jpg *.jpeg *.bmp)");
        if (!fileName.isEmpty()) {
            CannyEdgeDetector newDetector(fileName.toStdString(), sliderT1->value(), sliderT2->value());
            detector = newDetector;
            imageLabel->setPixmap(QPixmap::fromImage(QImage(fileName).scaled(imageLabel->size(), Qt::KeepAspectRatio)));
            statusLabel->setText("✅ Image chargée (affichage original)");
        }
    });

    QObject::connect(sliderT1, &QSlider::valueChanged, [&](int value) {
        t1Label->setText("Seuil 1 : " + QString::number(value));
        updateCanny();
    });

    QObject::connect(sliderT2, &QSlider::valueChanged, [&](int value) {
        t2Label->setText("Seuil 2 : " + QString::number(value));
        updateCanny();
    });

    QObject::connect(saveButton, &QPushButton::clicked, [&]() {
        QString filePath = QFileDialog::getSaveFileName(&window, "Enregistrer l'image", "", "Images (*.png *.jpg *.bmp)");
        if (!filePath.isEmpty() && detector.saveImage(filePath.toStdString())) {
            statusLabel->setText("💾 Image sauvegardée avec succès");
        }
    });

    QObject::connect(backButton, &QPushButton::clicked, [&]() {
        stackedWidget->setCurrentIndex(0);
    });

    mainLayout->addWidget(backButton);
    mainLayout->addWidget(loadButton);
    mainLayout->addWidget(saveButton);
    mainLayout->addWidget(t1Label);
    mainLayout->addWidget(sliderT1);
    mainLayout->addWidget(t2Label);
    mainLayout->addWidget(sliderT2);
    mainLayout->addWidget(imageLabel);
    mainLayout->addWidget(statusLabel);
    cannyPage->setLayout(mainLayout);

    // ==== Page Détection + Morphologie ====
    QWidget* detectionPage = new QWidget;
    QVBoxLayout* detectionLayout = new QVBoxLayout;
    QLabel* detectionLabel = new QLabel("Module Détection + Morphologie à implémenter");
    detectionLabel->setAlignment(Qt::AlignCenter);
    QPushButton* returnButton = new QPushButton("⬅️ Retour au menu");

    QObject::connect(returnButton, &QPushButton::clicked, [&]() {
        stackedWidget->setCurrentIndex(0);
    });

    detectionLayout->addWidget(detectionLabel);
    detectionLayout->addWidget(returnButton);
    detectionPage->setLayout(detectionLayout);

    stackedWidget->addWidget(homePage);     
    stackedWidget->addWidget(cannyPage);    
    stackedWidget->addWidget(detectionPage);

    QObject::connect(goToCanny, &QPushButton::clicked, [&]() {
        stackedWidget->setCurrentIndex(1);
    });

    QObject::connect(goToDetection, &QPushButton::clicked, [&]() {
        stackedWidget->setCurrentIndex(2);
    });

    window.setLayout(new QVBoxLayout);
    window.layout()->addWidget(stackedWidget);
    window.showMaximized();

    return app.exec();
}
