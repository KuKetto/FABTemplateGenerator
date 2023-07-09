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
    QPair<cv::Point2f, cv::Point2f> get_bounding_boxes(std::vector<cv::Point2f> points);
public:
    struct OverlayedResult {
        cv::Mat overlayed_image;
        int image_width;
        int image_height;
        QVector<QPair<std::string, QPair<cv::Point2f, cv::Point2f>>> input_overlay_positions;
    };

    ImageOverlay(){};
    int set_template_image(Image* template_image);
    void set_images_to_overlay(QVector<Image*> images_to_overlay);
    OverlayedResult overlay();
};

#endif // IMAGEOVERLAY_H
