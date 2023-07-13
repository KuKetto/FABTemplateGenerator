#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "../../containers/inputdatacontainer.h"
#include "../../containers/inputdatacontainer.cpp"
#include "../../containers/image.h"
#include "../../containers/config.h"
#include "../../containers/inputdata.h"
#include "../../utils/random.h"
#include "../../utils/exceptions/pathnotfoundexception.h"
#include "../../utils/exceptions/libraryexception.h"
#include "../../utils/exceptions/badusageexception.h"
#include "../../utils/exceptions/invalidvalueexception.h"
#include "../../utils/exceptions/dividebyzero.h"
#include "../../utils/exceptions/dividebyzero.cpp"
#include "../../utils/exceptions/novalidinputsleftexception.h"
#include "../../utils/exceptions/novalidinputsleftexception.cpp"

TEST_CASE("InputDataContainer class", "[InputDataContainer, Config, PathNotFoundException, NoValidInputsLeftException, BadUsageException]") {
    std::string current_file = __FILE__;
    std::filesystem::path current_path(current_file);
    std::filesystem::path inputs_dir = current_path.parent_path().parent_path().parent_path() / "examples/inputs";

    SECTION("Check constructor.") {
        SECTION("Constructor can be called without any error.") {
            InputDataContainer* input_data_container = nullptr;

            REQUIRE_NOTHROW(input_data_container = new InputDataContainer(".", Config()));

            delete input_data_container;
        }

        SECTION("Constructor throws exception with invalid path.") {
            InputDataContainer* invalid_path_input_data_container = nullptr;

            REQUIRE_THROWS_AS(invalid_path_input_data_container = new InputDataContainer("invalid path", Config()), PathNotFoundException);

            delete invalid_path_input_data_container;
        }
    }

    SECTION("Check getters") {
        InputDataContainer* input_data_container = new InputDataContainer(inputs_dir.string(), Config());

        SECTION("Check if classes have been initalized right and the getter returns the correct values") {
            QList<std::string> classes = {"OUT039", "MON164", "MON055", "OUT073", "WTR014", "DYN019", "MON141",
                                          "OUT198", "OUT228", "U-MON217"};

            bool lists_matched = true;
            for (auto& _class : input_data_container->get_classes().values()) {
                if (!classes.contains(_class)) {
                    lists_matched = false;
                    break;
                }
            }

            REQUIRE(lists_matched);
        }

        SECTION("Check if image count have been initalized right and the getter returns the correct values") {
            REQUIRE(input_data_container->get_image_count() == 10);
        }

        delete input_data_container;
    }

    SECTION("Check select_an_input method.") {
        SECTION("Check with valid data.") {
            Config config;
            config.set_image_count(1);
            InputDataContainer* input_data_container = new InputDataContainer(inputs_dir.string(), config);

            for (int i = 0; i < 10; i++)
                REQUIRE_NOTHROW(input_data_container->select_an_input().second < 8);
            REQUIRE_THROWS_AS(input_data_container->select_an_input(), NoValidInputsLeftException);

            delete input_data_container;
        }

        SECTION("Check empty data if it throws NoValidInputsLeftException.") {
            InputDataContainer* input_data_container = new InputDataContainer(inputs_dir.string(), Config());

            REQUIRE_THROWS_AS(input_data_container->select_an_input(), NoValidInputsLeftException);

            delete input_data_container;
        }

        SECTION("Check if it throws NoValidInputsLeftException when only 1 input is left.") {
            Config config;
            config.set_image_count(1);
            InputDataContainer* input_data_container = new InputDataContainer(inputs_dir.string(), config);

            for (int i = 0; i < 9; i++) {
                QPair<Image *, unsigned int> image = input_data_container->select_an_input();
                input_data_container->clean_up_finished_image(image);
            }

            REQUIRE_NOTHROW(input_data_container->select_an_input().second < 8);
            REQUIRE_THROWS_AS(input_data_container->select_an_input(), NoValidInputsLeftException);

            delete input_data_container;
        }
    }

    SECTION("Check if on_image_used method works as intended.") {
        SECTION("Check if it returns correct valid data with valid input") {
            Config config;
            config.set_image_count(2);
            InputDataContainer* input_data_container = new InputDataContainer(inputs_dir.string(), config);

            for (int i = 0; i < 9; i++) {
                QPair<Image *, unsigned int> image = input_data_container->select_an_input();
                input_data_container->clean_up_finished_image(image);
            }

            QPair<Image *, unsigned int> image = input_data_container->select_an_input();
            QVector<QPair<std::string, QPair<cv::Point2f, cv::Point2f>>> boundaries;
            for (int i = 0; i < 2; i++)
                boundaries.push_back(
                    QPair<std::string, QPair<cv::Point2f, cv::Point2f>>(
                        image.first->get_file_path(),
                        QPair<cv::Point2f, cv::Point2f>(
                            cv::Point2f(130, 432),
                            cv::Point2f(321, 123)
                        )
                    )
                );
            int width = 1920;
            int height = 1080;

            InputDataContainer::PostImageUsedData data = input_data_container->on_image_used(image, boundaries, width, height);
            QPair<double, double> image_center = QPair<double, double>(225.5 / width, 277.5 / height);
            QPair<double, double> boundary_size = QPair<double, double>(95.5 / width, 154.5 / height);

            REQUIRE(data.image_center.first == image_center.first);
            REQUIRE(data.image_center.second == image_center.second);
            REQUIRE(data.boundary_size.first == boundary_size.first);
            REQUIRE(data.boundary_size.second == boundary_size.second);
            REQUIRE(data.should_image_be_removed == false);
            REQUIRE(boundaries.size() == 1);

            image = input_data_container->select_an_input();
            data = input_data_container->on_image_used(image, boundaries, width, height);
            REQUIRE(data.should_image_be_removed == true);
            REQUIRE(boundaries.size() == 0);

            delete input_data_container;
        }
    }

    SECTION("Check if clean_up_finished_image method works as intended.") {
        SECTION("Check if it removes from the inputs") {
            Config config;
            config.set_image_count(100);
            InputDataContainer* input_data_container = new InputDataContainer(inputs_dir.string(), config);

            for (int i = 0; i < 10; i++) {
                QPair<Image *, unsigned int> image = input_data_container->select_an_input();
                REQUIRE_NOTHROW(input_data_container->clean_up_finished_image(image));
            }

            REQUIRE_THROWS_AS(input_data_container->select_an_input(), NoValidInputsLeftException);

            delete input_data_container;
        }

        SECTION("Check if it throws BadUsageException when trying delete an item twice") {
            Config config;
            config.set_image_count(100);
            InputDataContainer* input_data_container = new InputDataContainer(inputs_dir.string(), config);

            Image* temp_image = new Image();
            QPair<Image *, unsigned int> image = QPair<Image *, unsigned int>(temp_image, 1);
            REQUIRE_THROWS_AS(input_data_container->clean_up_finished_image(image), BadUsageException);

            delete temp_image;
            delete input_data_container;
        }
    }
}
