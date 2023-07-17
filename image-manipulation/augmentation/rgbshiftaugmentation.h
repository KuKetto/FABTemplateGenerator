#ifndef RGBSHIFTAUGMENTATION_H
#define RGBSHIFTAUGMENTATION_H

#include "augmentation.h"

/**
 * @file RGBShiftAugmentation.h
 *
 * @brief This class augments images by
 * shifting their rgb values.
 *
 * @author Domonkos Say
 */

class RGBShiftAugmentation : public Augmentation
{
private:
    double red_constant;
    double green_constant;
    double blue_constant;
public:
    /**
     * @brief Creates a RGBShiftAugmentation instance.
     * Call the augment method to augment the image.
     *
     * @param image_container: Image object noise augmentation
     * will be done on
     * @param red_constant: the red constant value between 0.75 and 1.25 (75% and 125%)
     * @param green_constant: the green constant value between 0.75 and 1.25 (75% and 125%)
     * @param blue_constant: the blue constant value between 0.75 and 1.25 (75% and 125%)
    */
    RGBShiftAugmentation(Image* image_container, const double& red_constant, const double& green_constant, const double& blue_constant);

    /** @brief Shift RGB value of an image
     * RGB shift works by shifting the respective red-green-blue values
     * by a different constant. This shift is limited between 75% and 125%
     *
     * This is implemented for 3 channel image (CV_8UC3 - RGB)
     *
     * @throws Utils::Exceptions::InvalidValueException if a value less than 0.75 or greater than 1.25 was provided
    */
    void augment() override;
};

#endif // RGBSHIFTAUGMENTATION_H
