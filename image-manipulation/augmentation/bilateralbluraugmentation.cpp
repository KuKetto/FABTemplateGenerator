#include "bilateralbluraugmentation.h"

BilateralBlurAugmentation::BilateralBlurAugmentation(Image *image_container, const int &diameter, const double &sigma_color, const double &sigma_space):
    diameter(diameter), sigma_color(sigma_color), sigma_space(sigma_space), Augmentation(image_container)
{

}

void BilateralBlurAugmentation::augment()
{
    cv::Mat image = image_container->get_opencv_image_object();
    cv::Mat blurred_image;

    cv::bilateralFilter(image, blurred_image, diameter, sigma_color, sigma_space);

    image_container->set_opencv_image_object(blurred_image);
}
