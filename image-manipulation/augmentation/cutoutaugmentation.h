#ifndef CUTOUTAUGMENTATION_H
#define CUTOUTAUGMENTATION_H

#include "augmentation.h"

/**
 * @file cutoutaugmentation.h
 *
 * @class CutOutAugmentation
 * @brief This class augments images by
 * cutting out and inpainting triangular
 * or rectangular regions.
 *
 * @author Domonkos Say
 */

class CutOutAugmentation : public Augmentation
{
private:
    double radius;
public:
    /**
     * @brief Creates a CutOutAugmentation instance.
     * Call the augment method to augment the image.
     *
     * @param image_container: Image object noise augmentation
     * will be done on
     * @param radius: determines the size of the neighborhood
     * around each masked pixel to be considered during the
     * inpainting process
    */
    CutOutAugmentation(Image* image_container, const double& radius);

    /** @brief There are 2 ways to cut out the image
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
     *
     *  Triangular version:
     *      - One point is selected at the left or right side between 20% and 50% of the image's height
     *      - The top or bottom corner point is selected at the given side
     *      - One point is selected in a close range (0-10%) of the opposite side's top or bottom
     *
     *  Rectangular version:
     *      - 4 random points selected in each quarter of the image
     *
     *  Uses the Image object getter and setter for accessing the
     *  OpenCV Mat image object. To do it, it assumes that the open
     *  method was already called on that Image object.
     *
     *  A smaller inpainting radius usually leads to incomplete or less smooth inpainting results,
     *  especially for larger cut out regions. A larger value however can result in more complete
     *  and smoother inpainting, as the algorithm can better infer the missing information based
     *  on a larger surrounding area.
     *
     *  Suggested values: Between 4 and 12.
     */
    void augment() override;
};

#endif // CUTOUTAUGMENTATION_H
