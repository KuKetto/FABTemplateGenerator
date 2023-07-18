#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "../../../image-manipulation/augmentation/augmentation.h"
#include "../../../image-manipulation/augmentation/noiseaugmentation.h"
#include "../../../image-manipulation/augmentation/lensbluraugmentation.h"
#include "../../../image-manipulation/augmentation/bilateralbluraugmentation.h"
#include "../../../image-manipulation/augmentation/imageflipaugmentation.h"
#include "../../../image-manipulation/augmentation/cutoutaugmentation.h"
#include "../../../image-manipulation/augmentation/brightnessadjustionaugmentation.h"
#include "../../../image-manipulation/augmentation/rgbshiftaugmentation.h"

#include "../../../image-manipulation/augmentation/noiseaugmentation.cpp"
#include "../../../image-manipulation/augmentation/lensbluraugmentation.cpp"
#include "../../../image-manipulation/augmentation/bilateralbluraugmentation.cpp"
#include "../../../image-manipulation/augmentation/imageflipaugmentation.cpp"
#include "../../../image-manipulation/augmentation/cutoutaugmentation.cpp"
#include "../../../image-manipulation/augmentation/brightnessadjustionaugmentation.cpp"
#include "../../../image-manipulation/augmentation/rgbshiftaugmentation.cpp"

#include "../../../containers/image.h"
#include "../../../containers/image.cpp"
#include "../../../containers/cardpositiondata.h"
#include "../../../containers/cardpositiondata.cpp"
#include "../../../containers/inputdata.h"
#include "../../../containers/inputdata.cpp"
#include "../../../containers/config.h"
#include "../../../containers/config.cpp"

#include "../../../utils/random.h"
#include "../../../utils/random.cpp"
#include "../../../utils/zlibcustomimageextractor.h"
#include "../../../utils/zlibcustomimageextractor.cpp"
#include "../../../utils/augmentationfactory.h"
#include "../../../utils/augmentationfactory.cpp"
#include "../../../utils/exceptions/invalidvalueexception.h"
#include "../../../utils/exceptions/invalidvalueexception.cpp"
#include "../../../utils/exceptions/pathnotfoundexception.h"
#include "../../../utils/exceptions/pathnotfoundexception.cpp"
#include "../../../utils/exceptions/badusageexception.h"
#include "../../../utils/exceptions/badusageexception.cpp"
#include "../../../utils/exceptions/libraryexception.h"
#include "../../../utils/exceptions/libraryexception.cpp"

std::string get_example_image_path() {
    const std::string current_file = __FILE__;
    const std::filesystem::path current_path(current_file);
    const std::filesystem::path inputs_dir = current_path.parent_path().parent_path().parent_path().parent_path() / "examples/inputs";
    const std::filesystem::path example_image_path = inputs_dir / "MON055.webp";

    return example_image_path.string();
}

std::string get_tests_data_path() {
    const std::string current_file = __FILE__;
    const std::filesystem::path current_path(current_file);
    const std::filesystem::path data_dir = current_path.parent_path().parent_path().parent_path() / "data";

    return data_dir.string();
}

Augmentation* getAugmentationUsingCustomData(const unsigned int& operation_type, Image* image_container,
                                            const double& input = 0.0, const double& input_2 = 0.0,
                                            const double& input_3 = 0.0)
{
    switch (operation_type) {
    case OPERATION_TYPE::NOISE:
        return new NoiseAugmentation(image_container, input);
    case OPERATION_TYPE::LENS_BLUR:
        return new LensBlurAugmentation(image_container, input);
    case OPERATION_TYPE::BILATERAL_BLUR:
        return new BilateralBlurAugmentation(image_container, input, input_2, input_3);
    case OPERATION_TYPE::IMAGE_FLIP:
        return new ImageFlipAugmentation(image_container);
    case OPERATION_TYPE::CUT_OUT:
        return new CutOutAugmentation(image_container, input);
    case OPERATION_TYPE::BRIGHTNESS_ADJUSTION:
        return new BrightnessAdjustionAugmentation(image_container, input);
    case OPERATION_TYPE::RGB_SHIFT:
        return new RGBShiftAugmentation(image_container, input, input_2, input_3);
    default:
        return nullptr;
    }
}

bool check_random_based_augmentation(const unsigned int& operation_type) {
    Image* augmented_image = new Image();
    augmented_image->set_file_path(get_example_image_path());
    augmented_image->open();

    Image* input_image = new Image();
    input_image->set_file_path(get_example_image_path());
    input_image->open();

    Augmentation* augmentation;
    AugmentationFactory* augmentation_factory = new AugmentationFactory(operation_type, augmented_image);
    augmentation = augmentation_factory->getAugmentation();

    REQUIRE_NOTHROW(augmentation->augment());

    // Hence the randomness of the method this can't really be tested in a different way
    bool mismatch_found = false;
    for (int i = 0; i < input_image->get_opencv_image_object().rows; i++)
        for (int j = 0; j < input_image->get_opencv_image_object().cols; j++) {
            if (input_image->get_opencv_image_object().at<uchar>(i,j)
                != augmented_image->get_opencv_image_object().at<uchar>(i,j))
            {
                mismatch_found = true;
                break;
            }
            if (mismatch_found) break;
        }

    augmented_image->close();
    input_image->close();
    delete augmented_image;
    delete input_image;
    delete augmentation;
    delete augmentation_factory;

    return mismatch_found;
}

bool check_pixel_matching_based_augmentation(const unsigned int& operation_type, const std::string& path,
                                             const double& input = 0.0, const double& input_2 = 0.0,
                                             const double& input_3 = 0.0)
{
    Image* augmented_image = new Image();
    augmented_image->set_file_path(get_example_image_path());
    augmented_image->open();

    Image* control_image = new Image();
    control_image->set_file_path(path);
    control_image->open();

    Augmentation* augmentation = getAugmentationUsingCustomData(operation_type, augmented_image, input, input_2, input_3);

    REQUIRE_NOTHROW(augmentation->augment());

    const double treshold = 1.0;
    bool mismatch_found = false;
    for (int i = 0; i < control_image->get_opencv_image_object().rows; i++)
        for (int j = 0; j < control_image->get_opencv_image_object().cols; j++) {
            uchar control_image_pixel = control_image->get_opencv_image_object().at<uchar>(i,j);
            uchar augmented_image_pixel = augmented_image->get_opencv_image_object().at<uchar>(i,j);
            if (std::abs(control_image_pixel - augmented_image_pixel) > treshold)
            {
                mismatch_found = true;
                break;
            }
            if (mismatch_found) break;
        }

    augmented_image->close();
    control_image->close();
    delete augmented_image;
    delete control_image;
    delete augmentation;

    return mismatch_found;
}

TEST_CASE("NoiseAugmentation class", "[NoiseAugmentation]") {
    SECTION("Constructor can be called without any error.") {
        NoiseAugmentation* noise_augmentation = nullptr;
        Image* image = new Image();

        REQUIRE_NOTHROW(noise_augmentation = new NoiseAugmentation(image, 40));

        delete image;
        delete noise_augmentation;
    }

    SECTION("Check if noise is really applied to the image") {
        REQUIRE(check_random_based_augmentation(OPERATION_TYPE::NOISE));
    }
}

TEST_CASE("LensBlurAugmentation class", "[LensBlurAugmentation]") {
    SECTION("Constructor can be called without any error.") {
        LensBlurAugmentation* lens_blur_augmentation = nullptr;
        Image* image = new Image();

        REQUIRE_NOTHROW(lens_blur_augmentation = new LensBlurAugmentation(image, 3));

        delete image;
        delete lens_blur_augmentation;
    }

    SECTION("Check if lens blur works as intended") {
        REQUIRE_FALSE(check_pixel_matching_based_augmentation(OPERATION_TYPE::LENS_BLUR,
                                                              get_tests_data_path() + "/lens_blur_radius_3.png",
                                                              3));
    }
}

TEST_CASE("BilateralBlurAugmentation class", "[BilateralBlurAugmentation]") {
    SECTION("Constructor can be called without any error.") {
        BilateralBlurAugmentation* bilateral_blur_augmentation = nullptr;
        Image* image = new Image();

        REQUIRE_NOTHROW(bilateral_blur_augmentation = new BilateralBlurAugmentation(image, 35, 150, 60));

        delete image;
        delete bilateral_blur_augmentation;
    }

    SECTION("Check if bilateral blur works as intended") {
        REQUIRE_FALSE(check_pixel_matching_based_augmentation(OPERATION_TYPE::BILATERAL_BLUR,
                                                              get_tests_data_path() + "/bilateral_blur_diameter_35_sigmacolor_150_sigmaspace_60.png",
                                                              35, 150, 60));
    }
}

TEST_CASE("ImageFlipAugmentation class", "[ImageFlipAugmentation]") {
    SECTION("Constructor can be called without any error.") {
        ImageFlipAugmentation* image_flip_augmentation = nullptr;
        Image* image = new Image();

        REQUIRE_NOTHROW(image_flip_augmentation = new ImageFlipAugmentation(image));

        delete image;
        delete image_flip_augmentation;
    }

    SECTION("Check if image flip works as intended") {
        ImageFlipAugmentation* image_flip_augmentation = nullptr;

        Image* control_image = new Image();
        control_image->set_file_path(get_tests_data_path() + "/flipped_image.png");
        REQUIRE_FALSE(check_pixel_matching_based_augmentation(OPERATION_TYPE::IMAGE_FLIP,
                                                              get_tests_data_path() + "/flipped_image.png"));

    }
}

TEST_CASE("CutOutAugmentation class", "[CutOutAugmentation]") {
    SECTION("Constructor can be called without any error.") {
        CutOutAugmentation* cut_out_augmentation = nullptr;
        Image* image = new Image();

        REQUIRE_NOTHROW(cut_out_augmentation = new CutOutAugmentation(image, 40));

        delete image;
        delete cut_out_augmentation;
    }

    SECTION("Check if there is a cut-out inpainted region") {
        REQUIRE(check_random_based_augmentation(OPERATION_TYPE::CUT_OUT));
    }
}

TEST_CASE("BrightnessAdjustionAugmentation class", "[BrightnessAdjustionAugmentation, InvalidValueException]") {
    SECTION("Constructor can be called without any error.") {
        BrightnessAdjustionAugmentation* brightness_adjustion_augmentation = nullptr;
        Image* image = new Image();

        REQUIRE_NOTHROW(brightness_adjustion_augmentation = new BrightnessAdjustionAugmentation(image, 1.3));

        delete image;
        delete brightness_adjustion_augmentation;
    }

    SECTION("Check if brightness adjustion works as intended") {
        REQUIRE_FALSE(check_pixel_matching_based_augmentation(OPERATION_TYPE::BRIGHTNESS_ADJUSTION,
                                                              get_tests_data_path() + "/brightness_adjustion_constant_1.3.png",
                                                              1.3));
    }

    SECTION("Check if throws InvalidValueException in case of invalid value") {
        SECTION("Too low value") {
            BrightnessAdjustionAugmentation* too_low_value = nullptr;
            Image* image = new Image();

            REQUIRE_NOTHROW(too_low_value = new BrightnessAdjustionAugmentation(image, 0.45));
            REQUIRE_THROWS_AS(too_low_value->augment(), InvalidValueException);

            delete image;
            delete too_low_value;
        }

        SECTION("Too high value") {
            BrightnessAdjustionAugmentation* too_high_value = nullptr;
            Image* image = new Image();

            REQUIRE_NOTHROW(too_high_value = new BrightnessAdjustionAugmentation(image, 1.6));
            REQUIRE_THROWS_AS(too_high_value->augment(), InvalidValueException);

            delete image;
            delete too_high_value;
        }
    }
}

TEST_CASE("RGBShiftAugmentation class", "[RGBShiftAugmentation, InvalidValueException]") {
    SECTION("Constructor can be called without any error.") {
        RGBShiftAugmentation* rgb_shift_augmentation = nullptr;
        Image* image = new Image();

        REQUIRE_NOTHROW(rgb_shift_augmentation = new RGBShiftAugmentation(image, 1.05, 1.1, 0.92));

        delete image;
        delete rgb_shift_augmentation;
    }

    SECTION("Check if brightness adjustion works as intended") {
        REQUIRE_FALSE(check_pixel_matching_based_augmentation(OPERATION_TYPE::RGB_SHIFT,
                                                              get_tests_data_path() + "/rgb_shifted_red_1.05_green_1.1_blue_0.92.png",
                                                              1.05, 1.1, 0.92));
    }

    SECTION("Check if throws InvalidValueException in case of invalid value") {
        SECTION("Too low values") {
            RGBShiftAugmentation* too_low_red_value = nullptr;
            RGBShiftAugmentation* too_low_green_value = nullptr;
            RGBShiftAugmentation* too_low_blue_value = nullptr;
            Image* image = new Image();

            REQUIRE_NOTHROW(too_low_red_value = new RGBShiftAugmentation(image, 0.74, 1, 1));
            REQUIRE_NOTHROW(too_low_green_value = new RGBShiftAugmentation(image, 1, 0.73, 1));
            REQUIRE_NOTHROW(too_low_blue_value = new RGBShiftAugmentation(image, 1, 1, 0.72));
            REQUIRE_THROWS_AS(too_low_red_value->augment(), InvalidValueException);
            REQUIRE_THROWS_AS(too_low_green_value->augment(), InvalidValueException);
            REQUIRE_THROWS_AS(too_low_blue_value->augment(), InvalidValueException);

            delete image;
            delete too_low_red_value;
            delete too_low_green_value;
            delete too_low_blue_value;
        }

        SECTION("Too high values") {
            RGBShiftAugmentation* too_high_red_value = nullptr;
            RGBShiftAugmentation* too_high_green_value = nullptr;
            RGBShiftAugmentation* too_high_blue_value = nullptr;
            Image* image = new Image();

            REQUIRE_NOTHROW(too_high_red_value = new RGBShiftAugmentation(image, 1.26, 1, 1));
            REQUIRE_NOTHROW(too_high_green_value = new RGBShiftAugmentation(image, 1, 1.27, 1));
            REQUIRE_NOTHROW(too_high_blue_value = new RGBShiftAugmentation(image, 1, 1, 1.28));
            REQUIRE_THROWS_AS(too_high_red_value->augment(), InvalidValueException);
            REQUIRE_THROWS_AS(too_high_green_value->augment(), InvalidValueException);
            REQUIRE_THROWS_AS(too_high_blue_value->augment(), InvalidValueException);

            delete image;
            delete too_high_red_value;
            delete too_high_green_value;
            delete too_high_blue_value;
        }
    }
}
