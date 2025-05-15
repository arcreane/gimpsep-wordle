#include "detection_and_morphology.h"
#include "canny_edge_detector.h"
#include <iostream>

#include <iostream>
#include <opencv2/opencv.hpp>
#include "image_resize.h"

int main() {
	std::string imagePath;
	std::cout << "=== Resize Image Program ===\n";

	// 1. Demander le chemin de l’image
	std::cout << "Enter the image path: ";
	std::cin >> imagePath;

	// 2. Charger l’image
	cv::Mat input = cv::imread(imagePath);
	if (input.empty()) {
		std::cerr << "Error: Could not load image from " << imagePath << "\n";
		return 1;
	}

	// 3. Demander les facteurs de redimensionnement
	double fx, fy;
	std::cout << "Enter width scale factor (fx): ";
	std::cin >> fx;
	std::cout << "Enter height scale factor (fy): ";
	std::cin >> fy;

	// 4. Redimensionner l’image avec ta fonction
	ImageResizer resizer;
	cv::Mat resized = resizer.resize(input, fx, fy);

	// 5. Afficher le résultat
	cv::imshow("Resized Image", resized);
	cv::waitKey(0);

	// 6. Sauvegarder l’image redimensionnée
	//cv::imwrite("resized_output.jpg", resized);
	//std::cout << "Image saved as 'resized_output.jpg'.\n";

	return 0;
}
