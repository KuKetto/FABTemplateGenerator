#ifndef INPUTDATA_H
#define INPUTDATA_H

#include "config.h"
#include <iostream>
#include "../utils/random.h"
#include <QVector>

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

    void init_possible_operation_types_vector(unsigned short operation_type, unsigned operation_type_count);
    bool decrease_used_operation_type(unsigned& value);
    void check_remaining_value(unsigned& value, unsigned short operation_type);
public:
    InputData(const Config& config, const unsigned class_id, const std::string class_name);

    unsigned short use_this_image();
    bool image_used(const unsigned short& operation_type);
    unsigned get_class_id() const {return class_id;};
};

#endif // INPUTDATA_H
