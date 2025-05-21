#ifndef VIDEO_MANIPULATION_H
#define VIDEO_MANIPULATION_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QTimer>
#include <opencv2/opencv.hpp>

class VideoManipulation : public QWidget {
    Q_OBJECT

public:
    explicit VideoManipulation(QWidget *parent = nullptr);

    private slots:
        void loadVideo();
    void updateFrame();
    void changeFrame(int value);
    void changeSpeed(int value);

private:
    QLabel *videoLabel;
    QSlider *frameSlider;
    QSlider *speedSlider;
    QLabel* speedLabel;
    QPushButton *loadButton;
    QPushButton* playButton;
    QPushButton* pauseButton;
    QVBoxLayout *layout;
    QTimer *timer;

    cv::VideoCapture video;
    int totalFrames;
    int currentFrame;
    int frameDelay;

    void showFrame(int frameNumber);
    QImage matToQImage(const cv::Mat &mat);

};

#endif
