#include "imageoverlay.h"

ImageOverlay::ImageOverlay()
{
    template_image = nullptr;
    images_to_overlay = QVector<Image*>();
    were_input_images_closed = QVector<bool>();
}

ImageOverlay::~ImageOverlay()
{
    try {
        if (template_image) {
            if (was_template_image_closed && template_image->is_open()) template_image->close();
        }
        if (!images_to_overlay.empty()) {
            for (int index = 0; index < images_to_overlay.size() && index < were_input_images_closed.size(); index++) {
                auto& image = images_to_overlay.at(index);
                if (image && were_input_images_closed.at(index) && image->is_open()) {
                    image->close();
                }
            }
        }
    } catch (...) {
        // images were deleted beforehand, nothing more to do
    }
}

int ImageOverlay::set_template_image(Image* template_image)
{
    if (!template_image) throw InvalidValueException("ImageOverlay::set_template_image", "template image", "a nullpointer");

    if (!template_image->is_open()) {
        template_image->open();
        was_template_image_closed = true;
    }

    // BadUsageException is thrown below if the template_image was not marked as a template image
    int required_image_count = template_image->get_card_positions_size();

    if (required_image_count == 0) {
        if (was_template_image_closed) {
            template_image->close();
            was_template_image_closed = false;
        }
        throw InvalidTemplateException("ImageOverlay::set_template_image", "The template image required 0 input image.");
    }

    this->template_image = template_image;

    return required_image_count;
}

void ImageOverlay::set_images_to_overlay(QVector<Image*> images_to_overlay)
{
    if (!template_image) throw BadUsageException("ImageOverlay::set_images_to_overlay", "Template image must be set before calling this method");

    if (!template_image->is_open()) {
        qDebug() << "WARNING! In ImageOverlay::overlay method. The given template image hasn't been open. "
                    "This is not the intended usecase, will attempt to open and close the template anyway.";
        template_image->open();
        was_template_image_closed = true;
    }

    if (images_to_overlay.size() != template_image->get_card_positions_size())
        throw InvalidValueException("ImageOverlay::set_images_to_overlay",
                                    " of " + std::to_string(template_image->get_card_positions_size()) + " (number of required images)",
                                    std::to_string(images_to_overlay.size()) + " (number of images provided)");
    this->images_to_overlay = images_to_overlay;
}

ImageOverlay::OverlayedResult ImageOverlay::overlay()
{
    if (!template_image) throw BadUsageException("ImageOverlay::overlay()", "The template image hasn't been set");
    if (images_to_overlay.empty()) throw BadUsageException("ImageOverlay::overlay()", "The input images haven't been set");

    if (!template_image->is_open()) {
        qDebug() << "WARNING! In ImageOverlay::overlay method. The given template image hasn't been open. "
                    "This is not the intended usecase, will attempt to open and close the template anyway.";
        template_image->open();
        was_template_image_closed = true;
    }

    for (int i = 0; i < images_to_overlay.size(); i++) {
        if (!images_to_overlay.at(i)->is_open()) {
            qDebug() << "WARNING! In ImageOverlay::overlay method. An input overlay image hasn't been open. "
                        "This is not the intended usecase, will attempt to open and close the input image anyway.";
            images_to_overlay[i]->open();
            were_input_images_closed.append(true);
        } else {
            were_input_images_closed.append(false);
        }
    }
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
            get_desired_size(),
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

    if (was_template_image_closed) {
        template_image->close();
        was_template_image_closed = false;
    }
    for (int i = 0; i < images_to_overlay.size(); i++)
        if (were_input_images_closed.at(i)) {
            images_to_overlay[i]->close();
            were_input_images_closed[i] = false;
        }

    return result;
}

cv::Mat ImageOverlay::create_perspective_matrix_for_image(const int& image_index) const
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

cv::Size ImageOverlay::get_desired_size() const
{
    cv::Size desired_size;
    desired_size.width = this->template_image->get_opencv_image_object().cols;
    desired_size.height = this->template_image->get_opencv_image_object().rows;

    return desired_size;
}

cv::Mat ImageOverlay::create_mask_from_alpha(const cv::Mat& alpha) const
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

QPair<cv::Point2f, cv::Point2f> ImageOverlay::get_bounding_boxes(const std::vector<cv::Point2f>& points) const
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
