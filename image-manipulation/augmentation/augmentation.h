#ifndef AUGMENTATION_H
#define AUGMENTATION_H

#include <opencv2/opencv.hpp>
#include "../../containers/image.h"
#include "../../utils/random.h"
#include "../../utils/exceptions/invalidvalueexception.h"

/**
 * @file augmentation.h
 *
 * @class Augmentation
 * @brief This is an abstract class for image
 * augmentations.
 * For more about advanced image augmentation
 * look up https://www.kaggle.com/code/ankursingh12/data-augmentation-for-object-detection/notebook
 *
 * @author Domonkos Say
 *
 */

class Augmentation
{
protected:
    Image* image_container;
public:
    Augmentation(Image* image_container): image_container(image_container) {};
    virtual ~Augmentation() = 0;

    virtual void augment() = 0;
};

inline Augmentation::~Augmentation() {}

#endif // AUGMENTATION_H
