#include "lensbluraugmentation.h"

LensBlurAugmentation::LensBlurAugmentation(Image *image_container, const double &radius):
    radius(radius), Augmentation(image_container)
{

}

void LensBlurAugmentation::augment()
{
    cv::Mat image = image_container->get_opencv_image_object();

    cv::Mat blurred_image;
    cv::GaussianBlur(image, blurred_image, cv::Size(), radius);

    image_container->set_opencv_image_object(blurred_image);
}

