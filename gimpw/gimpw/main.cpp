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
#include "image_resize.h"
#include "image_morphology.h"
#include "face_detection.h"

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
    QPushButton* goToMorphology = new QPushButton("🧬 Morphology");
    QPushButton* goToBackground = new QPushButton("🖼️ Background Detection");
    QPushButton* goToResizing = new QPushButton("↔️ Image Resizing");
    QPushButton* goToFaceDetection = new QPushButton("😊 Face Detection");

    QSpacerItem* spacerTop = new QSpacerItem(20, 100, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QSpacerItem* spacerBottom = new QSpacerItem(20, 100, QSizePolicy::Minimum, QSizePolicy::Expanding);

    homeLayout->addWidget(titleLabel);
    homeLayout->addItem(spacerTop);
    homeLayout->addWidget(goToCanny);
    homeLayout->addWidget(goToMorphology);
    homeLayout->addWidget(goToBackground);
    homeLayout->addWidget(goToResizing);
    homeLayout->addWidget(goToFaceDetection);
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

    // ==== Morphology Page ====
    QWidget* morphologyPage = new QWidget;
    QVBoxLayout* morphologyLayout = new QVBoxLayout;

    QLabel* morphImageLabel = new QLabel;
    morphImageLabel->setAlignment(Qt::AlignCenter);
    morphImageLabel->setMinimumSize(600, 400);
    morphImageLabel->setStyleSheet("border: 1px solid #ccc; background: white;");

    QLabel* morphStatusLabel = new QLabel("No image loaded");
    morphStatusLabel->setAlignment(Qt::AlignCenter);

    QPushButton* morphLoadButton = new QPushButton("📁 Load image");
    QPushButton* morphDilateButton = new QPushButton("➕ Apply Dilation");
    QPushButton* morphErodeButton = new QPushButton("➖ Apply Erosion");
    QPushButton* morphBackButton = new QPushButton("⬅️ Return to menu");

    QSlider* morphSlider = new QSlider(Qt::Horizontal);
    morphSlider->setRange(1, 20);
    morphSlider->setValue(2);
    QLabel* morphKernelLabel = new QLabel("Kernel size: 2");

    cv::Mat morphOriginalImage;
    cv::Mat morphResult;

    auto updateMorphology = [&](bool dilate) {
        if (morphOriginalImage.empty()) {
            morphStatusLabel->setText("❗ No image loaded");
            return;
        }
        int size = morphSlider->value();
        morphResult = applyMorphology(morphOriginalImage, size, dilate);
        QImage qimg((uchar*)morphResult.data, morphResult.cols, morphResult.rows, morphResult.step, QImage::Format_BGR888);
        morphImageLabel->setPixmap(QPixmap::fromImage(qimg).scaled(morphImageLabel->size(), Qt::KeepAspectRatio));
        morphStatusLabel->setText(dilate ? "✅ Dilation applied" : "✅ Erosion applied");
        };

    QObject::connect(morphLoadButton, &QPushButton::clicked, [&]() {
        QString path = QFileDialog::getOpenFileName(&window, "Load image", "", "Images (*.png *.jpg *.bmp)");
        if (!path.isEmpty()) {
            morphOriginalImage = cv::imread(path.toStdString());
            if (!morphOriginalImage.empty()) {
                QImage qimg((uchar*)morphOriginalImage.data, morphOriginalImage.cols, morphOriginalImage.rows, morphOriginalImage.step, QImage::Format_BGR888);
                morphImageLabel->setPixmap(QPixmap::fromImage(qimg).scaled(morphImageLabel->size(), Qt::KeepAspectRatio));
                morphStatusLabel->setText("✅ Image loaded");
            }
            else {
                morphStatusLabel->setText("❌ Failed to load image");
            }
        }
        });

    QObject::connect(morphDilateButton, &QPushButton::clicked, [&]() {
        updateMorphology(true);
        });
    QObject::connect(morphErodeButton, &QPushButton::clicked, [&]() {
        updateMorphology(false);
        });
    QObject::connect(morphSlider, &QSlider::valueChanged, [&](int value) {
        morphKernelLabel->setText("Kernel size: " + QString::number(value));
        });
    QObject::connect(morphBackButton, &QPushButton::clicked, [&]() {
        stackedWidget->setCurrentIndex(0);
        });

    morphologyLayout->addWidget(morphBackButton);
    morphologyLayout->addWidget(morphLoadButton);
    morphologyLayout->addWidget(morphDilateButton);
    morphologyLayout->addWidget(morphErodeButton);
    morphologyLayout->addWidget(morphKernelLabel);
    morphologyLayout->addWidget(morphSlider);
    morphologyLayout->addWidget(morphImageLabel);
    morphologyLayout->addWidget(morphStatusLabel);
    morphologyPage->setLayout(morphologyLayout);

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


        // ==== Page Image Resizing ====
    QWidget* resizePage = new QWidget;
    QVBoxLayout* resizeLayout = new QVBoxLayout;

    QLabel* resizeImageLabel = new QLabel;
    resizeImageLabel->setAlignment(Qt::AlignCenter);
    resizeImageLabel->setStyleSheet("border: 1px solid #ccc; background: white;");
    resizeImageLabel->setMinimumSize(600, 400);

    QLabel* resizeStatusLabel = new QLabel("No image loaded");
    resizeStatusLabel->setAlignment(Qt::AlignCenter);

    QPushButton* resizeLoadButton = new QPushButton("📁 Upload Image");
    QPushButton* resizeSaveButton = new QPushButton("💾 Save Image");
    QPushButton* resizeBackButton = new QPushButton("⬅️ Return to menu");

    QSlider* resizeSlider = new QSlider(Qt::Horizontal);
    resizeSlider->setRange(10, 300); // 0.1x à 3.0x
    resizeSlider->setValue(100);     // 1.0x par défaut
    QLabel* scaleLabel = new QLabel("Scale: 1.0x");

    double scaleFactor = 1.0;
    ImageResizer imageResizer;

    auto updateResizeImage = [&]() {
        std::cout << "[DEBUG] scaleFactor = " << scaleFactor << std::endl;
        imageResizer.resize(scaleFactor);
        resizeImageLabel->setPixmap(QPixmap::fromImage(imageResizer.getQImage()));
        scaleLabel->setText("Scale: " + QString::number(scaleFactor, 'f', 2) + "x");
        resizeStatusLabel->setText("🔄 Resize applied");
    };

    QObject::connect(resizeLoadButton, &QPushButton::clicked, [&]() {
        QString fileName = QFileDialog::getOpenFileName(&window, "Select an image", "", "Images (*.png *.jpg *.jpeg *.bmp)");
        if (!fileName.isEmpty()) {
            cv::Mat img = cv::imread(fileName.toStdString());

            if (!img.empty()) {
                imageResizer.setImage(img);
                scaleFactor = 1.0;
                resizeSlider->setValue(100);
                updateResizeImage();
                resizeStatusLabel->setText("✅ Image loaded");
            }
        }
    });

    QObject::connect(resizeSlider, &QSlider::valueChanged, [&](int value) {
        scaleFactor = value / 100.0;
        imageResizer.setScaleFactor(scaleFactor);
        updateResizeImage();
    });

    QObject::connect(resizeSaveButton, &QPushButton::clicked, [&]() {
        QString filePath = QFileDialog::getSaveFileName(&window, "Save image", "", "Images (*.png *.jpg *.bmp)");
        if (!filePath.isEmpty()) {
            cv::imwrite(filePath.toStdString(), imageResizer.getResizedMat());
            resizeStatusLabel->setText("💾 Image saved!");
        }
    });

    QObject::connect(resizeBackButton, &QPushButton::clicked, [&]() {
        stackedWidget->setCurrentWidget(homePage);
    });

    resizeLayout->addWidget(resizeBackButton);
    resizeLayout->addWidget(resizeLoadButton);
    resizeLayout->addWidget(resizeSaveButton);
    resizeLayout->addWidget(scaleLabel);
    resizeLayout->addWidget(resizeSlider);
    resizeLayout->addWidget(resizeImageLabel);
    resizeLayout->addWidget(resizeStatusLabel);
    resizePage->setLayout(resizeLayout);

    // ==== Face Detection Page ====
    QWidget* facePage = new QWidget;
    QVBoxLayout* faceLayout = new QVBoxLayout;

    QLabel* faceImageLabel = new QLabel;
    faceImageLabel->setAlignment(Qt::AlignCenter);
    faceImageLabel->setMinimumSize(600, 400);
    faceImageLabel->setStyleSheet("border: 1px solid #ccc; background: white;");

    QLabel* faceStatusLabel = new QLabel("No image loaded");
    faceStatusLabel->setAlignment(Qt::AlignCenter);

    QPushButton* faceLoadButton = new QPushButton("📁 Load image");
    QPushButton* faceDetectButton = new QPushButton("🔍 Detect faces, smiles and cats");
    QPushButton* faceBackButton = new QPushButton("⬅️ Return to menu");

    cv::Mat faceOriginalImage;
    cv::Mat faceDetectedImage;

    QObject::connect(faceLoadButton, &QPushButton::clicked, [&]() {
        QString path = QFileDialog::getOpenFileName(&window, "Load image", "", "Images (*.png *.jpg *.bmp)");
        if (!path.isEmpty()) {
            faceOriginalImage = cv::imread(path.toStdString());
            if (!faceOriginalImage.empty()) {
                QImage qimg((uchar*)faceOriginalImage.data, faceOriginalImage.cols, faceOriginalImage.rows, faceOriginalImage.step, QImage::Format_BGR888);
                faceImageLabel->setPixmap(QPixmap::fromImage(qimg).scaled(faceImageLabel->size(), Qt::KeepAspectRatio));
                faceStatusLabel->setText("✅ Image loaded");
            }
            else {
                faceStatusLabel->setText("❌ Failed to load image");
            }
        }
        });

    QObject::connect(faceDetectButton, &QPushButton::clicked, [&]() {
        if (!faceOriginalImage.empty()) {
            faceDetectedImage = applyDetection(faceOriginalImage);
            QImage qimg((uchar*)faceDetectedImage.data, faceDetectedImage.cols, faceDetectedImage.rows, faceDetectedImage.step, QImage::Format_BGR888);
            faceImageLabel->setPixmap(QPixmap::fromImage(qimg).scaled(faceImageLabel->size(), Qt::KeepAspectRatio));
            faceStatusLabel->setText("😊 Detection applied");
        }
        else {
            faceStatusLabel->setText("❗ No image loaded");
        }
        });

    QObject::connect(faceBackButton, &QPushButton::clicked, [&]() {
        stackedWidget->setCurrentIndex(0);
        });

    faceLayout->addWidget(faceBackButton);
    faceLayout->addWidget(faceLoadButton);
    faceLayout->addWidget(faceDetectButton);
    faceLayout->addWidget(faceImageLabel);
    faceLayout->addWidget(faceStatusLabel);
    facePage->setLayout(faceLayout);

    // ==== Add pages to the Home Page ====
    stackedWidget->addWidget(homePage);         // index 0
    stackedWidget->addWidget(cannyPage);        // index 1
    stackedWidget->addWidget(morphologyPage);   // index 2
    stackedWidget->addWidget(backgroundPage);   // index 3
    stackedWidget->addWidget(resizePage);       // index 4
    stackedWidget->addWidget(facePage);         // index 5

    QObject::connect(goToCanny, &QPushButton::clicked, [&]() {
        stackedWidget->setCurrentIndex(1);
        });
    QObject::connect(goToMorphology, &QPushButton::clicked, [&]() {
        stackedWidget->setCurrentIndex(2);
        });
    QObject::connect(goToBackground, &QPushButton::clicked, [&]() {
        stackedWidget->setCurrentIndex(3);
        });
    QObject::connect(goToResizing, &QPushButton::clicked, [&]() {
        stackedWidget->setCurrentIndex(4);
        });
    QObject::connect(goToFaceDetection, &QPushButton::clicked, [&]() {
        stackedWidget->setCurrentIndex(5);
        });

    window.setLayout(new QVBoxLayout);
    window.layout()->addWidget(stackedWidget);
    window.showMaximized();

    return app.exec();
}