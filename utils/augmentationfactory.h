#ifndef AUGMENTATIONFACTORY_H
#define AUGMENTATIONFACTORY_H

#include "../containers/inputdata.h"
#include "../image-manipulation/augmentation/augmentation.h"
#include "../image-manipulation/augmentation/noiseaugmentation.h"
#include "../image-manipulation/augmentation/lensbluraugmentation.h"
#include "../image-manipulation/augmentation/bilateralbluraugmentation.h"
#include "../image-manipulation/augmentation/imageflipaugmentation.h"
#include "../image-manipulation/augmentation/cutoutaugmentation.h"
#include "../image-manipulation/augmentation/brightnessadjustionaugmentation.h"
#include "../image-manipulation/augmentation/rgbshiftaugmentation.h"

class AugmentationFactory
{
    unsigned int operation_type;
    Image* image_container;
public:
    AugmentationFactory(const unsigned int& operation_type, Image* image_container);

    Augmentation* getAugmentation();
};

#endif // AUGMENTATIONFACTORY_H
