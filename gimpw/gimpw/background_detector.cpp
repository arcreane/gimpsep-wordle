#include "background_detector.h"

static Mat image, tempImage;
static Rect selectedRect;
static bool drawing = false;
static Point point1, point2;

void mouseHandler(int event, int x, int y, int, void*) {
    if (event == EVENT_LBUTTONDOWN) {
        drawing = true;
        point1 = Point(x, y);
        point2 = point1;
    }
    else if (event == EVENT_MOUSEMOVE && drawing) {
        point2 = Point(x, y);
        tempImage = image.clone();
        rectangle(tempImage, point1, point2, Scalar(0, 255, 0), 2);
        imshow("Draw Rectangle", tempImage);
    }
    else if (event == EVENT_LBUTTONUP) {
        drawing = false;
        point2 = Point(x, y);
        selectedRect = Rect(point1, point2);
        if (selectedRect.width > 0 && selectedRect.height > 0) {
            rectangle(image, selectedRect, Scalar(0, 255, 0), 2);
            imshow("Draw Rectangle", image);
        }
    }
}

void runBackgroundDetection() {
    string imagePath;
    cout << "Enter the path of the image: ";
    cin >> imagePath;

    Mat original = imread(imagePath);
    if (original.empty()) {
        cerr << "Error loading image!" << endl;
        return;
    }

    bool validRectangle = false;

    while (true) {
        image = original.clone();
        tempImage = image.clone();
        selectedRect = Rect();
        validRectangle = false;

        namedWindow("Draw Rectangle");
        setMouseCallback("Draw Rectangle", mouseHandler);

        cout << "Draw a rectangle with the mouse around the subject.\n"
            << "- Press ENTER to confirm\n"
            << "- Press 'r' to reset the rectangle\n"
            << "- Press ESC to cancel\n" << endl;

        imshow("Draw Rectangle", image);

        while (true) {
            int key = waitKey(0);
            if (key == 13 || key == 10) {
                if (selectedRect.width > 0 && selectedRect.height > 0) {
                    validRectangle = true;
                }
                else {
                    cout << "No valid rectangle drawn. Try again." << endl;
                    break;
                }
                break;
            }
            else if (key == 'r' || key == 'R') {
                cout << "Resetting rectangle..." << endl;
                destroyWindow("Draw Rectangle");
                break;
            }
            else if (key == 27) {
                cout << "Canceled by user." << endl;
                destroyWindow("Draw Rectangle");
                return;
            }
        }

        if (validRectangle) break;
    }

    destroyWindow("Draw Rectangle");

    BackgroundDetector detector;
    detector.apply(image, selectedRect);

    Mat& resultMask = detector.getMaskRef();
    Mat foreground = detector.getForeground(image);

    imshow("Final Result (Preview)", foreground);
    cout << "\nHere is the final result. Press any key to continue..." << endl;
    waitKey(0);
    destroyWindow("Final Result (Preview)");

    string outputFile;
    cout << "\n Enter output filename (e.g. output.png): ";
    cin >> outputFile;

    detector.saveForegroundAsPNG(image, outputFile);
}

BackgroundDetector::BackgroundDetector() {}

void BackgroundDetector::apply(const Mat& image, const cv::Rect& rect) {
    Mat bgModel, fgModel;
    Mat mask(image.size(), CV_8UC1, Scalar(GC_BGD));
    mask(rect).setTo(Scalar(GC_PR_FGD));

    grabCut(image, mask, rect, bgModel, fgModel, 5, GC_INIT_WITH_RECT);
    resultMask = (mask == GC_FGD) | (mask == GC_PR_FGD);
    resultMask.convertTo(resultMask, CV_8UC1, 255);
}

Mat BackgroundDetector::getForeground(const Mat& image) const {
    Mat foreground;
    image.copyTo(foreground, resultMask);
    return foreground;
}

void BackgroundDetector::saveForegroundAsPNG(const Mat& originalImage, const string& outputPath) const {
    if (resultMask.empty()) {
        cerr << "Mask is empty, cannot save PNG." << endl;
        return;
    }

    Mat foreground;
    originalImage.copyTo(foreground, resultMask);

    Mat alpha;
    resultMask.copyTo(alpha);

    vector<Mat> bgrChannels;
    split(foreground, bgrChannels);
    bgrChannels.push_back(alpha);

    Mat outputWithAlpha;
    merge(bgrChannels, outputWithAlpha);

    if (!imwrite(outputPath, outputWithAlpha)) {
        cerr << "Failed to write PNG file." << endl;
    }
    else {
        cout << "Image saved as: " << outputPath << endl;
    }
}

Mat& BackgroundDetector::getMaskRef() {
    return resultMask;
}
