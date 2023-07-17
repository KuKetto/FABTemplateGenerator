#include "brightnessadjustionaugmentation.h"

BrightnessAdjustionAugmentation::BrightnessAdjustionAugmentation(Image *image_container, const double &brightness_constant):
    brightness_constant(brightness_constant), Augmentation(image_container)
{

}

void BrightnessAdjustionAugmentation::augment()
{
    if (brightness_constant < 0.5 || brightness_constant > 1.5)
        throw InvalidValueException("Augmentation::adjust_brightness","between 0.5 and 1.5", std::to_string(brightness_constant));

    cv::Mat image = image_container->get_opencv_image_object();
    image *= brightness_constant;

    // Clamp pixel values to the valid range of 0 to 255
    cv::Mat clamped_image = cv::max(cv::min(image, 255.0f), 0.0f);

    image_container->set_opencv_image_object(clamped_image);
}
