#include "noiseaugmentation.h"

NoiseAugmentation::NoiseAugmentation(Image *image_container, const double &factor):
    factor(factor), Augmentation(image_container)
{

}

void NoiseAugmentation::augment()
{
    // Set the mean for the Gaussian noise
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
