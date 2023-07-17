#ifndef IMAGEFLIPAUGMENTATION_H
#define IMAGEFLIPAUGMENTATION_H

#include "augmentation.h"

/**
 * @file imageflipaugmentation.h
 *
 * @class ImageFlipAugmentation
 * @brief This class augments images by
 * flipping them.
 *
 * @author Domonkos Say
 */

class ImageFlipAugmentation : public Augmentation
{
public:
    /**
     * @brief Creates a ImageFlipAugmentation instance.
     * Call the augment method to augment the image.
     *
     * @param image_container: Image object noise augmentation
     * will be done on
    */
    ImageFlipAugmentation(Image* image_container);

    /**
     * @brief Flips the image horizontally and rotates it by
     * 180 degree (to upside down).
     * Uses the Image object getter and setter for accessing the
     * OpenCV Mat image object. To do it, it assumes that the open
     * method was already called on that Image object.
     */
    void augment() override;
};

#endif // IMAGEFLIPAUGMENTATION_H
