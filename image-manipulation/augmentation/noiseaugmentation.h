#ifndef NOISEAUGMENTATION_H
#define NOISEAUGMENTATION_H

#include "augmentation.h"

/**
 * @file noiseaugmentation.h
 *
 * @class NoiseAugmentation
 * @brief This class augments images with
 * Gaussian noise
 *
 * @author Domonkos Say
 */

class NoiseAugmentation : public Augmentation
{
private:
    double factor;
public:
    /**
     * @brief Creates a NoiseAugmentation instance.
     * Call the augment method to augment the image.
     *
     * @param image_container: Image object noise augmentation
     * will be done on
     * @param factor: the spread or amount of random noise generated
    */
    NoiseAugmentation(Image* image_container, const double& factor);

    /**
     * @brief Generates random Gaussian noise on the input image.
     * Uses the Image object getter and setter for accessing the
     * OpenCV Mat image object. To do it, it assumes that the open
     * method was already called on that Image object.
     *
     * Larger values of factor will produce noisier data with a broader
     * range of random values. Smaller values will produce less noisy data,
     * with random values concentrated closer to the mean.
     *
     * Suggested values: Between 30 and 45.
     */
    void augment() override;
};

#endif // NOISEAUGMENTATION_H
