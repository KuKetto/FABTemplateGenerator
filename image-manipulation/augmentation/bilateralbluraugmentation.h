#ifndef BILATERALBLURAUGMENTATION_H
#define BILATERALBLURAUGMENTATION_H

#include "augmentation.h"

/**
 * @file BilateralBlurAugmentation.h
 *
 * @brief This class augments images with
 * Bilateral blur
 *
 * @author Domonkos Say
 */

class BilateralBlurAugmentation : public Augmentation
{
    int diameter;
    double sigma_color;
    double sigma_space;
public:
    /**
     * @brief Creates a BilateralBlurAugmentation instance.
     * Call the augment method to augment the image.
     *
     * @param image_container: Image object noise augmentation
     * will be done on
     * @param diameter: The diameter of the pixel neighborhood used during filtering.
     * @param sigma_color: The standard deviation of the color space.
     * @param sigma_space: The standard deviation of the spatial space.
    */
    BilateralBlurAugmentation(Image* image_container, const int& diameter, const double& sigma_color, const double& sigma_space);

    /**
     * @brief The bilateral filter can effectively smooth regions
     * with similar color while preserving sharp transitions between
     * regions with different colors.
     * Uses the Image object getter and setter for accessing the
     * OpenCV Mat image object. To do it, it assumes that the open
     * method was already called on that Image object.
     *
     * A larger value of diameter leads to more extensive smoothing
     * across larger areas.
     *
     * A higher value of sigma_color preserves more color variation,
     * while a lower value results in stronger color smoothing.
     *
     * A higher value of sigma_space preserves more local structures
     * and edges during filtering.
     *
     * Suggested values:
     *  - diameter: Between 15 and 50.
     *  - sigma_color: Between 80 and 220.
     *  - sigma_space: Between 35 and 70.
     */
    void augment() override;
};

#endif // BILATERALBLURAUGMENTATION_H
