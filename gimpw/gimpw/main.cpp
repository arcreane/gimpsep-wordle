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
#include "background_detector.h"

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

    QPushButton* goToCanny = new QPushButton("🔍 Canny Edge Detection");
    QPushButton* goToDetection = new QPushButton("🧬 Detection + Morphology");
    QPushButton* goToBackground = new QPushButton("🖼️ Background Detection");

    QSpacerItem* spacerTop = new QSpacerItem(20, 100, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QSpacerItem* spacerBottom = new QSpacerItem(20, 100, QSizePolicy::Minimum, QSizePolicy::Expanding);

    homeLayout->addWidget(titleLabel);
    homeLayout->addItem(spacerTop);
    homeLayout->addWidget(goToCanny);
    homeLayout->addWidget(goToDetection);
    homeLayout->addWidget(goToBackground);
    homeLayout->addItem(spacerBottom);
    homePage->setLayout(homeLayout);

    // ==== Canny Edge Detection Page ====
    QWidget* cannyPage = new QWidget;
    QVBoxLayout* cannyLayout = new QVBoxLayout;
    QLabel* imageLabel = new QLabel;
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setMinimumSize(600, 400);
    imageLabel->setStyleSheet("border: 1px solid #ccc; background: white;");
    QLabel* statusLabel = new QLabel("No image loaded");
    statusLabel->setAlignment(Qt::AlignCenter);
    QPushButton* loadButton = new QPushButton("📁 Load image");
    QPushButton* saveButton = new QPushButton("💾 Save image");
    QPushButton* backButton = new QPushButton("⬅️ Return to menu");
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
        statusLabel->setText("🔄 Automatic update : thresholds " + QString::number(sliderT1->value()) + ", " + QString::number(sliderT2->value()));
    };

    QObject::connect(loadButton, &QPushButton::clicked, [&]() {
        QString fileName = QFileDialog::getOpenFileName(&window, "Load image", "", "Images (*.png *.jpg *.jpeg *.bmp)");
        if (!fileName.isEmpty()) {
            CannyEdgeDetector newDetector(fileName.toStdString(), sliderT1->value(), sliderT2->value());
            detector = newDetector;
            imageLabel->setPixmap(QPixmap::fromImage(QImage(fileName).scaled(imageLabel->size(), Qt::KeepAspectRatio)));
            statusLabel->setText("✅ Image correctly loaded");
        }
        else {
            statusLabel->setText("❌ Something went wrong with the loading of the image");
        }
    });

    QObject::connect(sliderT1, &QSlider::valueChanged, [&](int value) {
        t1Label->setText("Threshold 1 : " + QString::number(value));
        updateCanny();
    });

    QObject::connect(sliderT2, &QSlider::valueChanged, [&](int value) {
        t2Label->setText("Threshold 2 : " + QString::number(value));
        updateCanny();
    });

    QObject::connect(saveButton, &QPushButton::clicked, [&]() {
        QString filePath = QFileDialog::getSaveFileName(&window, "Save image", "", "Images (*.png *.jpg *.bmp)");
        if (!filePath.isEmpty() && detector.saveImage(filePath.toStdString())) {
            statusLabel->setText("💾 Image correctly saved");
        }
        else {
            statusLabel->setText("❌ Something went wrong with the saving of the image");
        }
    });

    QObject::connect(backButton, &QPushButton::clicked, [&]() {
        stackedWidget->setCurrentIndex(0);
    });

    cannyLayout->addWidget(backButton);
    cannyLayout->addWidget(loadButton);
    cannyLayout->addWidget(saveButton);
    cannyLayout->addWidget(t1Label);
    cannyLayout->addWidget(sliderT1);
    cannyLayout->addWidget(t2Label);
    cannyLayout->addWidget(sliderT2);
    cannyLayout->addWidget(imageLabel);
    cannyLayout->addWidget(statusLabel);
    cannyPage->setLayout(cannyLayout);

    // ==== Detection + Morphology Page ====
    QWidget* detectionPage = new QWidget;
    QVBoxLayout* detectionLayout = new QVBoxLayout;
    QLabel* detectionLabel = new QLabel("Detection + Morphology to implement");
    detectionLabel->setAlignment(Qt::AlignCenter);
    QPushButton* returnButton = new QPushButton("⬅️ Return to menu");

    QObject::connect(returnButton, &QPushButton::clicked, [&]() {
        stackedWidget->setCurrentIndex(0);
    });

    detectionLayout->addWidget(detectionLabel);
    detectionLayout->addWidget(returnButton);
    detectionPage->setLayout(detectionLayout);

    // ==== Background Detection Page ====
    QWidget* backgroundPage = new QWidget;
    QVBoxLayout* bgLayout = new QVBoxLayout;
    QLabel* bgImageLabel = new QLabel("No image loaded");
    bgImageLabel->setAlignment(Qt::AlignCenter);
    bgImageLabel->setMinimumSize(600, 400);
    bgImageLabel->setStyleSheet("border: 1px solid #ccc; background: white;");

    QPushButton* bgLoadButton = new QPushButton("📁 Load image");
    QPushButton* bgApplyButton = new QPushButton("🎯 Apply background detection");
    QPushButton* bgSaveButton = new QPushButton("💾 Save image");
    QPushButton* bgBackButton = new QPushButton("⬅️ Return to menu");

    QLabel* bgStatusLabel = new QLabel("State: ready");
    bgStatusLabel->setAlignment(Qt::AlignCenter);

    bgLayout->addWidget(bgBackButton);
    bgLayout->addWidget(bgLoadButton);
    bgLayout->addWidget(bgApplyButton);
    bgLayout->addWidget(bgSaveButton);
    bgLayout->addWidget(bgImageLabel);
    bgLayout->addWidget(bgStatusLabel);
    backgroundPage->setLayout(bgLayout);

    BackgroundDetector bgDetector;
    QImage loadedImage;

    QObject::connect(bgLoadButton, &QPushButton::clicked, [&]() {
        QString path = QFileDialog::getOpenFileName(&window, "Load image", "", "Images (*.png *.jpg *.bmp)");
        if (!path.isEmpty() && bgDetector.loadImage(path)) {
            loadedImage = bgDetector.getOriginalQImage();
            bgImageLabel->setPixmap(QPixmap::fromImage(loadedImage).scaled(bgImageLabel->size(), Qt::KeepAspectRatio));
            bgStatusLabel->setText("✅ Image correctly loaded");
        }
		else {
			bgStatusLabel->setText("❌ Something went wrong with the loading of the image");
		}
    });

    QObject::connect(bgApplyButton, &QPushButton::clicked, [&]() {
        bgStatusLabel->setText("⏳ Processing background detection...");
        app.processEvents(); 

        QRect rect = bgDetector.computeAutoSelection();
        if (!rect.isNull()) {
            bgDetector.setSelection(rect.topLeft(), rect.bottomRight());
            bgDetector.apply();
            QImage result = bgDetector.getResultQImage();
            bgImageLabel->setPixmap(QPixmap::fromImage(result).scaled(bgImageLabel->size(), Qt::KeepAspectRatio));
            bgStatusLabel->setText("🎯 Background detection applied");
        }
        else {
            bgStatusLabel->setText("❗Something went wrong with the background detection");
        }
    });

    QObject::connect(bgSaveButton, &QPushButton::clicked, [&]() {
        QString path = QFileDialog::getSaveFileName(&window, "Save image", "", "Images (*.png)");
        if (!path.isEmpty() && bgDetector.saveResult(path)) {
            bgStatusLabel->setText("💾 Image correctly saved");
        }
        else {
            bgStatusLabel->setText("❌ Something went wrong with the saving of the image");
        }
    });

    QObject::connect(bgBackButton, &QPushButton::clicked, [&]() {
        stackedWidget->setCurrentIndex(0);
    });


    // ==== Add pages to the Home Page ====
    stackedWidget->addWidget(homePage);
    stackedWidget->addWidget(cannyPage);
    stackedWidget->addWidget(detectionPage);
    stackedWidget->addWidget(backgroundPage);

    QObject::connect(goToCanny, &QPushButton::clicked, [&]() {
        stackedWidget->setCurrentIndex(1);
    });

    QObject::connect(goToDetection, &QPushButton::clicked, [&]() {
        stackedWidget->setCurrentIndex(2);
    });

    QObject::connect(goToBackground, &QPushButton::clicked, [&]() {
        stackedWidget->setCurrentIndex(3);
    });

    window.setLayout(new QVBoxLayout);
    window.layout()->addWidget(stackedWidget);
    window.showMaximized();

    return app.exec();
}
