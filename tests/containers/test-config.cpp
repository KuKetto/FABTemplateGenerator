#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "../../containers/config.h"
#include "../../containers/config.cpp"
#include "../../utils/exceptions/invalidvalueexception.h"
#include "../../utils/exceptions/invalidvalueexception.cpp"
#include "../../utils/configreader.h"
#include "../../utils/configreader.cpp"

TEST_CASE("Config class", "[Config, InvalidValueException]") {
    SECTION("Constructor can be called without any error.") {
        Config* config;

        REQUIRE_NOTHROW(config = new Config());

        delete config;
    }

    SECTION("Setters and Getters are working as expected") {
        Config* config;

        config = new Config();

        SECTION("Check if image count can be set without exception") {
            REQUIRE_NOTHROW(config->set_image_count(60));
        }

        SECTION("Check if normal image count can be set without exception") {
            REQUIRE_NOTHROW(config->set_normal_image_count(30));
        }

        SECTION("Check if augment count can be set without exception") {
            REQUIRE_NOTHROW(config->set_augment_count(30));
        }

        SECTION("Check if noise augment count can be set without exception") {
            REQUIRE_NOTHROW(config->set_augment_noise(4));
        }

        SECTION("Check if lens blur augment count can be set without exception") {
            REQUIRE_NOTHROW(config->set_augment_lens_blur(5));
        }

        SECTION("Check if bilateral blur augment count can be set without exception") {
            REQUIRE_NOTHROW(config->set_augment_bilateral_blur(3));
        }

        SECTION("Check if image flip augment count can be set without exception") {
            REQUIRE_NOTHROW(config->set_augment_image_flip(8));
        }

        SECTION("Check if cut out augment count can be set without exception") {
            REQUIRE_NOTHROW(config->set_augment_cut_out(9));
        }

        SECTION("Check if brightness adjustion augment count can be set without exception") {
            REQUIRE_NOTHROW(config->set_augment_brightness(1));
        }

        SECTION("Check if rgb shift augment count can be set without exception") {
            REQUIRE_NOTHROW(config->set_augment_rgb_shift(0));
        }

        config->set_image_count(60);
        config->set_normal_image_count(30);
        config->set_augment_count(30);
        config->set_augment_noise(4);
        config->set_augment_lens_blur(5);
        config->set_augment_bilateral_blur(3);
        config->set_augment_image_flip(8);
        config->set_augment_cut_out(9);
        config->set_augment_brightness(1);
        config->set_augment_rgb_shift(0);

        SECTION("Image count set successfuly, and getter returns correct value") {
            REQUIRE(config->get_image_count() == 60);
        }

        SECTION("Normal image count set successfuly, and getter returns correct value") {
            REQUIRE(config->get_normal_image_count() == 30);
        }

        SECTION("Augment count set successfuly, and getter returns correct value") {
            REQUIRE(config->get_augment_count() == 30);
        }

        SECTION("Noise augment count set successfuly, and getter returns correct value") {
            REQUIRE(config->get_augment_noise() == 4);
        }

        SECTION("Lens blur augment count set successfuly, and getter returns correct value") {
            REQUIRE(config->get_augment_lens_blur() == 5);
        }

        SECTION("Bilateral augment count set successfuly, and getter returns correct value") {
            REQUIRE(config->get_augment_bilateral_blur() == 3);
        }

        SECTION("Image flip augment count set successfuly, and getter returns correct value") {
            REQUIRE(config->get_augment_image_flip() == 8);
        }

        SECTION("Cut out augment count set successfuly, and getter returns correct value") {
            REQUIRE(config->get_augment_cut_out() == 9);
        }

        SECTION("Brightness adjustion augment count set successfuly, and getter returns correct value") {
            REQUIRE(config->get_augment_brightness() == 1);
        }

        SECTION("RGB shift augment count set successfuly, and getter returns correct value") {
            REQUIRE(config->get_augment_rgb_shift() == 0);
        }

        delete config;
    }

    SECTION("Config validation test with valid input") {
        Config* config;

        config = new Config();

        config->set_image_count(60);
        config->set_normal_image_count(30);
        config->set_augment_count(30);
        config->set_augment_noise(4);
        config->set_augment_lens_blur(5);
        config->set_augment_bilateral_blur(3);
        config->set_augment_image_flip(8);
        config->set_augment_cut_out(9);
        config->set_augment_brightness(1);
        config->set_augment_rgb_shift(0);

        REQUIRE_NOTHROW(config->validate());
        REQUIRE(config->validate() == true);

        delete config;
    }

    SECTION("Config validation test with invalid input. Checking image count.") {
        /* Checking for: image_count - normal_image_count != augment_count */
        Config* config;

        config = new Config();

        config->set_image_count(20);
        config->set_normal_image_count(5);
        config->set_augment_count(10);
        REQUIRE_THROWS_AS(config->validate(), InvalidValueException);

        config->set_image_count(7);
        config->set_normal_image_count(3);
        config->set_augment_count(10);
        REQUIRE_THROWS_AS(config->validate(), InvalidValueException);

        config->set_normal_image_count(8);
        config->set_augment_count(1);
        REQUIRE_THROWS_AS(config->validate(), InvalidValueException);

        config->set_normal_image_count(101);
        REQUIRE_THROWS_AS(config->validate(), InvalidValueException);

        delete config;
    }

    SECTION("Config validation test with invalid input. Checking augment count.") {
        /* Checking for:
            augment_noise + augment_lens_blur + augment_bilateral_blur
            + augment_image_flip + augment_cut_out + augment_brightness
            + augment_rgb_shift != augment_count
        */

        Config* config;

        config = new Config();

        config->set_image_count(100);
        config->set_normal_image_count(30);
        config->set_augment_count(70);
        config->set_augment_noise(10);
        config->set_augment_lens_blur(10);
        config->set_augment_bilateral_blur(10);
        config->set_augment_image_flip(10);
        config->set_augment_cut_out(10);
        config->set_augment_brightness(10);
        config->set_augment_rgb_shift(10);

        REQUIRE_NOTHROW(config->validate());
        REQUIRE(config->validate() == true);

        config->set_augment_noise(0);
        REQUIRE_THROWS_AS(config->validate(), InvalidValueException);

        config->set_augment_noise(10);
        config->set_augment_lens_blur(0);
        REQUIRE_THROWS_AS(config->validate(), InvalidValueException);

        config->set_augment_lens_blur(10);
        config->set_augment_bilateral_blur(0);
        REQUIRE_THROWS_AS(config->validate(), InvalidValueException);

        config->set_augment_bilateral_blur(10);
        config->set_augment_image_flip(0);
        REQUIRE_THROWS_AS(config->validate(), InvalidValueException);

        config->set_augment_image_flip(10);
        config->set_augment_cut_out(0);
        REQUIRE_THROWS_AS(config->validate(), InvalidValueException);

        config->set_augment_cut_out(10);
        config->set_augment_brightness(0);
        REQUIRE_THROWS_AS(config->validate(), InvalidValueException);

        config->set_augment_brightness(10);
        config->set_augment_rgb_shift(0);
        REQUIRE_THROWS_AS(config->validate(), InvalidValueException);

        delete config;
    }

    SECTION("Config validation test with the default config.") {

        ConfigReader config_reader("Invalid path");
        config_reader.read_config(false);

        Config config = config_reader.get_config();
        REQUIRE_NOTHROW(config.validate());
        REQUIRE(config.validate() == true);
    }
}
