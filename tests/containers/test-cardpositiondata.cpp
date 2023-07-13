#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "../../containers/cardpositiondata.h"
#include "../../containers/cardpositiondata.cpp"

TEST_CASE("CardPositionData class", "[CardPositionData]") {
    SECTION("Constructor can be called without any error.") {
        CardPositionData* card = nullptr;

        REQUIRE_NOTHROW(card = new CardPositionData());

        delete card;
    }

    SECTION("Setters and Getters are working as expected") {
        CardPositionData* card = new CardPositionData();

        SECTION("Check if top left position can be set without exception") {
            REQUIRE_NOTHROW(card->set_top_left_pos(std::pair<unsigned int, unsigned int>(0,0)));
        }

        SECTION("Check if top right position can be set without exception") {
            REQUIRE_NOTHROW(card->set_top_right_pos(std::pair<unsigned int, unsigned int>(0,1)));
        }

        SECTION("Check if bottom right position can be set without exception") {
            REQUIRE_NOTHROW(card->set_bottom_right_pos(std::pair<unsigned int, unsigned int>(1,1)));
        }

        SECTION("Check if bottom left position can be set without exception") {
            REQUIRE_NOTHROW(card->set_bottom_left_pos(std::pair<unsigned int, unsigned int>(1,0)));
        }

        card->set_top_left_pos(std::pair<unsigned int, unsigned int>(0,0));
        card->set_top_right_pos(std::pair<unsigned int, unsigned int>(0,1));
        card->set_bottom_right_pos(std::pair<unsigned int, unsigned int>(1,1));
        card->set_bottom_left_pos(std::pair<unsigned int, unsigned int>(1,0));

        SECTION("Top left position set successfuly, and getter returns correct value") {
            REQUIRE(card->get_top_left_pos() == std::pair<unsigned int, unsigned int>(0,0));
        }

        SECTION("Top right position set successfuly, and getter returns correct value") {
            REQUIRE(card->get_top_right_pos() == std::pair<unsigned int, unsigned int>(0,1));
        }

        SECTION("Bottom right position set successfuly, and getter returns correct value") {
            REQUIRE(card->get_bottom_right_pos() == std::pair<unsigned int, unsigned int>(1,1));
        }

        SECTION("Bottom left position set successfuly, and getter returns correct value") {
            REQUIRE(card->get_bottom_left_pos() == std::pair<unsigned int, unsigned int>(1,0));
        }

        delete card;
    }

    SECTION("CardPositionData returns the correct perspective position vector") {
        CardPositionData* card = new CardPositionData();

        card->set_top_left_pos(std::pair<unsigned int, unsigned int>(0,0));
        card->set_top_right_pos(std::pair<unsigned int, unsigned int>(0,1));
        card->set_bottom_right_pos(std::pair<unsigned int, unsigned int>(1,1));
        card->set_bottom_left_pos(std::pair<unsigned int, unsigned int>(1,0));

        std::vector<cv::Point2f> pos{cv::Point2f(0,0), cv::Point2f(0,1), cv::Point2f(1,1), cv::Point2f(1,0)};
        REQUIRE(card->get_perspective_positions() == pos);

        delete card;
    }
}
