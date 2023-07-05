#ifndef AUGMENTATION_H
#define AUGMENTATION_H

#include <opencv2/opencv.hpp>
#include "../containers/image.h"
#include "../utils/random.h"
#include "../utils/exceptions/invalidvalueexception.h"

class Augmentation
{
public:
    Augmentation(){};

    void apply_noise(Image* image_container, double factor);
    void apply_lens_blur(Image* image_container, double radius);
    void apply_bilateral_blur(Image* image_container, int diameter, double sigma_color, double sigma_space);

    void flip_image(Image* image_container);
    void cut_out_image(Image* image_container, double radius);

    /** @brief Adjust the brightness of an image by a constant
     * Brightness modification works by adding or substracting a constant
     * value in each invidual pixel. Here we adjust the brightness between
     * 50% and 150%.
     * This is implemented for 3 channel images (CV_8UC3 - RGB / BGR only)
     *
     * @param Image container pointer
     * @param A value between 0.5 and 1.5
     *
     * @throws Utils::Exceptions::InvalidValueException if a value less than 0.5 or greater than 1.5 was provided
     */
    void adjust_brightness(Image* image_container, double brightness_constant);

    /** @brief Shift RGB value of an image
     * RGB shift works by shifting the respective red-green-blue values
     * by a different constant. This shift is limited between 75% and 125%
     *
     * This is implemented for 3 channel image (CV_8UC3 - RGB)
     *
     * @param Image container pointer
     * @param Red constant value between 0.75 and 1.25
     * @param Green constant value between 0.75 and 1.25
     * @param Blue constant value between 0.75 and 1.25
     *
     * @throws Utils::Exceptions::InvalidValueException if a value less than 0.75 or greater than 1.25 was provided
     */
    void shift_rgb(Image* image_container, double red_constant, double green_constant, double blue_constant);
};

#endif // AUGMENTATION_H
