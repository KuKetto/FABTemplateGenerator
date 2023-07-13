#include "inputdata.h"
#include <QDebug>

InputData::InputData(const Config &config, const unsigned int& class_id, const std::string class_name):
    class_id(class_id), class_name(class_name)
{
    remaining_image_count = config.get_image_count();
    remaining_augmentation_count = config.get_augment_count();
    remaining_augment_noise_count = config.get_augment_noise();
    remaining_augment_lens_blur_count = config.get_augment_lens_blur();
    remaining_augment_bilateral_blur_count = config.get_augment_bilateral_blur();
    remaining_augment_image_flip_count = config.get_augment_image_flip();
    remaining_augment_cut_out_count = config.get_augment_cut_out();
    remaining_augment_brightness_count = config.get_augment_brightness();
    remaining_augment_rgb_shift_count = config.get_augment_rgb_shift();

    init_possible_operation_types_vector(OPERATION_TYPE::NORMAL, config.get_normal_image_count());
    init_possible_operation_types_vector(OPERATION_TYPE::NOISE, remaining_augment_noise_count);
    init_possible_operation_types_vector(OPERATION_TYPE::LENS_BLUR, remaining_augment_lens_blur_count);
    init_possible_operation_types_vector(OPERATION_TYPE::BILATERAL_BLUR, remaining_augment_bilateral_blur_count);
    init_possible_operation_types_vector(OPERATION_TYPE::IMAGE_FLIP, remaining_augment_image_flip_count);
    init_possible_operation_types_vector(OPERATION_TYPE::CUT_OUT, remaining_augment_cut_out_count);
    init_possible_operation_types_vector(OPERATION_TYPE::BRIGHTNESS_ADJUSTION, remaining_augment_brightness_count);
    init_possible_operation_types_vector(OPERATION_TYPE::RGB_SHIFT, remaining_augment_rgb_shift_count);
}

void InputData::init_possible_operation_types_vector(const unsigned short& operation_type, const unsigned int& operation_type_count)
{
    if (operation_type_count > 0)
        possible_operation_types.push_back(operation_type);
}

unsigned short InputData::use_this_image()
{
    if (remaining_image_count < 1) throw InvalidValueException("InputData::use_this_image", "number greater or equal to 1", std::to_string(remaining_image_count));
    remaining_image_count--;
    if (remaining_augmentation_count < 1) return OPERATION_TYPE::NORMAL;

    unsigned short result = possible_operation_types.at(
        Random::generate_integer(0, possible_operation_types.size() - 1)
    );
    return result;
}

bool InputData::image_used(const unsigned short &operation_type)
{
    switch (operation_type) {
    case OPERATION_TYPE::NOISE:
        check_remaining_value(remaining_augment_noise_count, OPERATION_TYPE::NOISE);
        break;
    case OPERATION_TYPE::LENS_BLUR:
        check_remaining_value(remaining_augment_lens_blur_count, OPERATION_TYPE::LENS_BLUR);
        break;
    case OPERATION_TYPE::BILATERAL_BLUR:
        check_remaining_value(remaining_augment_bilateral_blur_count, OPERATION_TYPE::BILATERAL_BLUR);
        break;
    case OPERATION_TYPE::IMAGE_FLIP:
        check_remaining_value(remaining_augment_image_flip_count, OPERATION_TYPE::IMAGE_FLIP);
        break;
    case OPERATION_TYPE::CUT_OUT:
        check_remaining_value(remaining_augment_cut_out_count, OPERATION_TYPE::CUT_OUT);
        break;
    case OPERATION_TYPE::BRIGHTNESS_ADJUSTION:
        check_remaining_value(remaining_augment_brightness_count, OPERATION_TYPE::BRIGHTNESS_ADJUSTION);
        break;
    case OPERATION_TYPE::RGB_SHIFT:
        check_remaining_value(remaining_augment_rgb_shift_count, OPERATION_TYPE::RGB_SHIFT);
        break;
    }

    if (operation_type > 0) remaining_augmentation_count--;

    if (remaining_image_count < 1) return true;
    return false;
}

bool InputData::decrease_used_operation_type(unsigned int &value)
{
    value--;
    return value < 1;
}

void InputData::check_remaining_value(unsigned int &value, unsigned short operation_type)
{
    if(decrease_used_operation_type(value)) {
        int index = possible_operation_types.indexOf(operation_type);
        if (index >= 0) possible_operation_types.remove(index);
    }
}
