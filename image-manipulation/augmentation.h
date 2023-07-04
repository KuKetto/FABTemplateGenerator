#ifndef AUGMENTATION_H
#define AUGMENTATION_H

#include <opencv2/opencv.hpp>
#include "../containers/image.h"

class Augmentation
{
public:
    Augmentation(){};

    void apply_noise(Image* image_container, double factor);
    void apply_lens_blur(Image* image_container, double radius);
    void apply_bilateral_blur(Image* image_container, int diameter, double sigma_color, double sigma_space);

    void flip_image(cv::Mat* image);
    void cut_out_image(cv::Mat* image);

    void adjust_brightness(cv::Mat* image);
    void shift_rgb(cv::Mat* image);
};

#endif // AUGMENTATION_H
