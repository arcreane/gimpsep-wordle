#include "detection_and_morphology.h"
#include "canny_edge_detector.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include "image_resize.h"

int main() {
	std::string imagePath;
	std::cout << "=== Resize Image Program ===\n";

	// 1. Ask for the path to the image
	std::cout << "Enter the image path: ";
	std::cin >> imagePath;

	// 2. Upload image
	cv::Mat input = cv::imread(imagePath);
	if (input.empty()) {
		std::cerr << "Error: Could not load image from " << imagePath << "\n";
		return 1;
	}

	// 3. Request resizing factors
	double fx, fy;
	std::cout << "Enter width scale factor (fx): ";
	std::cin >> fx;
	std::cout << "Enter height scale factor (fy): ";
	std::cin >> fy;

	// 4. Resize the image using your
	ImageResizer resizer;
	cv::Mat resized = resizer.resize(input, fx, fy);

	// 5. Display results
	cv::imshow("Resized Image", resized);
	cv::waitKey(0);

	// 6. Save the resized image
	cv::imwrite("resized_output.jpg", resized);
	std::cout << "Image saved as 'resized_output.jpg'.\n";

	return 0;
}
