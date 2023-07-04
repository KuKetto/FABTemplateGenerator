#ifndef IMAGEOVERLAY_H
#define IMAGEOVERLAY_H

#include "../containers/image.h"
#include <cstdlib>

class ImageOverlay
{
    Image* template_image;
    QVector<Image*> images_to_overlay;

    cv::Mat create_perspective_matrix_for_image(int image_index);
    cv::Size get_desired_size(int image_index);
    cv::Mat create_mask_from_alpha(const cv::Mat& alpha);
public:
    ImageOverlay(){};
    int set_template_image(Image* template_image);
    void set_images_to_overlay(QVector<Image*> images_to_overlay);
    cv::Mat overlay();
};

#endif // IMAGEOVERLAY_H
