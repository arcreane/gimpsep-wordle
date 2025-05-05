#ifndef CANNY_EDGE_DETECTOR_H
#define CANNY_EDGE_DETECTOR_H

#include <opencv2/opencv.hpp>
#include <string>

using namespace std;
using namespace cv;

class CannyEdgeDetector
{
	private:
		Mat image, edges;
		double threshold1, threshold2;

	public:
		CannyEdgeDetector(double t1 = 100.0, double t2 = 200.0);
		bool loadImage(const string& imageName);
		void detectEdges();
		Mat getEdges();
		void manipulateImage();
		void saveEdges(const string& output);
};

#endif
