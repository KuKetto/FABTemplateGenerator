#ifndef CARDPOSITIONDATA_H
#define CARDPOSITIONDATA_H

#include <vector>
#include <opencv2/core.hpp>

class CardPositionData
{
    std::pair<unsigned int, unsigned int> top_left_pos;
    std::pair<unsigned int, unsigned int> top_right_pos;
    std::pair<unsigned int, unsigned int> bottom_right_pos;
    std::pair<unsigned int, unsigned int> bottom_left_pos;

    cv::Point2f get_cv_point_from_std_pair(std::pair<unsigned int, unsigned int> position);
public:
    CardPositionData(){};
    void set_top_left_pos(std::pair<unsigned int, unsigned int> top_left_pos) {this->top_left_pos = top_left_pos;};
    void set_top_right_pos(std::pair<unsigned int, unsigned int> top_right_pos) {this->top_right_pos = top_right_pos;};
    void set_bottom_right_pos(std::pair<unsigned int, unsigned int> bottom_right_pos) {this->bottom_right_pos = bottom_right_pos;};
    void set_bottom_left_pos(std::pair<unsigned int, unsigned int> bottom_left_pos) {this->bottom_left_pos = bottom_left_pos;};

    std::pair<unsigned int, unsigned int> get_top_left_pos() {return top_left_pos;};
    std::pair<unsigned int, unsigned int> get_top_right_pos() {return top_right_pos;};
    std::pair<unsigned int, unsigned int> get_bottom_right_pos() {return bottom_right_pos;};
    std::pair<unsigned int, unsigned int> get_bottom_left_pos() {return bottom_left_pos;};

    std::vector<cv::Point2f> get_perspective_positions();
};

#endif // CARDPOSITIONDATA_H
