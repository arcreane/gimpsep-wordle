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
#include <QSizePolicy>
#include <QMovie>
#include <opencv2/opencv.hpp>

#include "canny_edge_detector.h"
#include "image_morphology.h"
#include "background_separator.h"
#include "image_resize.h"
#include "face_detection.h"
#include "video_manipulation.h"

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
    window.setWindowTitle("GIMPW");

    QStackedWidget* stackedWidget = new QStackedWidget;

    // ==== Home Page ====
    QWidget* homePage = new QWidget;
    QVBoxLayout* homeLayout = new QVBoxLayout;
    QLabel* titleLabel = new QLabel("Welcome to GIMPW, choose a fonctionnality !");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setObjectName("TitleLabel");

    QPushButton* goToCannyEdgeDetectionPage = new QPushButton("🔍 Canny Edge Detection");
    QPushButton* goToMorphologyPage = new QPushButton("🧬 Morphology");
    QPushButton* goToBackgroundSeparationPage = new QPushButton("🖼️ Background Separation");
    QPushButton* goToResizingPage = new QPushButton("↔️ Image Resizing");
    QPushButton* goToFaceDetectionPage = new QPushButton("😊 Face Detection");
    QPushButton* goToVideoManipulationPage = new QPushButton("🎞️ Video Manipulation");

    QSpacerItem* spacerTop = new QSpacerItem(20, 100, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QSpacerItem* spacerBottom = new QSpacerItem(20, 100, QSizePolicy::Minimum, QSizePolicy::Expanding);

    homeLayout->addWidget(titleLabel);
    homeLayout->addItem(spacerTop);
    homeLayout->addWidget(goToCannyEdgeDetectionPage);
    homeLayout->addWidget(goToMorphologyPage);
    homeLayout->addWidget(goToBackgroundSeparationPage);
    homeLayout->addWidget(goToResizingPage);
    homeLayout->addWidget(goToFaceDetectionPage);
    homeLayout->addWidget(goToVideoManipulationPage);
    homeLayout->addItem(spacerBottom);
    homePage->setLayout(homeLayout);

    // ==== Canny Edge Detection Page ====
    QWidget* cannyEdgeDetectionPage = new QWidget;
    QVBoxLayout* cannyEdgeDetectionLayout = new QVBoxLayout;
    QLabel* cannyEdgeDetectionImageLabel = new QLabel;

    cannyEdgeDetectionImageLabel->setAlignment(Qt::AlignCenter);
    cannyEdgeDetectionImageLabel->setMinimumSize(600, 400);
    cannyEdgeDetectionImageLabel->setStyleSheet("border: 1px solid #ccc; background: white;");

    QLabel* cannyEdgeDetectionStatusLabel = new QLabel("No image loaded");
    cannyEdgeDetectionStatusLabel->setAlignment(Qt::AlignCenter);

    QPushButton* cannyEdgeDetectionLoadButton = new QPushButton("📁 Load image");
    QPushButton* cannyEdgeDetectionSaveButton = new QPushButton("💾 Save image");
    QPushButton* cannyEdgeDetectionBackButton = new QPushButton("⬅️ Return to menu");

    QSlider* lowerTSlider = new QSlider(Qt::Horizontal);
    QSlider* upperTSlider = new QSlider(Qt::Horizontal);

    lowerTSlider->setRange(0, 255);
    lowerTSlider->setValue(50);
    upperTSlider->setRange(0, 255);
    upperTSlider->setValue(150);

    QLabel* lowerTLabel = new QLabel("Seuil 1 : 50");
    QLabel* upperTLabel = new QLabel("Seuil 2 : 150");

    CannyEdgeDetector detector;

    auto updateCanny = [&]() {
        detector.setThresholds(lowerTSlider->value(), upperTSlider->value());
        detector.computeEdges();
        cannyEdgeDetectionImageLabel->setPixmap(QPixmap::fromImage(detector.getEdgesImage()).scaled(cannyEdgeDetectionImageLabel->size(), Qt::KeepAspectRatio));
        cannyEdgeDetectionStatusLabel->setText("🔄 Automatic update : thresholds " + QString::number(lowerTSlider->value()) + ", " + QString::number(upperTSlider->value()));
    };

    QObject::connect(cannyEdgeDetectionLoadButton, &QPushButton::clicked, [&]() {
        QString fileName = QFileDialog::getOpenFileName(&window, "Load image", "", "Images (*.png *.jpg *.jpeg *.bmp)");
        if (!fileName.isEmpty()) {
            CannyEdgeDetector newDetector(fileName.toStdString(), lowerTSlider->value(), upperTSlider->value());
            detector = newDetector;
            cannyEdgeDetectionImageLabel->setPixmap(QPixmap::fromImage(QImage(fileName).scaled(cannyEdgeDetectionImageLabel->size(), Qt::KeepAspectRatio)));
            cannyEdgeDetectionStatusLabel->setText("✅ Image correctly loaded");
        }
        else {
            cannyEdgeDetectionStatusLabel->setText("❌ Something went wrong with the loading of the image");
        }
    });

    QObject::connect(lowerTSlider, &QSlider::valueChanged, [&](int value) {
        lowerTLabel->setText("Threshold 1 : " + QString::number(value));
        updateCanny();
    });

    QObject::connect(upperTSlider, &QSlider::valueChanged, [&](int value) {
        upperTLabel->setText("Threshold 2 : " + QString::number(value));
        updateCanny();
    });

    QObject::connect(cannyEdgeDetectionSaveButton, &QPushButton::clicked, [&]() {
        QString filePath = QFileDialog::getSaveFileName(&window, "Save image", "", "Images (*.png *.jpg *.bmp)");
        if (!filePath.isEmpty() && detector.saveEdgesImage(filePath.toStdString())) {
            cannyEdgeDetectionStatusLabel->setText("💾 Image correctly saved");
        }
        else {
            cannyEdgeDetectionStatusLabel->setText("❌ Something went wrong with the saving of the image");
        }
    });

    QObject::connect(cannyEdgeDetectionBackButton, &QPushButton::clicked, [&]() {
        stackedWidget->setCurrentIndex(0);
    });

    cannyEdgeDetectionLayout->addWidget(cannyEdgeDetectionBackButton);
    cannyEdgeDetectionLayout->addWidget(cannyEdgeDetectionLoadButton);
    cannyEdgeDetectionLayout->addWidget(cannyEdgeDetectionSaveButton);
    cannyEdgeDetectionLayout->addWidget(lowerTLabel);
    cannyEdgeDetectionLayout->addWidget(lowerTSlider);
    cannyEdgeDetectionLayout->addWidget(upperTLabel);
    cannyEdgeDetectionLayout->addWidget(upperTSlider);
    cannyEdgeDetectionLayout->addWidget(cannyEdgeDetectionImageLabel);
    cannyEdgeDetectionLayout->addWidget(cannyEdgeDetectionStatusLabel);
    cannyEdgeDetectionPage->setLayout(cannyEdgeDetectionLayout);

    // ==== Morphology Page ====
    QWidget* morphologyPage = new QWidget;
    QVBoxLayout* morphologyLayout = new QVBoxLayout;
    QLabel* morphologyImageLabel = new QLabel;

    morphologyImageLabel->setAlignment(Qt::AlignCenter);
    morphologyImageLabel->setMinimumSize(600, 400);
    morphologyImageLabel->setStyleSheet("border: 1px solid #ccc; background: white;");

    QLabel* morphologyStatusLabel = new QLabel("No image loaded");
    morphologyStatusLabel->setAlignment(Qt::AlignCenter);

    QPushButton* morphologyLoadButton = new QPushButton("📁 Load image");
    QPushButton* morphologyDilateButton = new QPushButton("➕ Apply Dilation");
    QPushButton* morphologyErodeButton = new QPushButton("➖ Apply Erosion");
    QPushButton* morphologyBackButton = new QPushButton("⬅️ Return to menu");

    QSlider* morphologySlider = new QSlider(Qt::Horizontal);

    morphologySlider->setRange(1, 20);
    morphologySlider->setValue(2);

    QLabel* morphologyKernelLabel = new QLabel("Kernel size: 2");

    Mat morphologyOriginalImage;
    Mat morphologyResult;

    auto updateMorphology = [&](bool dilate) {
        if (morphologyOriginalImage.empty()) {
            morphologyStatusLabel->setText("❗ No image loaded");
            return;
        }
        int size = morphologySlider->value();
        morphologyResult = applyMorphology(morphologyOriginalImage, size, dilate);
        QImage qimg((uchar*)morphologyResult.data, morphologyResult.cols, morphologyResult.rows, morphologyResult.step, QImage::Format_BGR888);
        morphologyImageLabel->setPixmap(QPixmap::fromImage(qimg).scaled(morphologyImageLabel->size(), Qt::KeepAspectRatio));
        morphologyStatusLabel->setText(dilate ? "✅ Dilation applied" : "✅ Erosion applied");
    };

    QObject::connect(morphologyLoadButton, &QPushButton::clicked, [&]() {
        QString path = QFileDialog::getOpenFileName(&window, "Load image", "", "Images (*.png *.jpg *.bmp)");
        if (!path.isEmpty()) {
            morphologyOriginalImage = imread(path.toStdString());
            if (!morphologyOriginalImage.empty()) {
                QImage qimg((uchar*)morphologyOriginalImage.data, morphologyOriginalImage.cols, morphologyOriginalImage.rows, morphologyOriginalImage.step, QImage::Format_BGR888);
                morphologyImageLabel->setPixmap(QPixmap::fromImage(qimg).scaled(morphologyImageLabel->size(), Qt::KeepAspectRatio));
                morphologyStatusLabel->setText("✅ Image loaded");
            }
            else {
                morphologyStatusLabel->setText("❌ Failed to load image");
            }
        }
    });

    QObject::connect(morphologyDilateButton, &QPushButton::clicked, [&]() {
        updateMorphology(true);
    });

    QObject::connect(morphologyErodeButton, &QPushButton::clicked, [&]() {
        updateMorphology(false);
    });

    QObject::connect(morphologySlider, &QSlider::valueChanged, [&](int value) {
        morphologyKernelLabel->setText("Kernel size: " + QString::number(value));
    });

    QObject::connect(morphologyBackButton, &QPushButton::clicked, [&]() {
        stackedWidget->setCurrentIndex(0);
    });

    morphologyLayout->addWidget(morphologyBackButton);
    morphologyLayout->addWidget(morphologyLoadButton);
    morphologyLayout->addWidget(morphologyDilateButton);
    morphologyLayout->addWidget(morphologyErodeButton);
    morphologyLayout->addWidget(morphologyKernelLabel);
    morphologyLayout->addWidget(morphologySlider);
    morphologyLayout->addWidget(morphologyImageLabel);
    morphologyLayout->addWidget(morphologyStatusLabel);
    morphologyPage->setLayout(morphologyLayout);

    // ==== Background Separation Page ====
    QWidget* backgroundSeparationPage = new QWidget;
    QVBoxLayout* backgroundSeparationLayout = new QVBoxLayout;
    QLabel* backgroundSeparationImageLabel = new QLabel("No image loaded");

    backgroundSeparationImageLabel->setAlignment(Qt::AlignCenter);
    backgroundSeparationImageLabel->setMinimumSize(600, 400);
    backgroundSeparationImageLabel->setStyleSheet("border: 1px solid #ccc; background: white;");

    QPushButton* backgroundSeparationLoadButton = new QPushButton("📁 Load image");
    QPushButton* backgroundSeparationApplyButton = new QPushButton("🎯 Apply background separation");
    QPushButton* backgroundSeparationSaveButton = new QPushButton("💾 Save image");
    QPushButton* backgroundSeparationBackButton = new QPushButton("⬅️ Return to menu");

    QLabel* backgroundSeparationStatusLabel = new QLabel("State: ready");
    backgroundSeparationStatusLabel->setAlignment(Qt::AlignCenter);

    BackgroundSeparator backgroundSeparator;
    QImage backgroundSeparationLoadedImage;

    QObject::connect(backgroundSeparationLoadButton, &QPushButton::clicked, [&]() {
        QString path = QFileDialog::getOpenFileName(&window, "Load image", "", "Images (*.png *.jpg *.bmp)");
        if (!path.isEmpty() && backgroundSeparator.loadImage(path)) {
            backgroundSeparationLoadedImage = backgroundSeparator.getOriginalQImage();
            backgroundSeparationImageLabel->setPixmap(QPixmap::fromImage(backgroundSeparationLoadedImage).scaled(backgroundSeparationImageLabel->size(), Qt::KeepAspectRatio));
            backgroundSeparationStatusLabel->setText("✅ Image correctly loaded");
        }
        else {
            backgroundSeparationStatusLabel->setText("❌ Something went wrong with the loading of the image");
        }
    });

    QObject::connect(backgroundSeparationApplyButton, &QPushButton::clicked, [&]() {
        backgroundSeparationStatusLabel->setText("⏳ Processing background separation...");
        app.processEvents();

        backgroundSeparator.computeAutoSelection();
        backgroundSeparator.apply();

        QImage result = backgroundSeparator.getResultQImage();
        if (!result.isNull()) {
            backgroundSeparationImageLabel->setPixmap(QPixmap::fromImage(result).scaled(backgroundSeparationImageLabel->size(), Qt::KeepAspectRatio));
            backgroundSeparationStatusLabel->setText("🎯 Background separation applied");
        }
        else {
            backgroundSeparationStatusLabel->setText("❗Something went wrong with the background separation");
        }
    });

    QObject::connect(backgroundSeparationSaveButton, &QPushButton::clicked, [&]() {
        QString path = QFileDialog::getSaveFileName(&window, "Save image", "", "Images (*.png)");
        if (!path.isEmpty() && backgroundSeparator.saveResult(path)) {
            backgroundSeparationStatusLabel->setText("💾 Image correctly saved");
        }
        else {
            backgroundSeparationStatusLabel->setText("❌ Something went wrong with the saving of the image");
        }
    });

    QObject::connect(backgroundSeparationBackButton, &QPushButton::clicked, [&]() {
        stackedWidget->setCurrentIndex(0);
    });

    backgroundSeparationLayout->addWidget(backgroundSeparationBackButton);
    backgroundSeparationLayout->addWidget(backgroundSeparationLoadButton);
    backgroundSeparationLayout->addWidget(backgroundSeparationApplyButton);
    backgroundSeparationLayout->addWidget(backgroundSeparationSaveButton);
    backgroundSeparationLayout->addWidget(backgroundSeparationImageLabel);
    backgroundSeparationLayout->addWidget(backgroundSeparationStatusLabel);
    backgroundSeparationPage->setLayout(backgroundSeparationLayout);

    // ==== Image Resizing Page ====
    QWidget* resizingPage = new QWidget;
    QVBoxLayout* resizingLayout = new QVBoxLayout;
    QLabel* resizingImageLabel = new QLabel;

    resizingImageLabel->setAlignment(Qt::AlignCenter);
    resizingImageLabel->setStyleSheet("border: 1px solid #ccc; background: white;");
    resizingImageLabel->setMinimumSize(600, 400);

    QLabel* resizingStatusLabel = new QLabel("No image loaded");
    resizingStatusLabel->setAlignment(Qt::AlignCenter);

    QPushButton* resizingLoadButton = new QPushButton("📁 Upload Image");
    QPushButton* resizingSaveButton = new QPushButton("💾 Save Image");
    QPushButton* resizingBackButton = new QPushButton("⬅️ Return to menu");

    QSlider* resizingSlider = new QSlider(Qt::Horizontal);
    resizingSlider->setRange(10, 300); // 0.1x à 3.0x
    resizingSlider->setValue(100);     // 1.0x par défaut
    QLabel* resizingScaleLabel = new QLabel("Scale: 1.0x");

    double resizingScaleFactor = 1.0;
    ImageResizer imageResizer;

    auto updateResizeImage = [&]() {
        cout << "[DEBUG] scaleFactor = " << resizingScaleFactor << endl;
        imageResizer.resize(resizingScaleFactor);
        resizingImageLabel->setPixmap(QPixmap::fromImage(imageResizer.getQImage()));
        resizingScaleLabel->setText("Scale: " + QString::number(resizingScaleFactor, 'f', 2) + "x");
        resizingStatusLabel->setText("🔄 Resize applied");
    };

    QObject::connect(resizingLoadButton, &QPushButton::clicked, [&]() {
        QString fileName = QFileDialog::getOpenFileName(&window, "Select an image", "", "Images (*.png *.jpg *.jpeg *.bmp)");
        if (!fileName.isEmpty()) {
            Mat img = imread(fileName.toStdString());

            if (!img.empty()) {
                imageResizer.setImage(img);
                resizingScaleFactor = 1.0;
                resizingSlider->setValue(100);
                updateResizeImage();
                resizingStatusLabel->setText("✅ Image loaded");
            }
        }
    });

    QObject::connect(resizingSlider, &QSlider::valueChanged, [&](int value) {
        resizingScaleFactor = value / 100.0;
        imageResizer.setScaleFactor(resizingScaleFactor);
        updateResizeImage();
    });

    QObject::connect(resizingSaveButton, &QPushButton::clicked, [&]() {
        QString filePath = QFileDialog::getSaveFileName(&window, "Save image", "", "Images (*.png *.jpg *.bmp)");
        if (!filePath.isEmpty()) {
            imwrite(filePath.toStdString(), imageResizer.getResizedMat());
            resizingStatusLabel->setText("💾 Image saved!");
        }
    });

    QObject::connect(resizingBackButton, &QPushButton::clicked, [&]() {
        stackedWidget->setCurrentWidget(homePage);
    });

    resizingLayout->addWidget(resizingBackButton);
    resizingLayout->addWidget(resizingLoadButton);
    resizingLayout->addWidget(resizingSaveButton);
    resizingLayout->addWidget(resizingScaleLabel);
    resizingLayout->addWidget(resizingSlider);
    resizingLayout->addWidget(resizingImageLabel);
    resizingLayout->addWidget(resizingStatusLabel);
    resizingPage->setLayout(resizingLayout);

    // ==== Face Detection Page ====
    QWidget* faceDetectionPage = new QWidget;
    QVBoxLayout* faceDetectionLayout = new QVBoxLayout;

    QLabel* faceDetectionImageLabel = new QLabel;
    faceDetectionImageLabel->setAlignment(Qt::AlignCenter);
    faceDetectionImageLabel->setMinimumSize(600, 400);
    faceDetectionImageLabel->setStyleSheet("border: 1px solid #ccc; background: white;");

    QLabel* faceDetectionStatusLabel = new QLabel("No image loaded");
    faceDetectionStatusLabel->setAlignment(Qt::AlignCenter);

    QPushButton* faceDetectionLoadButton = new QPushButton("📁 Load image");
    QPushButton* faceDetectionDetectButton = new QPushButton("🔍 Detect faces, smiles and cats");
    QPushButton* faceDetectionBackButton = new QPushButton("⬅️ Return to menu");

    Mat faceDetectionOriginalImage;
    Mat faceDetectionDetectedImage;

    QObject::connect(faceDetectionLoadButton, &QPushButton::clicked, [&]() {
        QString path = QFileDialog::getOpenFileName(&window, "Load image", "", "Images (*.png *.jpg *.bmp)");
        if (!path.isEmpty()) {
            faceDetectionOriginalImage = imread(path.toStdString());
            if (!faceDetectionOriginalImage.empty()) {
                QImage qimg((uchar*)faceDetectionOriginalImage.data, faceDetectionOriginalImage.cols, faceDetectionOriginalImage.rows, faceDetectionOriginalImage.step, QImage::Format_BGR888);
                faceDetectionImageLabel->setPixmap(QPixmap::fromImage(qimg).scaled(faceDetectionImageLabel->size(), Qt::KeepAspectRatio));
                faceDetectionStatusLabel->setText("✅ Image loaded");
            }
            else {
                faceDetectionStatusLabel->setText("❌ Failed to load image");
            }
        }
    });

    QObject::connect(faceDetectionDetectButton, &QPushButton::clicked, [&]() {
        if (!faceDetectionOriginalImage.empty()) {
            faceDetectionDetectedImage = applyDetection(faceDetectionOriginalImage);
            QImage qimg((uchar*)faceDetectionDetectedImage.data, faceDetectionDetectedImage.cols, faceDetectionDetectedImage.rows, faceDetectionDetectedImage.step, QImage::Format_BGR888);
            faceDetectionImageLabel->setPixmap(QPixmap::fromImage(qimg).scaled(faceDetectionImageLabel->size(), Qt::KeepAspectRatio));
            faceDetectionStatusLabel->setText("😊 Detection applied");
        }
        else {
            faceDetectionStatusLabel->setText("❗ No image loaded");
        }
    });

    QObject::connect(faceDetectionBackButton, &QPushButton::clicked, [&]() {
        stackedWidget->setCurrentIndex(0);
    });

    faceDetectionLayout->addWidget(faceDetectionBackButton);
    faceDetectionLayout->addWidget(faceDetectionLoadButton);
    faceDetectionLayout->addWidget(faceDetectionDetectButton);
    faceDetectionLayout->addWidget(faceDetectionImageLabel);
    faceDetectionLayout->addWidget(faceDetectionStatusLabel);
    faceDetectionPage->setLayout(faceDetectionLayout);

	// ==== Video Manipulation Page ====
    VideoManipulation* videoManipulationPage = new VideoManipulation;

    // ==== Add pages to the Home Page ====
    stackedWidget->addWidget(homePage);       
    stackedWidget->addWidget(cannyEdgeDetectionPage);    
    stackedWidget->addWidget(morphologyPage);   
    stackedWidget->addWidget(backgroundSeparationPage); 
    stackedWidget->addWidget(resizingPage);       
    stackedWidget->addWidget(faceDetectionPage);        
    stackedWidget->addWidget(videoManipulationPage); 

    QObject::connect(goToCannyEdgeDetectionPage, &QPushButton::clicked, [&]() {
        stackedWidget->setCurrentIndex(1);
    });

    QObject::connect(goToMorphologyPage, &QPushButton::clicked, [&]() {
        stackedWidget->setCurrentIndex(2);
    });

    QObject::connect(goToBackgroundSeparationPage, &QPushButton::clicked, [&]() {
        stackedWidget->setCurrentIndex(3);
    });

    QObject::connect(goToResizingPage, &QPushButton::clicked, [&]() {
        stackedWidget->setCurrentIndex(4);
    });

    QObject::connect(goToFaceDetectionPage, &QPushButton::clicked, [&]() {
        stackedWidget->setCurrentIndex(5);
    });

    QObject::connect(goToVideoManipulationPage, &QPushButton::clicked, [&]() {
        stackedWidget->setCurrentIndex(6);
    });

    window.setLayout(new QVBoxLayout);
    window.layout()->addWidget(stackedWidget);
    window.showMaximized();

    return app.exec();
}