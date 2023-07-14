#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "../../containers/templatecontainer.h"
#include "../../containers/templatecontainer.cpp"
#include "../../utils/exceptions/novalidinputsleftexception.h"
#include "../../utils/exceptions/invalidtemplateexception.h"
#include "../../utils/exceptions/invalidtemplateexception.cpp"

TEST_CASE("TemplateContainer class", "[TemplateContainer]") {
    std::string current_file = __FILE__;
    std::filesystem::path current_path(current_file);
    std::filesystem::path templates_dir = current_path.parent_path().parent_path().parent_path() / "examples/templates";

    SECTION("Check constructor.") {
        SECTION("Constructor can be called without any error.") {
            TemplateContainer* template_container = nullptr;

            REQUIRE_NOTHROW(template_container = new TemplateContainer(".", 0));

            delete template_container;
        }

        SECTION("Constructor throws exception with invalid path.") {
            TemplateContainer* invalid_path_template_container = nullptr;

            REQUIRE_THROWS_AS(invalid_path_template_container = new TemplateContainer("invalid path", -1), PathNotFoundException);

            delete invalid_path_template_container;
        }
    }

    SECTION("Check select_a_template method") {
        SECTION("Check if it returns templates only with required image count less than input") {
            short number_of_images = 4;
            TemplateContainer* template_container = new TemplateContainer(templates_dir.string(), number_of_images);

            for (int i = 0; i < 10; i++) {
                Image* _template = template_container->select_a_template();
                _template->open();
                REQUIRE_NOTHROW(_template->get_card_positions_size() <= number_of_images);
                _template->close();
            }

            delete template_container;
        }

        SECTION("Check if it throws NoValidInputsLeftException with 0 template input") {
            TemplateContainer* template_container = new TemplateContainer(".", 0);

            REQUIRE_THROWS_AS(template_container->select_a_template(), NoValidInputsLeftException);

            delete template_container;
        }
    }

    SECTION("Check on_image_deleted method") {
        SECTION("Check if removes the template when the counter is lower than it's required image count") {
            short number_of_images = 5;
            TemplateContainer* template_container = new TemplateContainer(templates_dir.string(), number_of_images);
            for (int i = 0; i < 5; i++) {
                REQUIRE_NOTHROW(template_container->select_a_template());
                Image* _template = template_container->select_a_template();
                _template->open();
                REQUIRE(_template->get_card_positions_size() <= number_of_images);
                _template->close();

                number_of_images--;
                REQUIRE_NOTHROW(template_container->on_image_deleted());
            }

            REQUIRE_THROWS_AS(template_container->select_a_template(), NoValidInputsLeftException);

            delete template_container;
        }

        SECTION("Check if it throws BadUsageException when called on an empty container.") {
            short number_of_images = 1;
            TemplateContainer* template_container = new TemplateContainer(templates_dir.string(), number_of_images);

            REQUIRE_NOTHROW(template_container->on_image_deleted());
            REQUIRE_THROWS_AS(template_container->on_image_deleted(), BadUsageException);

            delete template_container;

            TemplateContainer* no_valid_input_template_container = new TemplateContainer(".", number_of_images);

            REQUIRE_THROWS_AS(template_container->on_image_deleted(), BadUsageException);

            delete no_valid_input_template_container;
        }
    }
}
