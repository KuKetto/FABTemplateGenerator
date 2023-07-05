#ifndef AUGMENTATION_H
#define AUGMENTATION_H

#include <opencv2/opencv.hpp>
#include "../containers/image.h"
#include "../utils/random.h"

class Augmentation
{
public:
    Augmentation(){};

    void apply_noise(Image* image_container, double factor);
    void apply_lens_blur(Image* image_container, double radius);
    void apply_bilateral_blur(Image* image_container, int diameter, double sigma_color, double sigma_space);

    void flip_image(Image* image_container);
    void cut_out_image(Image* image_container);

    void adjust_brightness(Image* image_container);
    void shift_rgb(Image* image_container);
};

#endif // AUGMENTATION_H
