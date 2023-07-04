#include "augmentation.h"

void Augmentation::apply_noise(Image* image_container, double factor)
{
    // Set the mean and standard deviation for the Gaussian noise
    double mean = 0.0;
    //double factor = 30.0;

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
    //double radius = 2.0;

    cv::Mat blurred_image;
    cv::GaussianBlur(image, blurred_image, cv::Size(), radius);

    image_container->set_opencv_image_object(blurred_image);
}

void Augmentation::apply_bilateral_blur(Image *image_container, int diameter, double sigma_color, double sigma_space)
{
    cv::Mat image = image_container->get_opencv_image_object();
    cv::cvtColor(image, image, cv::COLOR_BGRA2BGR);
    cv::Mat blurred_image;

    cv::bilateralFilter(image, blurred_image, diameter, sigma_color, sigma_space);

    image_container->set_opencv_image_object(blurred_image);
}

