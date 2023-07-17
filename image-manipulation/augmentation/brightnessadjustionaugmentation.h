#ifndef BRIGHTNESSADJUSTIONAUGMENTATION_H
#define BRIGHTNESSADJUSTIONAUGMENTATION_H

#include "augmentation.h"

/**
 * @file brightnessadjustionaugmentation.h
 *
 * @class BrightnessAdjustionAugmentation
 * @brief This class augments images by
 * adjusting their brightness.
 *
 * @author Domonkos Say
 */


class BrightnessAdjustionAugmentation : public Augmentation
{
private:
    double brightness_constant;
public:
    /**
     * @brief Creates a BrightnessAdjustionAugmentation instance.
     * Call the augment method to augment the image.
     *
     * @param image_container: Image object noise augmentation
     * will be done on
     * @param brightness_constant: The constant changing the image's
     * brightness. A value between 0.5 and 1.5 (50% and 150%).
    */
    BrightnessAdjustionAugmentation(Image* image_container, const double& brightness_constant);

    /** @brief Adjust the brightness of an image by a constant
     * Brightness modification works by adding or substracting a constant
     * value in each invidual pixel. Here we adjust the brightness between
     * 50% and 150%.
     * This is implemented for 3 channel images (CV_8UC3 - RGB / BGR only)
     *
     * Uses the Image object getter and setter for accessing the
     * OpenCV Mat image object. To do it, it assumes that the open
     * method was already called on that Image object.
     *
     * @throws Utils::Exceptions::InvalidValueException if a value less than 0.5 or greater than 1.5 was provided
    */
    void augment() override;
};

#endif // BRIGHTNESSADJUSTIONAUGMENTATION_H
