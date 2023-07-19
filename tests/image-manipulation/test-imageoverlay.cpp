#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "../../../image-manipulation/imageoverlay.h"
#include "../../../image-manipulation/imageoverlay.cpp"

#include "../../../containers/image.h"

#include "../../../utils/exceptions/badusageexception.h"
#include "../../../utils/exceptions/invalidvalueexception.h"
#include "../../../utils/exceptions/invalidtemplateexception.h"
#include "../../../utils/exceptions/invalidtemplateexception.cpp"

std::string get_example_template_path() {
    const std::string current_file = __FILE__;
    const std::filesystem::path current_path(current_file);
    const std::filesystem::path templates_dir = current_path.parent_path().parent_path().parent_path().parent_path() / "examples/templates";
    const std::filesystem::path example_image_path = templates_dir / "image2.zip";

    return example_image_path.string();
}

std::string get_tests_data_template_path() {
    const std::string current_file = __FILE__;
    const std::filesystem::path current_path(current_file);
    const std::filesystem::path template_path = current_path.parent_path().parent_path().parent_path() / "data/template_without_card_data.zip";

    return template_path.string();
}

std::string get_tests_data_overlayed_result_path() {
    const std::string current_file = __FILE__;
    const std::filesystem::path current_path(current_file);
    const std::filesystem::path template_path = current_path.parent_path().parent_path().parent_path() / "data/overlayed_result.png";

    return template_path.string();
}

std::vector<std::string> get_example_images_path() {
    std::vector<std::string> example_paths;

    const std::string current_file = __FILE__;
    const std::filesystem::path current_path(current_file);
    const std::filesystem::path inputs_dir = current_path.parent_path().parent_path().parent_path().parent_path() / "examples/inputs";

    example_paths.push_back((inputs_dir / "MON055.webp").string());
    example_paths.push_back((inputs_dir / "DYN019.webp").string());

    return example_paths;
}

TEST_CASE("Test ImageOverlay class", "[ImageOverlay, BadUsageException, InvalidValueException, InvalidTemplateException]") {
    SECTION("Constructor can be called without any error.") {
        ImageOverlay* image_overlay = nullptr;

        REQUIRE_NOTHROW(image_overlay = new ImageOverlay());

        delete image_overlay;
    }

    SECTION("Check setters if they work as intended") {
        SECTION("Check if template setter with valid input it returns corrent amount of required images") {
            ImageOverlay* image_overlay = new ImageOverlay();
            Image* template_image = new Image(true);
            template_image->set_file_path(get_example_template_path());
            template_image->open();

            REQUIRE(image_overlay->set_template_image(template_image) == 2);

            template_image->close();
            delete template_image;
            delete image_overlay;
        }

        SECTION("Check if template setter throws InvalidValueException for nullpointer input") {
            ImageOverlay* image_overlay = new ImageOverlay();
            Image* template_image = nullptr;

            REQUIRE_THROWS_AS(image_overlay->set_template_image(template_image), InvalidValueException);

            delete template_image;
            delete image_overlay;
        }

        SECTION("Check if template setter throws BadUsageException for not a template input") {
            ImageOverlay* image_overlay = new ImageOverlay();
            Image* template_image = new Image();
            template_image->set_file_path(get_example_images_path()[0]);
            template_image->open();

            REQUIRE_THROWS_AS(image_overlay->set_template_image(template_image), BadUsageException);

            template_image->close();
            delete template_image;
            delete image_overlay;
        }

        SECTION("Check if template setter throws InvalidTemplateException for a template that requires 0 image") {
            ImageOverlay* image_overlay = new ImageOverlay();
            Image* template_image = new Image(true);
            template_image->set_file_path(get_tests_data_template_path());
            template_image->open();

            REQUIRE_THROWS_AS(image_overlay->set_template_image(template_image), InvalidTemplateException);

            template_image->close();
            delete template_image;
            delete image_overlay;
        }

        SECTION("Check if input image setter throws BadUsageException when template image was not set beforehand") {
            ImageOverlay* image_overlay = new ImageOverlay();
            QVector<Image*> images{new Image()};

            REQUIRE_THROWS_AS(image_overlay->set_images_to_overlay(images), BadUsageException);

            for (auto image : images) delete image;
            delete image_overlay;
        }

        SECTION("Check if input image setter throws InvalidValueException when the vector size not matches the required image count") {
            ImageOverlay* image_overlay = new ImageOverlay();
            Image* template_image = new Image(true);
            template_image->set_file_path(get_example_template_path());
            template_image->open();
            QVector<Image*> images{new Image()};

            REQUIRE(image_overlay->set_template_image(template_image) == 2);
            REQUIRE_THROWS_AS(image_overlay->set_images_to_overlay(images), InvalidValueException);

            for (auto image : images) delete image;
            template_image->close();
            delete template_image;
            delete image_overlay;
        }

        SECTION("Check if with correct parameters input image setter does not fail") {
            ImageOverlay* image_overlay = new ImageOverlay();
            Image* template_image = new Image(true);
            template_image->set_file_path(get_example_template_path());
            template_image->open();
            QVector<Image*> images{new Image(), new Image()};

            REQUIRE(image_overlay->set_template_image(template_image) == 2);
            REQUIRE_NOTHROW(image_overlay->set_images_to_overlay(images));

            for (auto image : images) delete image;
            template_image->close();
            delete template_image;
            delete image_overlay;
        }
    }

    SECTION("Check if overlay method throws BadUsageException when inputs are missing") {
        ImageOverlay* image_overlay = new ImageOverlay();
        Image* template_image = new Image(true);
        template_image->set_file_path(get_example_template_path());
        QVector<Image*> images{new Image(), new Image()};

        SECTION("Check without inputs") {
            REQUIRE_THROWS_AS(image_overlay->overlay(), BadUsageException);
        }

        SECTION("Check with only template as input") {
            image_overlay->set_template_image(template_image);
            REQUIRE_THROWS_AS(image_overlay->overlay(), BadUsageException);
        }

        for (auto image : images) delete image;
        delete template_image;
        delete image_overlay;
    }

    SECTION("Check by pixel matching that overlay method overlays as expected and returns correct card data as well") {
        ImageOverlay* image_overlay = new ImageOverlay();

        Image* template_image = new Image(true);
        template_image->set_file_path(get_example_template_path());
        template_image->open();

        QVector<Image*> images{new Image(), new Image()};
        images[0]->set_file_path(get_example_images_path()[0]);
        images[1]->set_file_path(get_example_images_path()[1]);

        Image* control_image = new Image();
        control_image->set_file_path(get_tests_data_overlayed_result_path());
        control_image->open();
        cv::Mat control_bgra_image;
        cv::cvtColor(control_image->get_opencv_image_object(), control_bgra_image, cv::COLOR_RGB2BGRA);

        REQUIRE(image_overlay->set_template_image(template_image) == 2);
        REQUIRE_NOTHROW(image_overlay->set_images_to_overlay(images));
        ImageOverlay::OverlayedResult result = image_overlay->overlay();

        REQUIRE(result.image_width == control_image->get_opencv_image_object().cols);
        REQUIRE(result.image_height == control_image->get_opencv_image_object().rows);

        bool mismatch_found = false;
        double treshold = 1.0;
        for (int i = 0; i < result.image_height; i++) {
            for (int j = 0; j < result.image_width; j++) {
                cv::Vec4b control_image_pixels = control_bgra_image.at<cv::Vec4b>(i,j);
                cv::Vec4b overlayed_image_pixels = result.overlayed_image.at<cv::Vec4b>(i,j);
                for (int k = 0; k < 4; k++) {
                    if (std::abs(control_image_pixels[k] - overlayed_image_pixels[k]) > treshold)
                    {
                        mismatch_found = true;
                        break;
                    }
                }
                if (mismatch_found) break;
            }
            if (mismatch_found) break;
        }

        REQUIRE_FALSE(mismatch_found);
        REQUIRE(result.input_overlay_positions.size() == 2);
        REQUIRE(result.input_overlay_positions.at(0).first == get_example_images_path()[0]);
        REQUIRE(result.input_overlay_positions.at(1).first == get_example_images_path()[1]);
        REQUIRE(result.input_overlay_positions.at(0).second.first == cv::Point2f(1213, 865));
        REQUIRE(result.input_overlay_positions.at(0).second.second == cv::Point2f(1934, 1850));
        REQUIRE(result.input_overlay_positions.at(1).second.first == cv::Point2f(2192, 886));
        REQUIRE(result.input_overlay_positions.at(1).second.second == cv::Point2f(3340, 1878));

        control_image->close();
        delete control_image;
        for (auto image : images) delete image;
        template_image->close();
        delete template_image;
        delete image_overlay;
    }
}
