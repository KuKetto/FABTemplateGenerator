#ifndef IMAGEOVERLAY_H
#define IMAGEOVERLAY_H

#include <cstdlib>

#include "../containers/image.h"

#include "../utils/exceptions/invalidvalueexception.h"
#include "../utils/exceptions/badusageexception.h"
#include "../utils/exceptions/invalidtemplateexception.h"

/**
 * @file imageoverlay.h
 *
 * @class ImageOverlay
 * @brief This class is responsible for overlaying
 * the input images on template images based on the
 * template's CardPositionData
 *
 * @author Domonkos Say
 *
 */

class ImageOverlay
{
    Image* template_image;
    QVector<Image*> images_to_overlay;
    bool was_template_image_closed = false;
    QVector<bool> were_input_images_closed;

    /** @brief A helper function to simplify the code in the overlay method.
     *  Returns an OpenCV cv::Mat object which pairs together:
     *  - the input image's perspective (the 4 corner point of the input image)
     *  - and the desired perspective defined in the template's image
     *  CardPositionData at the given index
     *
     *  via the cv::getPerspectiveTransform method.
     *  @param image_index: The selected index of both images_to_overlay QVector
     *  and template image's Image::card_positions QVector.
     */
    cv::Mat create_perspective_matrix_for_image(const int& image_index) const;

    /** @brief A helper function to simplify the code in the overlay method.
     *  Returns the template's image size which is the perspective transform's
     *  desired size.
     */
    cv::Size get_desired_size() const;

    /** @brief A helper function to simplify the code in the overlay method.
     *  Creates and returns a binary mask of the perspective warped input image
     *  as an OpenCV cv::Mat object.
     *
     *  Mask is a one channel image object. Checks for the transparent regions
     *  of the perspective warped input image (which is stored at the 4th channel).
     *  If the mask is transparent at a pixel position it will be blacked out in
     *  the mask else it's white. Then it's converted to binary.
     *
     *  @param: alpha: An image with alpha layer which we want to make mask of.
     */
    cv::Mat create_mask_from_alpha(const cv::Mat& alpha) const;

    /** @brief A helper function to simplify the code in the overlay method.
     *  It looks for the top left and bottom right corner of the warped perspective
     *  based on the return paramter of CardPositionData::get_card_positions_perspective().
     *
     *  top left corner found by minimum x and y
     *
     *  bottom right corner found by maximum x and y
     *
     *  @param points: A cv::Point2f vector of points use the return statement of
     *  CardPositionData::get_card_positions_perspective() method
     */
    QPair<cv::Point2f, cv::Point2f> get_bounding_boxes(const std::vector<cv::Point2f>& points) const;
public:
    /**
     *  @struct OverlayedResult
     *  @brief A basic struct used as return statement in overlay method
     *
     *  @variable overlayed_image: result, overlayed image
     *  @variable image_width: result, overlayed image's width, same as template image's width
     *  @variable image_height: result, overlayed image's height, same as template image's height
     *  @variable input_overlay_positions: a vector of QPair<filepath, bounding_boxes> which is used
     *  in InputDataContainer::on_image_used method
     */
    struct OverlayedResult {
        cv::Mat overlayed_image;
        int image_width;
        int image_height;
        QVector<QPair<std::string, QPair<cv::Point2f, cv::Point2f>>> input_overlay_positions;
    };

    /** @brief Creates an empty ImageOverlay object
     *  use the setters to set the required template image
     *  and vector of images to be overlayed with.
     *
     *  Assumes all the Image objects are open.
     */
    ImageOverlay();

    /** @brief Destructs the ImageOverlay object.
     *  Does not deallocate the set Image objects,
     *  it's the developer's responsibility to
     *  allocate and deallocate them.
     *
     *  Safety mechanism, closes images that were need to be
     *  opened in set_images_to_overlay or overlay method because
     *  bad usage.
     */
    ~ImageOverlay();

    /** @brief A setter for the template image
     *  Returns the required image count (size of Image::card_positions)
     *
     *  @param template_image: A template is an Image object created with 'true'
     *  value in it's constructor.
     *
     *  Assumes template_image is open. If the required image is 0 it may be closed.
     *  Then it tries to open it if it isn't. It will be closed after calling the
     *  overlay function or in the destructor.
     *
     *  @throws Utils::Exceptions::BadUsageException if not a template object was
     *  provided
     *  @throws Utils::Exceptions::InvalidValueException if a nullpointer was
     *  provided
     *  @throws Utils::Exceptions::InvalidTemplateException if a template with
     *  0 required images was provided
    */
    int set_template_image(Image* template_image);

    /** @brief A setter for the template image
     *  Requires items equal to set_template_image method's return value.
     *
     *  @param images_to_overlay: A QVector of Image objects. These will
     *  be overlayed on the top of the template image.
     *
     *  Assumes all the template Image object is set and open. It will try to open
     *  the template if they hasn't been opened. It will be closed after calling the
     *  overlay function or in the destructor.
     *
     *  @throws Utils::Exceptions::BadUsageException if template hasn't been
     *  set beforehand
     *  @throws Utils::Exceptions::InvalidValueException if the QVector's
     *  size not matches to the required image count by the template image.
    */
    void set_images_to_overlay(QVector<Image*> images_to_overlay);

    /** @brief Overlays the input images on top of the template
     *  image. The images are not changed, a new image will be returned.
     *
     *  Iterates through the input images and warp their perspective
     *  based on the template image's card data (Image::card_positions).
     *  Then it masks the warped images and use those masks to overlay the
     *  image on top of the template image.
     *
     *  Assumes all the Image objects are set and open. It will try to open
     *  the images if they haven't been opened and close them before returning.
     *  @throws Utils::Exceptions::BadUsageException if one the input hasn't been set
     */
    OverlayedResult overlay();
};

#endif // IMAGEOVERLAY_H
