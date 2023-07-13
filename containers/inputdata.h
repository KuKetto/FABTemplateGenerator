#ifndef INPUTDATA_H
#define INPUTDATA_H

#include "config.h"
#include <iostream>
#include "../utils/random.h"
#include "../utils/exceptions/invalidvalueexception.h"
#include <QVector>

/**
 * @file CardPositionData.h
 *
 * @brief This class is responsible for storing
 * how much operation is left on an image from
 * a given type.
 *
 * @author Domonkos Say
 *
 */

/** @brief An enum containting a number for operation types,
 *  used inside switches.
 */
enum OPERATION_TYPE{
    NORMAL = 0,
    NOISE = 1,
    LENS_BLUR = 2,
    BILATERAL_BLUR = 3,
    IMAGE_FLIP = 4,
    CUT_OUT = 5,
    BRIGHTNESS_ADJUSTION = 6,
    RGB_SHIFT = 7
};

class InputData
{
    unsigned remaining_image_count;
    unsigned remaining_augmentation_count;
    unsigned remaining_augment_noise_count;
    unsigned remaining_augment_lens_blur_count;
    unsigned remaining_augment_bilateral_blur_count;
    unsigned remaining_augment_image_flip_count;
    unsigned remaining_augment_cut_out_count;
    unsigned remaining_augment_brightness_count;
    unsigned remaining_augment_rgb_shift_count;

    unsigned class_id;
    std::string class_name;
    QVector<unsigned short> possible_operation_types;

    /** @brief A helper function to simplify the code in the constructor.
     *  Pushes an operation type to the possible_operation_types QVector
     *  if the operation's count is at least 1.
     *
     *  @param operation_type: A number representing an operation type
     *  refering to the value in OPERATION_TYPE enum.
     *  @param operation_type_count: A number representing the count of
     *  the previously mentioned operation type.
     */
    void init_possible_operation_types_vector(const unsigned short& operation_type, const unsigned& operation_type_count);

    /** @brief A helper function to simplify the code in the
     *  check_remaining_value method. The reference of the value
     *  is decreased by one then it returns true if the value is
     *  less than 1. If true the operation type should be removed
     *  from possible_operation_types QVector.
     *
     *  @param value: A reference to the remaining count of an
     *  operation type.
     */
    bool decrease_used_operation_type(unsigned& value);

    /** @brief A helper function to simplify the code in the
     *  image_used method. It calls the decrease_used_operation_type
     *  method then removes the operation type from the
     *  possible_operation_types QVector if the method returned true.
     *
     *  @param value: A reference to the remaining count of an
     *  operation type.
     *  @param operation_type: A number representing an operation type
     *  refering to the value in OPERATION_TYPE enum.
     */
    void check_remaining_value(unsigned& value, unsigned short operation_type);
public:
    /** @brief Constructor, creates a new InputData object.
     *  Requires a config to initalize the starting values
     *  of the remaining operation type counts.
     *
     *  @param config: A Config object read or created by ConfigReader utility.
     *  @param class_id: A class id which will be used for image annotation.
     *  @param class_name: A class name used to generate classes.txt which
     *  includes class_id - filename pairs.
     */
    InputData(const Config& config, const unsigned& class_id, const std::string class_name);

    /** @brief A method to decide what operation to use
     *  on the image. Decreases remaining_image_count by one
     *  then selects a random index of possible_operation_types QVector
     *  if the augmentation count is at least 1. Else it returns
     *  normal operation type (no augmentation will be required).
     *  The operation type at this index will be returned.
     *
     *  @throws Utils::Exceptions::InvalidValueException if
     *  image count is less than 1.
     */
    unsigned short use_this_image();

    /** @brief A method to be called when image processing
     *  is finished. Looks up which operation type was
     *  used with check_remaining_value method.
     *  Returns true if the remaining image count to be
     *  generated is less than one. If it returns true
     *  it will be removed from Containers::InputDataContainer
     *
     *  @param operation_type: A number representing an operation type
     *  refering to the value in OPERATION_TYPE enum.
     */
    bool image_used(const unsigned short& operation_type);

    /** @brief A getter to the class id set in constructor */
    unsigned get_class_id() const {return class_id;};
};

#endif // INPUTDATA_H
