#ifndef LENSBLURAUGMENTATION_H
#define LENSBLURAUGMENTATION_H

#include "augmentation.h"

/**
 * @file LensBlurAugmentation.h
 *
 * @brief This class augments images with
 * Gaussian (lens) blur
 *
 * @author Domonkos Say
 */

class LensBlurAugmentation : public Augmentation
{
private:
    double radius;
public:
    /**
     * @brief Creates a LensBlurAugmentation instance.
     * Call the augment method to augment the image.
     *
     * @param image_container: Image object noise augmentation
     * will be done on
     * @param radius: The size of Gaussian kernel
    */
    LensBlurAugmentation(Image* image_container, const double& radius);

    /**
     * @brief The Gaussian blur is achieved by convolving the
     * input image with a Gaussian kernel.
     * Uses the Image object getter and setter for accessing the
     * OpenCV Mat image object. To do it, it assumes that the open
     * method was already called on that Image object.
     *
     * The size of Gaussian kernel is determined by the radius parameter.
     * With smaller value of radius the output image will retain more
     * details and sharpness. With larger value of radius the output image
     * will become smoother, and fine details might get blurred out.
     *
     * Suggested values: Between 1 and 3.
     */
    void augment() override;
};

#endif // LENSBLURAUGMENTATION_H
