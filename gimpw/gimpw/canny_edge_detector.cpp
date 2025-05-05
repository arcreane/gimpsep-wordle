#include "canny_edge_detector.h"

CannyEdgeDetector::CannyEdgeDetector(double t1, double t2)
	: threshold1(t1), threshold2(t2) {
};

bool CannyEdgeDetector::loadImage(const string& imageName) {
	image = imread(imageName, IMREAD_GRAYSCALE);
	if (image.empty()) {
		cerr << "Error: Could not open or find the image!" << endl;
		return false;
	}
	return true;
};

void CannyEdgeDetector::detectEdges() {
	Canny(image, edges, threshold1, threshold2);
}

Mat CannyEdgeDetector::getEdges() {
	return edges;
};

void onTrackbarChange(int, void* userdata) {
	auto* detector = static_cast<CannyEdgeDetector*>(userdata);
	detector->detectEdges();
	cv::imshow("Canny Edges", detector->getEdges());
}

void CannyEdgeDetector::manipulateImage() {
	int t1 = static_cast<int>(threshold1);
	int t2 = static_cast<int>(threshold2);

	cv::namedWindow("Canny Edges", cv::WINDOW_AUTOSIZE);

	cv::createTrackbar("Threshold 1", "Canny Edges", &t1, 500, onTrackbarChange, this);
	cv::createTrackbar("Threshold 2", "Canny Edges", &t2, 500, onTrackbarChange, this);

	detectEdges();
	cv::imshow("Canny Edges", edges);

	while (true) {
		int key = cv::waitKey(30);
		if (key == 's') {
			std::string outputPath;
			std::cout << "Enter the path of the output image : ";
			std::cin >> outputPath;
			saveEdges(outputPath);
			std::cout << "Image saved.\n";
		}
		else if (key == 27 || key == 'q') { 
			break;
		}

		threshold1 = static_cast<double>(cv::getTrackbarPos("Threshold 1", "Canny Edges"));
		threshold2 = static_cast<double>(cv::getTrackbarPos("Threshold 2", "Canny Edges"));

		detectEdges();
		cv::imshow("Canny Edges", edges);
	}

	cv::destroyWindow("Canny Edges");
}

void CannyEdgeDetector::saveEdges(const string& output) {
	imwrite(output, edges);
}