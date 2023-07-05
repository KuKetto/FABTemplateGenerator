#include "augmentation.h"

void Augmentation::apply_noise(Image* image_container, double factor)
{
    // Set the mean and standard deviation for the Gaussian noise
    double mean = 0.0;

    cv::Mat image = image_container->get_opencv_image_object();

    // Initialize the noise matrix with the same size and type as the original image
    cv::Mat noise(image.size(), image.type());

    // Generate Gaussian noise
    cv::randn(noise, cv::Scalar::all(mean), cv::Scalar::all(factor));

    // Add the noise to the image
    cv::add(image, noise, image, cv::noArray(), image.depth());

    image_container->set_opencv_image_object(image);
}

void Augmentation::apply_lens_blur(Image* image_container, double radius)
{
    cv::Mat image = image_container->get_opencv_image_object();

    cv::Mat blurred_image;
    cv::GaussianBlur(image, blurred_image, cv::Size(), radius);

    image_container->set_opencv_image_object(blurred_image);
}

void Augmentation::apply_bilateral_blur(Image *image_container, int diameter, double sigma_color, double sigma_space)
{
    cv::Mat image = image_container->get_opencv_image_object();
    cv::Mat blurred_image;

    cv::bilateralFilter(image, blurred_image, diameter, sigma_color, sigma_space);

    image_container->set_opencv_image_object(blurred_image);
}

void Augmentation::flip_image(Image *image_container)
{
    cv::Mat flipped_image;
    cv::flip(image_container->get_opencv_image_object(), flipped_image, 0);
    image_container->set_opencv_image_object(flipped_image);
}

void Augmentation::cut_out_image(Image *image_container, double radius)
{
    /* There are 2 ways to cut out the image
     *  - select a rectangular region
     *  - cut a triangle shaped part of the image
     *
     *  Because of the implementation of the overlay function
     *  the image will be warped to it's desired position. Placing
     *  a transparent region or just by cutting out the image won't work.
     *  We need to black out the pixels we want to remove. And this will
     *  be done by image inpainting.
     *
     *  NOTE: For code clarity the if-else branches are not merged, this will not
     *  decrease the performance by much.
     */
    Random random;
    std::vector<cv::Point> inpainting_points;
    cv::Mat image = image_container->get_opencv_image_object();

    bool is_triangle_mode = random.generate_integer(0, 1) == 1 ? true : false;
    if (is_triangle_mode) {
        // triangle
        bool is_left_side = random.generate_integer(0, 1) == 1 ? true : false;
        bool is_top_side = random.generate_integer(0, 1) == 1 ? true : false;

        double from_side_percentage = random.generate_double(20, 50) / 100;
        double to_side_percentage = random.generate_double(0, 10) / 100;

        // starting point
        if (is_left_side) inpainting_points.push_back(cv::Point(0, image.rows * from_side_percentage));
        else inpainting_points.push_back(cv::Point(image.cols, image.rows * from_side_percentage));

        // corner
        if (is_left_side) {
            if (is_top_side) inpainting_points.push_back(cv::Point(0, 0));
            else inpainting_points.push_back(cv::Point(0, image.rows));
        } else {
            if (is_top_side) inpainting_points.push_back(cv::Point(image.cols, 0));
            else inpainting_points.push_back(cv::Point(image.cols, image.rows));
        }

        // destination point
        if (is_left_side) {
            if (is_top_side) inpainting_points.push_back(cv::Point(image.cols - image.cols * to_side_percentage, 0));
            else inpainting_points.push_back(cv::Point(image.cols - image.cols * to_side_percentage, image.rows));
        } else {
            if (is_top_side) inpainting_points.push_back(cv::Point(image.cols * to_side_percentage, 0));
            else inpainting_points.push_back(cv::Point(image.cols * to_side_percentage, image.rows));
        }
    } else {
        // rectangular
        for (int i = 0; i < 4; i++) {
            bool is_left_side = i % 2 == 0;
            bool is_top_side = i < 2;

            double x_pos = random.generate_double(
                is_left_side ? 0 : image.cols / 2,
                is_left_side ? image.cols / 2 : image.cols
            );

            double y_pos = random.generate_double(
                is_top_side ? 0 : image.rows / 2,
                is_top_side ? image.rows / 2 : image.rows
            );

            inpainting_points.push_back(cv::Point(x_pos, y_pos));
        }
    }

    cv::Mat mask = cv::Mat::zeros(image.size(), CV_8UC1);
    cv::fillConvexPoly(mask, inpainting_points, cv::Scalar(255));

    cv::Mat inpainted_image;
    cv::inpaint(image, mask, inpainted_image, radius, cv::INPAINT_TELEA);

    image_container->set_opencv_image_object(inpainted_image);
}

void Augmentation::adjust_brightness(Image *image_container, double brightness_constant)
{
    try {
        if (brightness_constant < 0.5 || brightness_constant > 1.5)
            throw InvalidValueException("Augmentation::adjust_brightness","between 0.5 and 1.5", std::to_string(brightness_constant));
    } catch (InvalidValueException& ex) {
        qDebug() << ex.what();
        return;
    }


    cv::Mat image = image_container->get_opencv_image_object();
    image *= brightness_constant;

    // Clamp pixel values to the valid range of 0 to 255
    cv::Mat clamped_image = cv::max(cv::min(image, 255.0f), 0.0f);

    image_container->set_opencv_image_object(clamped_image);
}

void Augmentation::shift_rgb(Image *image_container, double red_constant, double green_constant, double blue_constant)
{
    try {
        if (red_constant < 0.75 || red_constant > 1.25)
            throw InvalidValueException("Augmentation::shift_rgb","between 0.75 and 1.25", "red constant of " + std::to_string(red_constant));
        if (green_constant < 0.75 || green_constant > 1.25)
            throw InvalidValueException("Augmentation::shift_rgb","between 0.75 and 1.25", "green constant of " + std::to_string(green_constant));
        if (blue_constant < 0.75 || blue_constant > 1.25)
            throw InvalidValueException("Augmentation::shift_rgb","between 0.75 and 1.25", "blue constant of " + std::to_string(blue_constant));
    } catch (InvalidValueException& ex) {
        qDebug() << ex.what();
        return;
    }

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

