#include "image_morphology.h"

cv::Mat Morphology::applyMorphology(const cv::Mat& input, int size, bool dilate)
{
    cv::Mat output;
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT,
        cv::Size(2 * size + 1, 2 * size + 1),
        cv::Point(size, size));  // create rectangular kernel centered at (size, size)

    if (dilate)
        cv::dilate(input, output, element);  // apply dilation
    else
        cv::erode(input, output, element);  // apply erosion

    return output;  // return result
}
