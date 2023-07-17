#include "augmentationfactory.h"

AugmentationFactory::AugmentationFactory(const unsigned int &operation_type, Image *image_container):
    operation_type(operation_type), image_container(image_container)
{

}

Augmentation *AugmentationFactory::getAugmentation()
{
    switch (operation_type) {
    case OPERATION_TYPE::NOISE:
        return new NoiseAugmentation(image_container, Random::generate_double(30, 45));
    case OPERATION_TYPE::LENS_BLUR:
        return new LensBlurAugmentation(image_container, Random::generate_double(1, 3));
    case OPERATION_TYPE::BILATERAL_BLUR:
        return new BilateralBlurAugmentation(image_container, Random::generate_integer(15, 50), Random::generate_integer(80, 220), Random::generate_integer(35, 70));
    case OPERATION_TYPE::IMAGE_FLIP:
        return new ImageFlipAugmentation(image_container);
    case OPERATION_TYPE::CUT_OUT:
        return new CutOutAugmentation(image_container, Random::generate_integer(4, 12));
    case OPERATION_TYPE::BRIGHTNESS_ADJUSTION:
        return new BrightnessAdjustionAugmentation(image_container, Random::generate_double(0.75, 1.25));
    case OPERATION_TYPE::RGB_SHIFT:
        return new RGBShiftAugmentation(image_container, Random::generate_double(0.9, 1.1), Random::generate_double(0.9, 1.1), Random::generate_double(0.9, 1.1));
    default:
        return nullptr;
    }
}
