#include "imageoverlay.h"

int ImageOverlay::set_template_image(Image* template_image)
{
    this->template_image = template_image;

    // return the required count of images:
    return this->template_image->get_card_positions_size();
}

void ImageOverlay::set_images_to_overlay(QVector<Image*> images_to_overlay)
{
    this->images_to_overlay = images_to_overlay;
}

ImageOverlay::OverlayedResult ImageOverlay::overlay()
{
    OverlayedResult result;
    cv::Mat overlayed_image;
    cv::cvtColor(template_image->get_opencv_image_object(), overlayed_image, cv::COLOR_RGB2BGRA);
    double blending_factor = 1;

    for (int i = 0; i < images_to_overlay.size(); i++) {
        result.input_overlay_positions.push_back(QPair<std::string, QPair<cv::Point2f, cv::Point2f>>(
            images_to_overlay.value(i)->get_file_path(),
            get_bounding_boxes(this->template_image->get_card_positions_perspective(i))
        ));

        cv::Mat transparent_input_image;
        cv::cvtColor(images_to_overlay.value(i)->get_opencv_image_object(), transparent_input_image, cv::COLOR_RGB2BGRA);

        cv::Mat warped_image;

        cv::warpPerspective(
            transparent_input_image,
            warped_image,
            create_perspective_matrix_for_image(i),
            get_desired_size(i),
            cv::INTER_LINEAR,
            cv::BORDER_TRANSPARENT
        );

        cv::Mat mask = create_mask_from_alpha(warped_image);

        // Overlay the non-transparent regions of the warped image onto the overlayed image
        cv::Mat blended_image;
        cv::bitwise_and(overlayed_image, overlayed_image, blended_image, mask);
        cv::bitwise_or(blended_image, warped_image, overlayed_image);

        // Update the blending factor for subsequent images
        blending_factor *= (1.0 - blending_factor);
    }

    result.overlayed_image = overlayed_image;
    result.image_width = template_image->get_opencv_image_object().cols;
    result.image_height = template_image->get_opencv_image_object().rows;
    return result;
}

cv::Mat ImageOverlay::create_perspective_matrix_for_image(int image_index)
{
    cv::Mat image = images_to_overlay.value(image_index)->get_opencv_image_object();

    std::vector<cv::Point2f> image_perspective_points;
    image_perspective_points.push_back(cv::Point2f(0, 0));
    image_perspective_points.push_back(cv::Point2f(image.cols, 0));
    image_perspective_points.push_back(cv::Point2f(image.cols, image.rows));
    image_perspective_points.push_back(cv::Point2f(0, image.rows));

    return cv::getPerspectiveTransform(
        image_perspective_points,
        this->template_image->get_card_positions_perspective(image_index)
    );
}

cv::Size ImageOverlay::get_desired_size(int image_index)
{
    cv::Size desired_size;
    desired_size.width = this->template_image->get_opencv_image_object().cols;
    desired_size.height = this->template_image->get_opencv_image_object().rows;

    return desired_size;
}

cv::Mat ImageOverlay::create_mask_from_alpha(const cv::Mat& alpha)
{
    cv::Mat mask(alpha.size(), CV_8UC1);

    for (int i = 0; i < alpha.rows; i++)
    {
        for (int j = 0; j < alpha.cols; j++)
        {
            if (alpha.at<cv::Vec4b>(i, j)[3] != 0)
                mask.at<uchar>(i, j) = 255;  // Set as non-zero (white) for non-transparent pixels
            else
                mask.at<uchar>(i, j) = 0;    // Set as zero (black) for transparent pixels
        }
    }

    cv::threshold(mask, mask, 1, 255, cv::THRESH_BINARY_INV);
    return mask;
}

QPair<cv::Point2f, cv::Point2f> ImageOverlay::get_bounding_boxes(std::vector<cv::Point2f> points)
{
    float min_x_pos = 9999.0;
    float min_y_pos = 9999.0;
    float max_x_pos = 0.0;
    float max_y_pos = 0.0;
    for (auto& point : points) {
        if (point.x < min_x_pos) min_x_pos = point.x;
        else if (point.x > max_x_pos) max_x_pos = point.x;

        if (point.y < min_y_pos) min_y_pos = point.y;
        else if (point.y > max_y_pos) max_y_pos = point.y;
    }

    return QPair<cv::Point2f, cv::Point2f>(cv::Point2f(min_x_pos, min_y_pos), cv::Point2f(max_x_pos, max_y_pos));
}
