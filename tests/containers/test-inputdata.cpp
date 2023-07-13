#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "../../containers/inputdata.h"
#include "../../containers/inputdata.cpp"
#include "../../containers/config.h"
#include "../../utils/random.h"
#include "../../utils/random.cpp"
#include "../../utils/exceptions/invalidvalueexception.h"

TEST_CASE("InputData class", "[InputData, Config, Random, InvalidValueException]") {
    SECTION("Constructor can be called without any error.") {
        InputData* input_data = nullptr;

        REQUIRE_NOTHROW(input_data = new InputData(Config(), 0, ""));

        delete input_data;
    }

    SECTION("Test setters and getters") {
        InputData* input_data = new InputData(Config(), 0, "");

        SECTION("Check if class id has been set correctly and return correct value"){
            REQUIRE(input_data->get_class_id() == 0);
        }

        delete input_data;
    }

    SECTION("Test use_this_image method") {
        SECTION("Check if an input data with empty config throws exception"){
            InputData* empty_input_data = new InputData(Config(), 0, "");

            REQUIRE_THROWS_AS(empty_input_data->use_this_image(), InvalidValueException);

            delete empty_input_data;
        }

        SECTION("Check if the augmentation count is less then one it returns OPERATION_TYPE::NORMAL") {
            Config config;
            config.set_image_count(10);
            InputData* input_data = new InputData(config, 0, "");

            REQUIRE(input_data->use_this_image() == OPERATION_TYPE::NORMAL);

            delete input_data;
        }

        SECTION("Check if with valid config it returns OPERATION_TYPE::CUT_OUT then throws exception when used up") {
            Config config;
            config.set_image_count(5);
            config.set_augment_count(5);
            config.set_augment_cut_out(5);
            InputData* input_data = new InputData(config, 0, "");

            for (int i = 0; i < 5; i++)
                REQUIRE(input_data->use_this_image() == OPERATION_TYPE::CUT_OUT);
            REQUIRE_THROWS_AS(input_data->use_this_image(), InvalidValueException);

            delete input_data;
        }
    }

    SECTION("Test image_used method") {
        SECTION("Test for noise augment") {
            Config config;
            config.set_image_count(1);
            config.set_augment_count(5);
            config.set_augment_noise(5);
            InputData* input_data = new InputData(config, 0, "");

            REQUIRE(input_data->use_this_image() == OPERATION_TYPE::NOISE);
            for (int i = 0; i < 4; i++)
                REQUIRE_NOTHROW(input_data->image_used(OPERATION_TYPE::NOISE) == false);
            REQUIRE_NOTHROW(input_data->image_used(OPERATION_TYPE::NOISE) == true);
            REQUIRE_THROWS_AS(input_data->use_this_image(), InvalidValueException);

            delete input_data;
        }

        SECTION("Test for lens blur augment") {
            Config config;
            config.set_image_count(1);
            config.set_augment_count(5);
            config.set_augment_lens_blur(5);
            InputData* input_data = new InputData(config, 0, "");

            REQUIRE(input_data->use_this_image() == OPERATION_TYPE::LENS_BLUR);
            for (int i = 0; i < 4; i++)
                REQUIRE_NOTHROW(input_data->image_used(OPERATION_TYPE::LENS_BLUR) == false);
            REQUIRE_NOTHROW(input_data->image_used(OPERATION_TYPE::LENS_BLUR) == true);
            REQUIRE_THROWS_AS(input_data->use_this_image(), InvalidValueException);

            delete input_data;
        }

        SECTION("Test for bilateral blur augment") {
            Config config;
            config.set_image_count(1);
            config.set_augment_count(5);
            config.set_augment_bilateral_blur(5);
            InputData* input_data = new InputData(config, 0, "");

            REQUIRE(input_data->use_this_image() == OPERATION_TYPE::BILATERAL_BLUR);
            for (int i = 0; i < 4; i++)
                REQUIRE_NOTHROW(input_data->image_used(OPERATION_TYPE::BILATERAL_BLUR) == false);
            REQUIRE_NOTHROW(input_data->image_used(OPERATION_TYPE::BILATERAL_BLUR) == true);
            REQUIRE_THROWS_AS(input_data->use_this_image(), InvalidValueException);

            delete input_data;
        }

        SECTION("Test for image flip augment") {
            Config config;
            config.set_image_count(1);
            config.set_augment_count(5);
            config.set_augment_image_flip(5);
            InputData* input_data = new InputData(config, 0, "");

            REQUIRE(input_data->use_this_image() == OPERATION_TYPE::IMAGE_FLIP);
            for (int i = 0; i < 4; i++)
                REQUIRE_NOTHROW(input_data->image_used(OPERATION_TYPE::IMAGE_FLIP) == false);
            REQUIRE_NOTHROW(input_data->image_used(OPERATION_TYPE::IMAGE_FLIP) == true);
            REQUIRE_THROWS_AS(input_data->use_this_image(), InvalidValueException);

            delete input_data;
        }

        SECTION("Test for cut out augment") {
            Config config;
            config.set_image_count(1);
            config.set_augment_count(5);
            config.set_augment_cut_out(5);
            InputData* input_data = new InputData(config, 0, "");

            REQUIRE(input_data->use_this_image() == OPERATION_TYPE::CUT_OUT);
            for (int i = 0; i < 4; i++)
                REQUIRE_NOTHROW(input_data->image_used(OPERATION_TYPE::CUT_OUT) == false);
            REQUIRE_NOTHROW(input_data->image_used(OPERATION_TYPE::CUT_OUT) == true);
            REQUIRE_THROWS_AS(input_data->use_this_image(), InvalidValueException);

            delete input_data;
        }

        SECTION("Test for brightness adjustion augment") {
            Config config;
            config.set_image_count(1);
            config.set_augment_count(5);
            config.set_augment_brightness(5);
            InputData* input_data = new InputData(config, 0, "");

            REQUIRE(input_data->use_this_image() == OPERATION_TYPE::BRIGHTNESS_ADJUSTION);
            for (int i = 0; i < 4; i++)
                REQUIRE_NOTHROW(input_data->image_used(OPERATION_TYPE::BRIGHTNESS_ADJUSTION) == false);
            REQUIRE_NOTHROW(input_data->image_used(OPERATION_TYPE::BRIGHTNESS_ADJUSTION) == true);
            REQUIRE_THROWS_AS(input_data->use_this_image(), InvalidValueException);

            delete input_data;
        }

        SECTION("Test for rgb shift augment") {
            Config config;
            config.set_image_count(1);
            config.set_augment_count(5);
            config.set_augment_rgb_shift(5);
            InputData* input_data = new InputData(config, 0, "");

            REQUIRE(input_data->use_this_image() == OPERATION_TYPE::RGB_SHIFT);
            for (int i = 0; i < 4; i++)
                REQUIRE_NOTHROW(input_data->image_used(OPERATION_TYPE::RGB_SHIFT) == false);
            REQUIRE_NOTHROW(input_data->image_used(OPERATION_TYPE::RGB_SHIFT) == true);
            REQUIRE_THROWS_AS(input_data->use_this_image(), InvalidValueException);

            delete input_data;
        }
    }
}
