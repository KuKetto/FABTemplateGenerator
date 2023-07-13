#include "cardpositiondata.h"

cv::Point2f CardPositionData::get_cv_point_from_std_pair(std::pair<unsigned int, unsigned int> position) const
{
    return cv::Point2f(position.first, position.second);
}

std::vector<cv::Point2f> CardPositionData::get_perspective_positions() const
{
    std::vector<cv::Point2f> image_perspective_positions;

    image_perspective_positions.push_back(get_cv_point_from_std_pair(top_left_pos));
    image_perspective_positions.push_back(get_cv_point_from_std_pair(top_right_pos));
    image_perspective_positions.push_back(get_cv_point_from_std_pair(bottom_right_pos));
    image_perspective_positions.push_back(get_cv_point_from_std_pair(bottom_left_pos));

    return image_perspective_positions;
}
