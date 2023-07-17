#include "imageflipaugmentation.h"

ImageFlipAugmentation::ImageFlipAugmentation(Image *image_container):
    Augmentation(image_container)
{

}

void ImageFlipAugmentation::augment()
{
    cv::Mat flipped_image;
    cv::flip(image_container->get_opencv_image_object(), flipped_image, 0);
    image_container->set_opencv_image_object(flipped_image);
}
