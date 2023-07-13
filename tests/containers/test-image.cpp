#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "../../containers/image.h"
#include "../../containers/image.cpp"
#include "../../utils/exceptions/filenotfound.h"
#include "../../utils/exceptions/filenotfound.cpp"
#include "../../utils/exceptions/libraryexception.h"
#include "../../utils/exceptions/libraryexception.cpp"
#include "../../utils/zlibcustomimageextractor.h"
#include "../../utils/zlibcustomimageextractor.cpp"
#include "../../utils/exceptions/badusageexception.h"
#include "../../utils/exceptions/badusageexception.cpp"

TEST_CASE("Image class", "[Image, FileNotFound, LibraryException, BadUsageException]") {
    std::string current_file = __FILE__;
    std::filesystem::path current_path(current_file);
    std::filesystem::path examples_dir = current_path.parent_path().parent_path().parent_path() / "examples";
    std::filesystem::path image_path = examples_dir / "inputs" / "DYN019.webp";
    std::filesystem::path template_path = examples_dir / "templates" / "image4.zip";

    SECTION("Constructor can be called without any error.") {
        Image* image;

        REQUIRE_NOTHROW(image = new Image());
        delete image;

        REQUIRE_NOTHROW(image = new Image(true));
        delete image;
    }

    SECTION("Test setters and getters") {
        Image* image = new Image(true);

        SECTION("Check if file path can be set without exception") {
            REQUIRE_NOTHROW(image->set_file_path(template_path.string()));
        }
        image->set_file_path(template_path.string());
        SECTION("Check if file path has been set correctly and return correct value") {
            REQUIRE(image->get_file_path() == template_path.string());
        }

        cv::Mat kernel = (cv::Mat_<int>(2, 2) << 1,2,3,4);
        SECTION("Check if opencv mat image object can be set without exception") {
            REQUIRE_NOTHROW(image->set_opencv_image_object(kernel));
        }
        image->set_opencv_image_object(kernel);
        SECTION("Check if opencv mat image object has been set correctly and return correct value") {
            REQUIRE(image->get_opencv_image_object().at<int>(0, 0) == 1);
            REQUIRE(image->get_opencv_image_object().at<int>(0, 1) == 2);
            REQUIRE(image->get_opencv_image_object().at<int>(1, 0) == 3);
            REQUIRE(image->get_opencv_image_object().at<int>(1, 1) == 4);
        }


        SECTION("Check if loaded template's card position has been initalized right and getters return correct values") {
            image->open();
            REQUIRE(image->get_card_positions_size() == 4);
            REQUIRE(image->get_card_positions_perspective(2)[0].x == 2525);
            REQUIRE(image->get_card_positions_perspective(2)[0].y == 1170);
            image->close();
        }

        delete image;
    }

    SECTION("Test open method") {
        SECTION("Test open method for valid image") {
            Image* valid_image = new Image();
            valid_image->set_file_path(image_path.string());

            REQUIRE_NOTHROW(valid_image->open());
            REQUIRE(valid_image->is_open() == true);
            REQUIRE(valid_image->get_opencv_image_object().rows == 762);
            REQUIRE(valid_image->get_opencv_image_object().cols == 546);

            valid_image->close();
            delete valid_image;
        }

        SECTION("Test open method for valid template") {
            Image* valid_template = new Image(true);
            valid_template->set_file_path(template_path.string());

            REQUIRE_NOTHROW(valid_template->open());
            REQUIRE(valid_template->is_open() == true);
            REQUIRE(valid_template->get_opencv_image_object().rows == 2322);
            REQUIRE(valid_template->get_opencv_image_object().cols == 4128);
            REQUIRE(valid_template->get_card_positions_size() == 4);
            REQUIRE(valid_template->get_card_positions_perspective(2)[0].x == 2525);
            REQUIRE(valid_template->get_card_positions_perspective(2)[0].y == 1170);

            valid_template->close();
            delete valid_template;
        }

        SECTION("Test open method for empty file path") {
            Image* empty_file_path_image = new Image();

            REQUIRE_THROWS_AS(empty_file_path_image->open(), FileNotFound);
            REQUIRE(empty_file_path_image->is_open() == false);

            delete empty_file_path_image;

        }

        SECTION("Test open method for OpenCV library error via invalid file path") {
            Image* invalid_path_image = new Image();
            invalid_path_image->set_file_path("invalid path");

            REQUIRE_THROWS_AS(invalid_path_image->open(), LibraryException);
            REQUIRE(invalid_path_image->is_open() == false);

            delete invalid_path_image;
        }

        SECTION("Test open method for bad usage (in case of an image zip file is provided instead of an image)") {
            Image* bad_usage = new Image();

            bad_usage->set_file_path(template_path.string());
            REQUIRE_THROWS_AS(bad_usage->open(), BadUsageException);

            delete bad_usage;
        }
    }

    SECTION("Test if close method deallocates the data allocated by open method") {
        Image* valid_template = new Image(true);
        valid_template->set_file_path(template_path.string());
        valid_template->open();

        valid_template->close();
        REQUIRE(valid_template->is_open() == false);
        REQUIRE(valid_template->get_opencv_image_object().cols == 0);
        REQUIRE(valid_template->get_opencv_image_object().rows == 0);
        REQUIRE(valid_template->get_card_positions_size() == 0);

        delete valid_template;
    }
}
