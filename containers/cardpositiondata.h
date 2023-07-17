#ifndef CARDPOSITIONDATA_H
#define CARDPOSITIONDATA_H

#include <vector>
#include <opencv2/core.hpp>

/**
 * @file cardpositiondata.h
 *
 * @class CardPositionData
 * @brief This class is responsible for storing
 * card positions in the template image.
 *
 * @author Domonkos Say
 *
 */

class CardPositionData
{
    std::pair<unsigned int, unsigned int> top_left_pos;
    std::pair<unsigned int, unsigned int> top_right_pos;
    std::pair<unsigned int, unsigned int> bottom_right_pos;
    std::pair<unsigned int, unsigned int> bottom_left_pos;

    /** @brief This function is converting the stored
     *  std::pair<unsigned int, unsigned int> positions
     *  to OpenCV's position format (cv::Point2f)
     *
     * @param position: position format stored in CardPositionData
     * @returns an OpenCV cv::Point2f representing a corner of a card
     */
    cv::Point2f get_cv_point_from_std_pair(std::pair<unsigned int, unsigned int> position) const;
public:
    /** @brief Creates an empty CardPositionData object.
     *  This object stores a card's each corner with 4
     *  std::pair<unsigned int, unsigned int> in (x,y)
     *  order. Use the setters to set the x and y values
     *  of the card's corners.
    */
    CardPositionData(){};



    /** @brief A setter for the card's top left corner position (x,y) pair
     *
     *  @param top_left_pos: a std::pair of a position in (x,y) order
    */
    void set_top_left_pos(const std::pair<unsigned int, unsigned int>& top_left_pos) {this->top_left_pos = top_left_pos;};

    /** @brief A setter for the card's top right corner position (x,y) pair
     *
     *  @param top_right_pos: a std::pair of a position in (x,y) order
    */
    void set_top_right_pos(const std::pair<unsigned int, unsigned int>& top_right_pos) {this->top_right_pos = top_right_pos;};

    /** @brief A setter for the card's bottom right corner position (x,y) pair
     *
     *  @param bottom_right_pos: a std::pair of a position in (x,y) order
    */
    void set_bottom_right_pos(const std::pair<unsigned int, unsigned int>& bottom_right_pos) {this->bottom_right_pos = bottom_right_pos;};

    /** @brief A setter for the card's bottom left corner position (x,y) pair
     *
     *  @param bottom_left_pos: a std::pair of a position in (x,y) order
    */
    void set_bottom_left_pos(const std::pair<unsigned int, unsigned int>& bottom_left_pos) {this->bottom_left_pos = bottom_left_pos;};



    /** @brief A getter for the card's top left corner position (x,y) pair*/
    std::pair<unsigned int, unsigned int> get_top_left_pos() const {return top_left_pos;};
    /** @brief A getter for the card's top right corner position (x,y) pair*/
    std::pair<unsigned int, unsigned int> get_top_right_pos() const {return top_right_pos;};
    /** @brief A getter for the card's bottom right corner position (x,y) pair*/
    std::pair<unsigned int, unsigned int> get_bottom_right_pos() const {return bottom_right_pos;};
    /** @brief A getter for the card's bottom left corner position (x,y) pair*/
    std::pair<unsigned int, unsigned int> get_bottom_left_pos() const {return bottom_left_pos;};



    /** @brief This function is responsible for
     *  creating a proper destination array for OpenCV's
     *  cv::getPerspectiveTransform function.
     *  For more information see ImageOverlay::create_perspective_matrix_for_image
     *
     *  More on about cv::getPerspectiveTransform:
     *  https://docs.opencv.org/4.x/da/d54/group__imgproc__transform.html
     *
     * @returns an std::vector of cv::Point2f which represents each corner of a card
     */
    std::vector<cv::Point2f> get_perspective_positions() const;
};

#endif // CARDPOSITIONDATA_H
