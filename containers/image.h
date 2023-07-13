#ifndef IMAGE_H
#define IMAGE_H

#include <opencv2/opencv.hpp>
#include <QVector>
#include "../utils/zlibcustomimageextractor.h"
#include "cardpositiondata.h"
#include "../utils/exceptions/filenotfound.h"
#include "../utils/exceptions/libraryexception.h"
#include "../utils/exceptions/badusageexception.h"

/**
 * @file Image.h
 *
 * @brief This class is responsible for storing
 * OpenCV image objects and in case of templates
 * CardPositionData as well.
 *
 * @author Domonkos Say
 *
 */

class Image
{
    std::string file_path;
    cv::Mat image;
    QVector<CardPositionData>* card_positions;

    bool is_template = false;
    bool is_image_open = false;
    void load_card_data(nlohmann::json json_data);
public:
    /** @brief Creates an empty Image object
     *  while setting if it's either a template
     *  or just a regular image.
     *
     *  @param is_template: sets if it's either
     *  a template or just a regular image.
     *
     *  @footnote
     *
     *  Set the file path and call open.
     *  You can access variables via getters.
     *  Call the close method to free it's
     *  allocated memory.
     *
     *  @endfootnote
    */
    Image(const bool& is_template = false);

    /** @brief Destructor. If the object
     *  is in opened state (not closed) calls
     *  the close method to free it's
     *  allocated memory.
     */
    ~Image();



    /** @brief Sets the absolute file path
     *  of the image (jpg/png/...) or template (zip).
     *
     *  @param file_path: absolute file path to the file
     */
    void set_file_path(const std::string& file_path);

    /** @brief Opens the file while alloccating memory dynamicly.
     *  Uses Utils::ZLibCustomImageExtractor to open templates.
     *  Uses cv::imread to open image files.
     *
     *  @throws Utils::Utils::Exceptions::FileNotFound if the file path is empty
     *  @throws Utils::Utils::Exceptions::LibraryException if OpenCV fails reading the image
     */
    void open();

    /** @brief Deallocates the memory allocated
     *  by the open method.
     *  Sets the OpenCV image cv::Mat object to be empty
     *  and frees the dynamicly allocated memory
     *  inside card_positions
     *
     *  @param destruct: if set to true it won't reallocate
     *  card_positions vector
     */
    void close(const bool& destruct = false);

    /** @brief Shows the read OpenCV image cv::Mat object
     *  with OpenCV's cv::imshow method. This is mostly used
     *  for testing purposes.
     */
    void show() const;



    /** @brief Returns if the Image object is open or not*/
    bool is_open() const {return is_image_open;}

    /** @brief A getter for the file path
     *  Used to compare Image objects.
     */
    std::string get_file_path() const {return file_path;}

    /** @brief Returns the size of the card_positions vector*/
    unsigned int get_card_positions_size() const {return card_positions->size();}

    /** @brief Returns the read OpenCV image cv::Mat object*/
    cv::Mat get_opencv_image_object() const {return image;}

    /** @brief Sets the read OpenCV image cv::Mat object
     *  Used during augmenting to overwrite the read image.
     *
     *  @param image: new OpenCV cv::Mat object to overwrite the current one
     */
    void set_opencv_image_object(const cv::Mat& image) {this->image = image;}

    /** @brief Returns the perspective vector for image warping
     *  for the selected card position.
     *  For more, see: Containers::CardPositionData::get_perspective_positions
     *
     *  @param index: new OpenCV cv::Mat object to overwrite the current one
     */
    std::vector<cv::Point2f> get_card_positions_perspective(const int& index) const;
};

#endif // IMAGE_H
