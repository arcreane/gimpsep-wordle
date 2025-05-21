#include "video_manipulation.h"
#include <QFileDialog>
#include <QDebug>

VideoManipulation::VideoManipulation(QWidget *parent) : QWidget(parent), currentFrame(0), frameDelay(33) {
    // === Initialisation du layout principal ===
    layout = new QVBoxLayout;

    // === Widgets vidéo ===
    videoLabel = new QLabel("No video loaded");
    videoLabel->setAlignment(Qt::AlignCenter);
    videoLabel->setMinimumSize(800, 500);
    videoLabel->setStyleSheet("border: 1px solid #ccc; background-color: black;");

    // === Bouton de chargement ===
    loadButton = new QPushButton("Load Video");

    // === Slider de frame ===
    frameSlider = new QSlider(Qt::Horizontal);
    frameSlider->setEnabled(false);

    // === Slider de vitesse ===
    speedSlider = new QSlider(Qt::Horizontal);
    speedSlider->setRange(10, 200);
    speedSlider->setValue(100);
    speedLabel = new QLabel("Speed: x1.0");

    // === Boutons lecture / pause ===
    playButton = new QPushButton("▶️ Play");
    pauseButton = new QPushButton("⏸️ Pause");

    // === Layout horizontal pour lecture/pause ===
    QHBoxLayout* controlsLayout = new QHBoxLayout;
    controlsLayout->addWidget(playButton);
    controlsLayout->addWidget(pauseButton);

    // === Ajout des widgets au layout principal ===
    layout->addWidget(loadButton);
    layout->addWidget(videoLabel);
    layout->addWidget(new QLabel("Frame Slider"));
    layout->addWidget(frameSlider);
    layout->addWidget(new QLabel("Speed (ms per frame)"));
    layout->addWidget(speedSlider);
    layout->addWidget(speedLabel);
    layout->addLayout(controlsLayout);

    // === Finaliser le layout ===
    setLayout(layout);

    // === Timer ===
    timer = new QTimer(this);

    // === Connexions ===
    connect(timer, &QTimer::timeout, this, &VideoManipulation::updateFrame);
    connect(loadButton, &QPushButton::clicked, this, &VideoManipulation::loadVideo);
    connect(frameSlider, &QSlider::valueChanged, this, &VideoManipulation::changeFrame);
    connect(speedSlider, &QSlider::valueChanged, this, &VideoManipulation::changeSpeed);
    connect(playButton, &QPushButton::clicked, [&]() {
        if (video.isOpened()) {
            timer->start(frameDelay);
        }
    });
    connect(pauseButton, &QPushButton::clicked, [&]() {
        timer->stop();
    });
}


void VideoManipulation::loadVideo() {
    QString filename = QFileDialog::getOpenFileName(this, "Open Video", "", "Video Files (*.avi *.mp4 *.mov)");
    if (filename.isEmpty()) return;

    video.open(filename.toStdString());
    if (!video.isOpened()) {
        videoLabel->setText("Failed to open video.");
        return;
    }

    totalFrames = static_cast<int>(video.get(cv::CAP_PROP_FRAME_COUNT));
    frameSlider->setRange(0, totalFrames - 1);
    frameSlider->setEnabled(true);

    currentFrame = 0;
    video.set(cv::CAP_PROP_POS_FRAMES, currentFrame);
    showFrame(currentFrame);

    timer->start(frameDelay);
}



void VideoManipulation::updateFrame() {
    cv::Mat frame;
    if (!video.read(frame)) {
        timer->stop();
        return;
    }
    currentFrame++;
    frameSlider->blockSignals(true);
    frameSlider->setValue(currentFrame);
    frameSlider->blockSignals(false);

    QImage qimg = matToQImage(frame);
    videoLabel->setPixmap(QPixmap::fromImage(qimg).scaled(videoLabel->size(), Qt::KeepAspectRatio));
}


void VideoManipulation::changeFrame(int value) {
    if (!video.isOpened()) return;
    video.set(cv::CAP_PROP_POS_FRAMES, value);
    currentFrame = value;

    cv::Mat frame;
    if (video.read(frame) && !frame.empty()) {
        QImage qimg = matToQImage(frame);
        videoLabel->setPixmap(QPixmap::fromImage(qimg).scaled(videoLabel->size(), Qt::KeepAspectRatio));
    }
}


void VideoManipulation::changeSpeed(int value) {
    int maxValue = speedSlider->maximum();
    int minValue = speedSlider->minimum();
    int inverted = maxValue - (value - minValue);

    frameDelay = inverted;
    timer->setInterval(frameDelay);

    float speedFactor = 33.0f / frameDelay;
    speedLabel->setText(QString("Speed: x%1").arg(QString::number(speedFactor, 'f', 1)));
}



void VideoManipulation::showFrame(int frameNumber) {
    if (!video.isOpened()) return;

    video.set(cv::CAP_PROP_POS_FRAMES, frameNumber);
    cv::Mat frame;
    if (!video.read(frame) || frame.empty()) return;

    QImage qimg = matToQImage(frame);
    videoLabel->setPixmap(QPixmap::fromImage(qimg).scaled(videoLabel->size(), Qt::KeepAspectRatio));
}


QImage VideoManipulation::matToQImage(const cv::Mat &mat) {
    cv::Mat rgb;
    if (mat.channels() == 3)
        cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
    else
        cv::cvtColor(mat, rgb, cv::COLOR_GRAY2RGB);
    return QImage((const uchar*) rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888).copy();
}


