#ifndef INPUTDATACONTAINER_H
#define INPUTDATACONTAINER_H

#include "inputdata.h"
#include "image.h"
#include "config.h"
#include "../utils/exceptions/dividebyzero.h"
#include "../utils/exceptions/novalidinputsleftexception.h"
#include "../utils/exceptions/badusageexception.h"

/**
 * @file inputdatacontainer.h
 *
 * @class InputDataContainer
 * @brief This class is responsible for storing
 * how much operation is left on an image from
 * a given type.
 *
 * @author Domonkos Say
 *
 */

class InputDataContainer
{
    QVector<QPair<Image*,InputData>> inputs;
    QMap<unsigned, std::string> classes;

    /** @brief A helper function to simplify the code in the
     *  get_image_normalized_boundaries method. Normalize a
     *  value based on the normalizing factor. This is required
     *  as images annotated with the image's size normalized.
     *
     *  @param value: A value that will be normalized
     *  @param normalizing_factor: A factor value will be
     *  normalized with.
     *
     *  @footnote
     *  normalize_value(10, 100) = 10 / 100 = 0.1
     *  @endfootnote
     *
     *  @throws Utils::Exceptions::DivideByZero if normalizing_factor
     *  is 0
     */
    double normalize_value(const double& value, const double& normalizing_factor) const;

    /** @brief A helper function to simplify the code in the
     *  on_image_used method. Returns the boundaries of an
     *  overlayed image. These contain the normalized values
     *  of the overlayed image's center and the normalized
     *  values of the boundary boy width and height. (Used
     *  normalizing factor: output image's width and height.
     *
     *  @param boundaries: A pair of 2 cv::Point2f points.
     *  The first is the overlayed image's top left and the
     *  second is the overlayed image's bottom right corner.
     *  @param image_width: The output image's width
     *  @param image_height: The output image's height
     */
    QPair<QPair<double, double>, QPair<double, double>> get_image_normalized_boundaries(const QPair<cv::Point2f, cv::Point2f>& boundaries, const int& image_width, const int& image_height) const;
public:
    struct PostImageUsedData {
        bool should_image_be_removed;
        unsigned class_id;
        QPair<double, double> boundary_size;
        QPair<double, double> image_center;
    };

    /** @brief Constructor, creates a new InputDataContainer object.
     *  Requires a directory of images.
     *
     *  @param directory: Directory path where the input images are located
     *  @param config: A Config object read or created by ConfigReader utility.
     *
     *  @throws Utils::Directory::PathNotFoundException if the given directory
     *  path is invalid
     */
    InputDataContainer(const std::string& directory, const Config& config);

    /** @brief Destructs the allocated data in inputs QVector
     *  by calling Image::close methods on the Image objects
     *  then deleting them.
     */
    ~InputDataContainer();

    /** @brief A getter for classes QMap. It stores the class id
     *  and class name pairs.
     */
    QMap<unsigned, std::string> get_classes() const {return classes;}

    /** @brief Returns how much valid inputs is left.
     *  An input is valid if it's remaining image count is greater
     *  than 0.
     */
    unsigned int get_image_count() const {return inputs.size();}

    /** @brief Selects an input to be use by Generation::generate.
     *  Selects a random number as an index of inputs QVector.
     *  It then tries to select an operation the image. If succeeds
     *  returns the OpenCV image object and the operation.
     *  If not:
     *      - not enough images were left: the generation stops
     *      - or randomly an invalid image were selected. In this case it is removed and the inputs QVector is looped through until a valid image is found.
     *
     *  @throws Utils::Exceptions::NoValidInputsLeftException if
     *  no valid images are left. In this case the generation finishes.
    */
    QPair<Image*, unsigned int> select_an_input();

    /** @brief To be called after the overlaying process finished.
     *  Look up the image in the inputs QVector. Creates and returns
     *  PostImageUsedData struct which includes if this image should be
     *  deleted. The image object is not deleted here just marked, maybe
     *  it was used multiple times during the overlaying process.
     *  Also returns the class id, image's boundaries and the boundary's size
     *  to be used during annotation.
     *
     *  @param image: Image object that has been used. Same as the return
     *  value of select_an_input method.
     *  @param boundaries: A QVector containing the positions (CardPositionData)
     *  of the template where images were overlayed on top of. Contains the images
     *  file paths to be matched to the correct Image object.
     *  @param image_width: The output image's width
     *  @param image_height: The output image's height
     */
    PostImageUsedData on_image_used(const QPair<Image*, unsigned int>& image, QVector<QPair<std::string, QPair<cv::Point2f, cv::Point2f>>>& boundaries, int image_width, int image_height);

    /** @brief To be called on the images that had been marked for
     *  deletion by on_image_used method.
     *  Looks up for the image in the inputs QVector matching it by file
     *  paths then frees it's allocated data and removes it from the container.
     *
     *  @param image: Image object that has been used. Same as the return
     *  value of select_an_input method.
     *
     *  @throws Utils::Exceptions::BadUsageException if the image is not in
     *  the inputs QVector.
     */
    void clean_up_finished_image(const QPair<Image*, unsigned int>& image);
};

#endif // INPUTDATACONTAINER_H
