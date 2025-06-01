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
#include <QTimer>
#include <opencv2/opencv.hpp>

#include "image_morphology.h"
#include "image_resize.h"
#include "brightness_adjuster.h"
#include "canny_edge_detector.h"
#include "panorama.h"
#include "video_manipulation.h"
#include "face_detection.h"
#include "background_separator.h"
#include "object_detector.h"

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
    QPushButton* goToPanoramaPage = new QPushButton("🌄 Panorama");
    QPushButton* goToObjectDetectionPage = new QPushButton("🔲  Object Detection");
    QPushButton* goToBrightnessAdjusterPage = new QPushButton("💡 Adjust Brightness");

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
	homeLayout->addWidget(goToPanoramaPage);
	homeLayout->addWidget(goToObjectDetectionPage);
    homeLayout->addWidget(goToBrightnessAdjusterPage);
    homeLayout->addItem(spacerBottom);
    homePage->setLayout(homeLayout);

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

    Morphology morphologyProcessor;
    Mat morphologyOriginalImage;
    Mat morphologyResult;

    auto updateMorphology = [&](bool dilate) {
        if (morphologyOriginalImage.empty()) {
            morphologyStatusLabel->setText("❗ No image loaded");
            return;
        }
        int size = morphologySlider->value();
        morphologyResult = morphologyProcessor.applyMorphology(morphologyOriginalImage, size, dilate);
        QImage qimg((uchar*)morphologyResult.data, morphologyResult.cols, morphologyResult.rows, morphologyResult.step, QImage::Format_BGR888);
        morphologyImageLabel->setPixmap(QPixmap::fromImage(qimg).scaled(morphologyImageLabel->size(), Qt::KeepAspectRatio));
        morphologyStatusLabel->setText(dilate ? "✅ Dilation applied" : "✅ Erosion applied");
    };

    QObject::connect(morphologyLoadButton, &QPushButton::clicked, [&]() {
        QString path = QFileDialog::getOpenFileName(&window, "Load image", "./data/images", "Images (*.png *.jpg *.jpeg *.bmp)");
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
        QString fileName = QFileDialog::getOpenFileName(&window, "Load image", "./data/images", "Images (*.png *.jpg *.jpeg *.bmp)");
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
        QString filePath = QFileDialog::getSaveFileName(&window, "Save image", "./data/out", "Images (*.png *.jpg *.jpeg *.bmp)");
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
        QString path = QFileDialog::getOpenFileName(&window, "Load image", "./data/images", "Images (*.png *.jpg *.jpeg *.bmp)");
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
        QString path = QFileDialog::getSaveFileName(&window, "Save image", "./data/out", "Images (*.png *.jpg *.jpeg *.bmp)");
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
        QString fileName = QFileDialog::getOpenFileName(&window, "Load image", "./data/images", "Images (*.png *.jpg *.jpeg *.bmp)");
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
        QString filePath = QFileDialog::getSaveFileName(&window, "Save image", "./data/out", "Images (*.png *.jpg *.jpeg *.bmp)");
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

    Detector faceDetector;
    faceDetector.loadClassifiers("data/haarcascades/haarcascade_frontalface_default.xml", "data/haarcascades/haarcascade_smile.xml", "data/haarcascades/haarcascade_frontalcatface.xml");

    QObject::connect(faceDetectionLoadButton, &QPushButton::clicked, [&]() {
        QString path = QFileDialog::getOpenFileName(&window, "Load image", "./data/images", "Images (*.png *.jpg *.jpeg *.bmp)");
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
            faceDetectionDetectedImage = faceDetector.applyDetection(faceDetectionOriginalImage);
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
    QWidget* videoManipulationPage = new QWidget;
    QVBoxLayout* videoLayout = new QVBoxLayout;

    QLabel* videoLabel = new QLabel("No video loaded");
    videoLabel->setAlignment(Qt::AlignCenter);
    videoLabel->setMinimumSize(800, 500);
    videoLabel->setStyleSheet("border: 1px solid #ccc; background: black;");

    QLabel* videoStatusLabel = new QLabel("No video loaded");
    videoStatusLabel->setAlignment(Qt::AlignCenter);

    QPushButton* videoLoadButton = new QPushButton("📁 Load Video");
    QPushButton* videoPlayPauseButton = new QPushButton("▶️ Play");
    QPushButton* videoSaveButton = new QPushButton("💾 Save Video");
    QPushButton* videoBackButton = new QPushButton("⬅️ Return to menu");

    QSlider* videoFrameSlider = new QSlider(Qt::Horizontal);
    videoFrameSlider->setEnabled(false);

    QSlider* videoSpeedSlider = new QSlider(Qt::Horizontal);
    videoSpeedSlider->setRange(0, 300); 
    videoSpeedSlider->setValue(100);

    QHBoxLayout* speedLayout = new QHBoxLayout;
    QLabel* speedLabel = new QLabel("Speed:");
    QLabel* videoSpeedValueLabel = new QLabel("x1,0"); 
    speedLayout->addWidget(speedLabel);
    speedLayout->addWidget(videoSpeedValueLabel);
    speedLayout->addStretch();

    VideoManipulation videoProcessor;
    QTimer* videoTimer = new QTimer;
    bool isPlaying = false;

    QObject::connect(videoLoadButton, &QPushButton::clicked, [&]() {
        QString filename = QFileDialog::getOpenFileName(&window, "Open Video", "./data/videos", "Video Files (*.mov *.mp4)");
        if (!filename.isEmpty() && videoProcessor.loadVideo(filename.toStdString())) {
            videoLabel->setPixmap(QPixmap::fromImage(videoProcessor.getCurrentFrameQImage()).scaled(videoLabel->size(), Qt::KeepAspectRatio));
            videoFrameSlider->setRange(0, videoProcessor.getTotalFrames() - 1);
            videoFrameSlider->setEnabled(true);
            videoStatusLabel->setText("✅ Video loaded");
        } else {
            videoStatusLabel->setText("❌ Failed to load video");
        }
    });

    QObject::connect(videoPlayPauseButton, &QPushButton::clicked, [&]() {
    if (!isPlaying) {
        double factor = videoProcessor.getSpeedFactor();
        if (factor > 0.01) {
            videoTimer->setInterval(static_cast<int>(33 / factor));
        } else {
            videoTimer->setInterval(1000);
        }

        videoTimer->start();
        videoPlayPauseButton->setText("⏸️ Pause");
        isPlaying = true;
        } else {
            videoTimer->stop();
            videoPlayPauseButton->setText("▶️ Play");
            isPlaying = false;
        }
    });

    QObject::connect(videoSaveButton, &QPushButton::clicked, [&]() {
        QString path = QFileDialog::getSaveFileName(&window, "Save video", "./data/out", "Video Files (*.mov *.mp4)");
        if (!path.isEmpty()) {
            if (videoProcessor.saveVideo(path.toStdString())) {
                videoStatusLabel->setText("💾 Video saved!");
            } else {
                videoStatusLabel->setText("❌ Failed to save video");
            }
        }
    });

    QObject::connect(videoFrameSlider, &QSlider::valueChanged, [&](int value) {
        if (videoProcessor.goToFrame(value)) {
            videoLabel->setPixmap(QPixmap::fromImage(videoProcessor.getCurrentFrameQImage()).scaled(videoLabel->size(), Qt::KeepAspectRatio));
        }
    });

    QObject::connect(videoSpeedSlider, &QSlider::valueChanged, [&](int value) {
        double factor = value / 100.0;  // 0–3
        videoProcessor.setSpeedFactor(factor);

        QString speedText = "x" + QString::number(factor, 'f', 1);
        videoSpeedValueLabel->setText(speedText);

        if (factor > 0.01) {
            videoTimer->setInterval(static_cast<int>(33 / factor));
        } else {
            videoTimer->setInterval(1000);
        }
    });

    QObject::connect(videoTimer, &QTimer::timeout, [&]() {
        if (videoProcessor.nextFrame()) {
            videoFrameSlider->blockSignals(true);
            videoFrameSlider->setValue(videoProcessor.getCurrentFrame());
            videoFrameSlider->blockSignals(false);
            videoLabel->setPixmap(QPixmap::fromImage(videoProcessor.getCurrentFrameQImage()).scaled(videoLabel->size(), Qt::KeepAspectRatio));
        } else {
            videoTimer->stop();
            videoPlayPauseButton->setText("▶️ Play");
            isPlaying = false;
        }
    });

    QObject::connect(videoBackButton, &QPushButton::clicked, [&]() {
        videoTimer->stop();
        videoPlayPauseButton->setText("▶️ Play");
        isPlaying = false;
        stackedWidget->setCurrentWidget(homePage);
    });

    videoLayout->addWidget(videoBackButton);
    videoLayout->addWidget(videoLoadButton);
    videoLayout->addWidget(videoPlayPauseButton);
    videoLayout->addWidget(videoSaveButton);
    videoLayout->addLayout(speedLayout);
    videoLayout->addWidget(videoSpeedSlider);
    videoLayout->addWidget(new QLabel("Frame:"));
    videoLayout->addWidget(videoFrameSlider);
    videoLayout->addWidget(videoLabel);
    videoLayout->addWidget(videoStatusLabel);
    videoManipulationPage->setLayout(videoLayout);

    // ==== Panorama Page ====
    QWidget* panoramaPage = new QWidget;
    QVBoxLayout* panoramaLayout = new QVBoxLayout;

    QLabel* panoramaImageLabel = new QLabel;
    panoramaImageLabel->setAlignment(Qt::AlignCenter);
    panoramaImageLabel->setMinimumSize(600, 400);
    panoramaImageLabel->setStyleSheet("border: 1px solid #ccc; background: white;");

    QLabel* panoramaStatusLabel = new QLabel("No images loaded");
    panoramaStatusLabel->setAlignment(Qt::AlignCenter);

    QPushButton* panoramaLoadButton = new QPushButton("📁 Load images");
    QPushButton* panoramaStitchButton = new QPushButton("🪄 Compute panorama");
    QPushButton* panoramaSaveButton = new QPushButton("💾 Save panorama");
    QPushButton* panoramaBackButton = new QPushButton("⬅️ Return to menu");

    Panorama panoramaProcessor;

    QObject::connect(panoramaLoadButton, &QPushButton::clicked, [&]() {
        QStringList files = QFileDialog::getOpenFileNames(&window, "Load images", "./data/images", "Images (*.png *.jpg *.jpeg *.bmp)");
        if (!files.isEmpty()) {
            panoramaProcessor.clearImages();
            for (const QString& file : files) {
                panoramaProcessor.loadImages(file.toStdString());
            }
            panoramaStatusLabel->setText("✅ " + QString::number(panoramaProcessor.getImagesLoadedCount()) + " image(s) loaded");
        }
        else {
            panoramaStatusLabel->setText("❌ No images loaded");
        }
    });

    QObject::connect(panoramaStitchButton, &QPushButton::clicked, [&]() {
        if (panoramaProcessor.getImagesLoadedCount() < 2) {
            panoramaStatusLabel->setText("❗ Need at least two images");
            return;
        }
        if (panoramaProcessor.computePanorama()) {
            Mat panoramaResult = panoramaProcessor.getPanoramaImage();
            QImage qimg((uchar*)panoramaResult.data, panoramaResult.cols, panoramaResult.rows, panoramaResult.step, QImage::Format_BGR888);
            panoramaImageLabel->setPixmap(QPixmap::fromImage(qimg.rgbSwapped()).scaled(panoramaImageLabel->size(), Qt::KeepAspectRatio));
            panoramaStatusLabel->setText("✅ Panorama created");
        }
        else {
            panoramaStatusLabel->setText("❌ Computing of the panorama failed");
        }
    });

    QObject::connect(panoramaSaveButton, &QPushButton::clicked, [&]() {
        if (panoramaProcessor.getPanoramaImage().empty()) {
            panoramaStatusLabel->setText("❗ No panorama to save");
            return;
        }
        QString path = QFileDialog::getSaveFileName(&window, "Save image", "./data/out", "Images (*.png *.jpg *.jpeg *.bmp)");
        if (!path.isEmpty()) {
            if (panoramaProcessor.savePanoramaImage(path.toStdString())) {
                panoramaStatusLabel->setText("💾 Panorama saved");
            }
            else {
                panoramaStatusLabel->setText("❌ Failed to save panorama");
            }
        }
    });

    QObject::connect(panoramaBackButton, &QPushButton::clicked, [&]() {
        stackedWidget->setCurrentWidget(homePage);
    });

    panoramaLayout->addWidget(panoramaBackButton);
    panoramaLayout->addWidget(panoramaLoadButton);
    panoramaLayout->addWidget(panoramaStitchButton);
    panoramaLayout->addWidget(panoramaSaveButton);
    panoramaLayout->addWidget(panoramaImageLabel);
    panoramaLayout->addWidget(panoramaStatusLabel);
    panoramaPage->setLayout(panoramaLayout);


    // ==== Object Detection Page ====
    QWidget* objectDetectionPage = new QWidget;
    QVBoxLayout* objectDetectionLayout = new QVBoxLayout;

    QLabel* objectDetectionImageLabel = new QLabel;
    objectDetectionImageLabel->setAlignment(Qt::AlignCenter);
    objectDetectionImageLabel->setMinimumSize(600, 400);
    objectDetectionImageLabel->setStyleSheet("border: 1px solid #ccc; background: white;");

    QLabel* objectDetectionStatusLabel = new QLabel("No image loaded");
    objectDetectionStatusLabel->setAlignment(Qt::AlignCenter);

    QPushButton* objectDetectionLoadButton = new QPushButton("📁 Load image");
    QPushButton* objectDetectionDetectButton = new QPushButton("🔎 Detect object");
    QPushButton* objectDetectionBackButton = new QPushButton("⬅️ Return to menu");

    Mat objectDetectionOriginalImage;
    Mat objectDetectionDetectedImage;

    string classFile = "data/yolo/coco.names";
    string configFile = "data/yolo/yolov3.cfg";
    string weightsFile = "data/yolo/yolov3.weights";

    ObjectDetector objectDetector(classFile, configFile, weightsFile);
    bool yoloLoaded = objectDetector.charger();

    QObject::connect(objectDetectionLoadButton, &QPushButton::clicked, [&]() {
        QString path = QFileDialog::getOpenFileName(&window, "Load image", "./data/images", "Images (*.png *.jpg *.jpeg *.bmp)");
        if (!path.isEmpty()) {
            objectDetectionOriginalImage = imread(path.toStdString());
            if (!objectDetectionOriginalImage.empty()) {
                QImage qimg((uchar*)objectDetectionOriginalImage.data, objectDetectionOriginalImage.cols, objectDetectionOriginalImage.rows, objectDetectionOriginalImage.step, QImage::Format_BGR888);
                objectDetectionImageLabel->setPixmap(QPixmap::fromImage(qimg).scaled(objectDetectionImageLabel->size(), Qt::KeepAspectRatio));
                objectDetectionStatusLabel->setText("✅ Image loaded");
            }
            else {
                objectDetectionStatusLabel->setText("❌ Failed to load image");
            }
        }
    });

    QObject::connect(objectDetectionDetectButton, &QPushButton::clicked, [&]() {
        if (!yoloLoaded) {
            objectDetectionStatusLabel->setText("❌ Object detector not loaded");
            return;
        }
        if (objectDetectionOriginalImage.empty()) {
            objectDetectionStatusLabel->setText("❗ No image loaded");
            return;
        }
        objectDetectionDetectedImage = objectDetectionOriginalImage.clone();
        objectDetector.detecter(objectDetectionDetectedImage);
        QImage qimg((uchar*)objectDetectionDetectedImage.data, objectDetectionDetectedImage.cols, objectDetectionDetectedImage.rows, objectDetectionDetectedImage.step, QImage::Format_BGR888);
        objectDetectionImageLabel->setPixmap(QPixmap::fromImage(qimg).scaled(objectDetectionImageLabel->size(), Qt::KeepAspectRatio));
        objectDetectionStatusLabel->setText("✅ Object detected");
    });

    QObject::connect(objectDetectionBackButton, &QPushButton::clicked, [&]() {
        stackedWidget->setCurrentWidget(homePage);
    });

    objectDetectionLayout->addWidget(objectDetectionBackButton);
    objectDetectionLayout->addWidget(objectDetectionLoadButton);
    objectDetectionLayout->addWidget(objectDetectionDetectButton);
    objectDetectionLayout->addWidget(objectDetectionImageLabel);
    objectDetectionLayout->addWidget(objectDetectionStatusLabel);
    objectDetectionPage->setLayout(objectDetectionLayout);

    // ==== Brightness Adjuster Page ====
    QWidget* brightnessAdjusterPage = new QWidget;
    QVBoxLayout* brightnessLayout = new QVBoxLayout;
    QLabel* brightnessImageLabel = new QLabel;
    brightnessImageLabel->setAlignment(Qt::AlignCenter);
    brightnessImageLabel->setMinimumSize(600, 400);
    brightnessImageLabel->setStyleSheet("border: 1px solid #ccc; background: white;");

    QLabel* brightnessStatusLabel = new QLabel("No image loaded");
    brightnessStatusLabel->setAlignment(Qt::AlignCenter);

    QPushButton* brightnessLoadButton = new QPushButton("📁 Load image");
    QPushButton* brightnessSaveButton = new QPushButton("💾 Save image");
    QPushButton* brightnessBackButton = new QPushButton("⬅️ Return to menu");

    QSlider* brightnessSlider = new QSlider(Qt::Horizontal);
    brightnessSlider->setRange(10, 300); 
    brightnessSlider->setValue(100);

    QLabel* brightnessFactorLabel = new QLabel("Brightness factor: 1.0");

    cv::Mat brightnessOriginalImage;
    cv::Mat brightnessAdjustedImage;

    QObject::connect(brightnessLoadButton, &QPushButton::clicked, [&]() {
        QString filePath = QFileDialog::getOpenFileName(&window, "Load image", "./data/images", "Images (*.png *.jpg *.jpeg *.bmp)");
        if (!filePath.isEmpty()) {
            brightnessOriginalImage = cv::imread(filePath.toStdString());
            if (brightnessOriginalImage.empty()) {
                brightnessStatusLabel->setText("❌ Failed to load image");
                return;
            }
            cv::cvtColor(brightnessOriginalImage, brightnessOriginalImage, cv::COLOR_BGR2RGB);
            QImage qimg(brightnessOriginalImage.data, brightnessOriginalImage.cols, brightnessOriginalImage.rows, brightnessOriginalImage.step, QImage::Format_RGB888);
            brightnessImageLabel->setPixmap(QPixmap::fromImage(qimg).scaled(brightnessImageLabel->size(), Qt::KeepAspectRatio));
            brightnessStatusLabel->setText("✅ Image loaded");
        }
    });

    QObject::connect(brightnessSlider, &QSlider::valueChanged, [&](int value) {
        if (brightnessOriginalImage.empty()) return;
        double factor = value / 100.0;
        brightnessFactorLabel->setText("Brightness factor: " + QString::number(factor, 'f', 2));

        BrightnessAdjuster adjuster(factor);
        brightnessAdjustedImage = adjuster.adjust(brightnessOriginalImage);

        QImage qimg(brightnessAdjustedImage.data, brightnessAdjustedImage.cols, brightnessAdjustedImage.rows, brightnessAdjustedImage.step, QImage::Format_RGB888);
        brightnessImageLabel->setPixmap(QPixmap::fromImage(qimg).scaled(brightnessImageLabel->size(), Qt::KeepAspectRatio));
        brightnessStatusLabel->setText("✅ Brightness adjusted");
    });

    QObject::connect(brightnessSaveButton, &QPushButton::clicked, [&]() {
        if (brightnessAdjustedImage.empty()) return;
        QString savePath = QFileDialog::getSaveFileName(&window, "Save image", "./data/out", "Images (*.png *.jpg *.jpeg *.bmp)");
        if (!savePath.isEmpty()) {
            cv::imwrite(savePath.toStdString(), cv::Mat(brightnessAdjustedImage));
            brightnessStatusLabel->setText("💾 Image saved!");
        }
    });

    QObject::connect(brightnessBackButton, &QPushButton::clicked, [&]() {
        stackedWidget->setCurrentWidget(homePage);
        brightnessSlider->setValue(100);
        brightnessImageLabel->clear();
        brightnessStatusLabel->setText("No image loaded");
    });

    brightnessLayout->addWidget(brightnessBackButton);
    brightnessLayout->addWidget(brightnessLoadButton);
    brightnessLayout->addWidget(brightnessSaveButton);
    brightnessLayout->addWidget(brightnessFactorLabel);
    brightnessLayout->addWidget(brightnessSlider);
    brightnessLayout->addWidget(brightnessImageLabel);
    brightnessLayout->addWidget(brightnessStatusLabel);
    brightnessAdjusterPage->setLayout(brightnessLayout);

    // ==== Add pages to the Home Page ====
    stackedWidget->addWidget(homePage);       
    stackedWidget->addWidget(cannyEdgeDetectionPage);    
    stackedWidget->addWidget(morphologyPage);   
    stackedWidget->addWidget(backgroundSeparationPage); 
    stackedWidget->addWidget(resizingPage);       
    stackedWidget->addWidget(faceDetectionPage);        
    stackedWidget->addWidget(videoManipulationPage); 
	stackedWidget->addWidget(panoramaPage);
	stackedWidget->addWidget(objectDetectionPage);
	stackedWidget->addWidget(brightnessAdjusterPage);

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

	QObject::connect(goToPanoramaPage, &QPushButton::clicked, [&]() {
		stackedWidget->setCurrentIndex(7);
    });

	QObject::connect(goToObjectDetectionPage, &QPushButton::clicked, [&]() {
		stackedWidget->setCurrentIndex(8);
    });

	QObject::connect(goToBrightnessAdjusterPage, &QPushButton::clicked, [&]() {
		stackedWidget->setCurrentIndex(9);
    });

    window.setLayout(new QVBoxLayout);
    window.layout()->addWidget(stackedWidget);
    window.showMaximized();

    return app.exec();
}