#include "rgbshiftaugmentation.h"

RGBShiftAugmentation::RGBShiftAugmentation(Image *image_container, const double &red_constant, const double &green_constant, const double &blue_constant):
    red_constant(red_constant), green_constant(green_constant), blue_constant(blue_constant), Augmentation(image_container)
{

}

void RGBShiftAugmentation::augment()
{
    if (red_constant < 0.75 || red_constant > 1.25)
        throw InvalidValueException("Augmentation::shift_rgb","between 0.75 and 1.25", "red constant of " + std::to_string(red_constant));
    if (green_constant < 0.75 || green_constant > 1.25)
        throw InvalidValueException("Augmentation::shift_rgb","between 0.75 and 1.25", "green constant of " + std::to_string(green_constant));
    if (blue_constant < 0.75 || blue_constant > 1.25)
        throw InvalidValueException("Augmentation::shift_rgb","between 0.75 and 1.25", "blue constant of " + std::to_string(blue_constant));

    cv::Mat image = image_container->get_opencv_image_object();
    cv::Vec3d rgb_shift_values = {red_constant, green_constant, blue_constant};
    for (int i = 0; i < image.rows; i++)
        for (int j = 0; j < image.cols; j++) {
            cv::Vec3b& pixel = image.at<cv::Vec3b>(i, j);
            cv::Vec3f pixel_float(pixel[0], pixel[1], pixel[2]);
            for (int k = 0; k < 3; k++) {
                float value_float = pixel_float[k] * rgb_shift_values[k];
                if (value_float > 255.0f) value_float = 255.0f;
                if (value_float < 0.0f) value_float = 0.0f;
                pixel[k] = static_cast<uchar>(value_float);
            }
        }

    image_container->set_opencv_image_object(image);
}
