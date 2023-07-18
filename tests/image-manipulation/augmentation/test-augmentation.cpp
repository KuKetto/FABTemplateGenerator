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

#include "../../../utils/random.h"
#include "../../../utils/random.cpp"
#include "../../../utils/zlibcustomimageextractor.h"
#include "../../../utils/zlibcustomimageextractor.cpp"
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

TEST_CASE("NoiseAugmentation class", "[NoiseAugmentation]") {
    SECTION("Constructor can be called without any error.") {
        NoiseAugmentation* noise_augmentation = nullptr;
        Image* image = new Image();

        REQUIRE_NOTHROW(noise_augmentation = new NoiseAugmentation(image, 40));

        delete image;
        delete noise_augmentation;
    }

    SECTION("Check if noise is really applied to the image") {
        NoiseAugmentation* noise_augmentation = nullptr;

        Image* input_image = new Image();
        input_image->set_file_path(get_example_image_path());
        input_image->open();

        Image* noise_augmented_image = new Image();
        noise_augmented_image->set_file_path(get_example_image_path());
        noise_augmented_image->open();

        // Set the factor variable way to high and try to find a pixel which does not match by the noise
        REQUIRE_NOTHROW(noise_augmentation = new NoiseAugmentation(noise_augmented_image, 100));
        REQUIRE_NOTHROW(noise_augmentation->augment());

        // Hence the randomness of the method this can't really be tested in a different way
        bool mismatch_found = false;
        for (int i = 0; i < input_image->get_opencv_image_object().rows; i++)
            for (int j = 0; j < input_image->get_opencv_image_object().cols; j++) {
                if (input_image->get_opencv_image_object().at<uchar>(i,j)
                    != noise_augmented_image->get_opencv_image_object().at<uchar>(i,j))
                {
                    mismatch_found = true;
                    break;
                }
                if (mismatch_found) break;
            }

        REQUIRE(mismatch_found);

        noise_augmented_image->close();
        input_image->close();
        delete noise_augmented_image;
        delete input_image;
        delete noise_augmentation;
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
        LensBlurAugmentation* lens_blur_augmentation = nullptr;

        Image* control_image = new Image();
        control_image->set_file_path(get_tests_data_path() + "/lens_blur_radius_3.png");
        control_image->open();

        Image* lens_blur_augmented_image = new Image();
        lens_blur_augmented_image->set_file_path(get_example_image_path());
        lens_blur_augmented_image->open();

        // Set the radius 3 and match it to the control image
        REQUIRE_NOTHROW(lens_blur_augmentation = new LensBlurAugmentation(lens_blur_augmented_image, 3));
        REQUIRE_NOTHROW(lens_blur_augmentation->augment());

        bool mismatch_found = false;
        for (int i = 0; i < control_image->get_opencv_image_object().rows; i++)
            for (int j = 0; j < control_image->get_opencv_image_object().cols; j++) {
                if (control_image->get_opencv_image_object().at<uchar>(i,j)
                    != lens_blur_augmented_image->get_opencv_image_object().at<uchar>(i,j))
                {
                    mismatch_found = true;
                    break;
                }
                if (mismatch_found) break;
            }

        REQUIRE_FALSE(mismatch_found);

        lens_blur_augmented_image->close();
        control_image->close();
        delete lens_blur_augmented_image;
        delete control_image;
        delete lens_blur_augmentation;
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
        BilateralBlurAugmentation* bilateral_blur_augmentation = nullptr;

        Image* control_image = new Image();
        control_image->set_file_path(get_tests_data_path() + "/bilateral_blur_diameter_35_sigmacolor_150_sigmaspace_60.png");
        control_image->open();

        Image* bilateral_blur_augmented_image = new Image();
        bilateral_blur_augmented_image->set_file_path(get_example_image_path());
        bilateral_blur_augmented_image->open();

        // Set the diameter to 35, the sigma color to 150 and the sigma space to 60 then match it to the control image
        REQUIRE_NOTHROW(bilateral_blur_augmentation = new BilateralBlurAugmentation(bilateral_blur_augmented_image, 35, 150, 60));
        REQUIRE_NOTHROW(bilateral_blur_augmentation->augment());

        /**
         * @test TEMPORARY
         * TODO: Remove this temporary debugging section
         *
         * Debugging: Bilateral blur image data, the following section on github actions fails
        */
        const double treshold = 1.0;
        SECTION("Temporary debug section") {
            bool mismatch_found = false;
            for (int i = 0; i < control_image->get_opencv_image_object().rows; i++)
                for (int j = 0; j < control_image->get_opencv_image_object().cols; j++) {
                    uchar control_image_pixel = control_image->get_opencv_image_object().at<uchar>(i,j);
                    uchar bilateral_augmented_image_pixel = bilateral_blur_augmented_image->get_opencv_image_object().at<uchar>(i,j);
                    if (std::abs(control_image_pixel - bilateral_augmented_image_pixel) > 1)
                    {
                        qDebug() << "Mismatch found on pixel (" << i << ", " << j << ") as the value of the control image ("
                                 << control_image->get_opencv_image_object().at<uchar>(i,j) << ") does not match to the result ("
                                 << bilateral_blur_augmented_image->get_opencv_image_object().at<uchar>(i,j) << ")";
                        mismatch_found = true;
                        break;
                    }
                    if (mismatch_found) break;
                }

            REQUIRE_FALSE(mismatch_found);
        }

        bilateral_blur_augmented_image->close();
        control_image->close();
        delete bilateral_blur_augmented_image;
        delete control_image;
        delete bilateral_blur_augmentation;
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
        control_image->open();

        Image* image_flipped_augmented_image = new Image();
        image_flipped_augmented_image->set_file_path(get_example_image_path());
        image_flipped_augmented_image->open();

        REQUIRE_NOTHROW(image_flip_augmentation = new ImageFlipAugmentation(image_flipped_augmented_image));
        REQUIRE_NOTHROW(image_flip_augmentation->augment());

        bool mismatch_found = false;
        for (int i = 0; i < control_image->get_opencv_image_object().rows; i++)
            for (int j = 0; j < control_image->get_opencv_image_object().cols; j++) {
                if (control_image->get_opencv_image_object().at<uchar>(i,j)
                    != image_flipped_augmented_image->get_opencv_image_object().at<uchar>(i,j))
                {
                    mismatch_found = true;
                    break;
                }
                if (mismatch_found) break;
            }

        REQUIRE_FALSE(mismatch_found);

        image_flipped_augmented_image->close();
        control_image->close();
        delete image_flipped_augmented_image;
        delete control_image;
        delete image_flip_augmentation;
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
        CutOutAugmentation* cut_out_augmentation = nullptr;

        Image* input_image = new Image();
        input_image->set_file_path(get_example_image_path());
        input_image->open();

        Image* cut_out_augmented_image = new Image();
        cut_out_augmented_image->set_file_path(get_example_image_path());
        cut_out_augmented_image->open();

        REQUIRE_NOTHROW(cut_out_augmentation = new CutOutAugmentation(cut_out_augmented_image, 8));
        REQUIRE_NOTHROW(cut_out_augmentation->augment());

        // Hence the randomness of the method this can't really be tested in a different way
        bool mismatch_found = false;
        for (int i = 0; i < input_image->get_opencv_image_object().rows; i++)
            for (int j = 0; j < input_image->get_opencv_image_object().cols; j++) {
                if (input_image->get_opencv_image_object().at<uchar>(i,j)
                    != cut_out_augmented_image->get_opencv_image_object().at<uchar>(i,j))
                {
                    mismatch_found = true;
                    break;
                }
                if (mismatch_found) break;
            }

        REQUIRE(mismatch_found);

        cut_out_augmented_image->close();
        input_image->close();
        delete cut_out_augmented_image;
        delete input_image;
        delete cut_out_augmentation;
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
        BrightnessAdjustionAugmentation* brightness_adjustion_augmentation = nullptr;

        Image* control_image = new Image();
        control_image->set_file_path(get_tests_data_path() + "/brightness_adjustion_constant_1.3.png");
        control_image->open();

        Image* brightness_adjusted_augmented_image = new Image();
        brightness_adjusted_augmented_image->set_file_path(get_example_image_path());
        brightness_adjusted_augmented_image->open();

        // Set the brightness constant to 1.3 and match it to the control image
        REQUIRE_NOTHROW(brightness_adjustion_augmentation = new BrightnessAdjustionAugmentation(brightness_adjusted_augmented_image, 1.3));
        REQUIRE_NOTHROW(brightness_adjustion_augmentation->augment());

        bool mismatch_found = false;
        for (int i = 0; i < control_image->get_opencv_image_object().rows; i++)
            for (int j = 0; j < control_image->get_opencv_image_object().cols; j++) {
                if (control_image->get_opencv_image_object().at<uchar>(i,j)
                    != brightness_adjusted_augmented_image->get_opencv_image_object().at<uchar>(i,j))
                {
                    mismatch_found = true;
                    break;
                }
                if (mismatch_found) break;
            }

        REQUIRE_FALSE(mismatch_found);

        brightness_adjusted_augmented_image->close();
        control_image->close();
        delete brightness_adjusted_augmented_image;
        delete control_image;
        delete brightness_adjustion_augmentation;
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
        RGBShiftAugmentation* rgb_shift_augmentation = nullptr;

        Image* control_image = new Image();
        control_image->set_file_path(get_tests_data_path() + "/rgb_shifted_red_1.05_green_1.1_blue_0.92.png");
        control_image->open();

        Image* rgb_shift_augmented_image = new Image();
        rgb_shift_augmented_image->set_file_path(get_example_image_path());
        rgb_shift_augmented_image->open();

        // Set the red constant to 1.05, the green constant to 1.1 and the blue constant to 0.92 then match it to the control image
        REQUIRE_NOTHROW(rgb_shift_augmentation = new RGBShiftAugmentation(rgb_shift_augmented_image, 1.05, 1.1, 0.92));
        REQUIRE_NOTHROW(rgb_shift_augmentation->augment());

        bool mismatch_found = false;
        for (int i = 0; i < control_image->get_opencv_image_object().rows; i++)
            for (int j = 0; j < control_image->get_opencv_image_object().cols; j++) {
                if (control_image->get_opencv_image_object().at<uchar>(i,j)
                    != rgb_shift_augmented_image->get_opencv_image_object().at<uchar>(i,j))
                {
                    mismatch_found = true;
                    break;
                }
                if (mismatch_found) break;
            }

        REQUIRE_FALSE(mismatch_found);

        rgb_shift_augmented_image->close();
        control_image->close();
        delete rgb_shift_augmented_image;
        delete control_image;
        delete rgb_shift_augmentation;
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
