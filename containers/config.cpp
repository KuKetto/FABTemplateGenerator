#include "config.h"

unsigned int Config::get_image_count() const
{
    return image_count;
}

void Config::set_image_count(unsigned int new_image_count)
{
    image_count = new_image_count;
}

unsigned int Config::get_normal_image_count() const
{
    return normal_image_count;
}

void Config::set_normal_image_count(unsigned int new_normal_image_count)
{
    normal_image_count = new_normal_image_count;
}

unsigned int Config::get_augment_count() const
{
    return augment_count;
}

void Config::set_augment_count(unsigned int new_augment_count)
{
    augment_count = new_augment_count;
}

unsigned int Config::get_augment_noise() const
{
    return augment_noise;
}

void Config::set_augment_noise(unsigned int new_augment_noise)
{
    augment_noise = new_augment_noise;
}

unsigned int Config::get_augment_lens_blur() const
{
    return augment_lens_blur;
}

void Config::set_augment_lens_blur(unsigned int new_augment_lens_blur)
{
    augment_lens_blur = new_augment_lens_blur;
}

unsigned int Config::get_augment_bilateral_blur() const
{
    return augment_bilateral_blur;
}

void Config::set_augment_bilateral_blur(unsigned int new_augment_bilateral_blur)
{
    augment_bilateral_blur = new_augment_bilateral_blur;
}

unsigned int Config::get_augment_image_flip() const
{
    return augment_image_flip;
}

void Config::set_augment_image_flip(unsigned int new_augment_image_flip)
{
    augment_image_flip = new_augment_image_flip;
}

unsigned int Config::get_augment_cut_out() const
{
    return augment_cut_out;
}

void Config::set_augment_cut_out(unsigned int new_augment_cut_out)
{
    augment_cut_out = new_augment_cut_out;
}

unsigned int Config::get_augment_brightness() const
{
    return augment_brightness;
}

void Config::set_augment_brightness(unsigned int new_augment_brightness)
{
    augment_brightness = new_augment_brightness;
}

unsigned int Config::get_augment_rgb_shift() const
{
    return augment_rgb_shift;
}

void Config::set_augment_rgb_shift(unsigned int new_augment_rgb_shift)
{
    augment_rgb_shift = new_augment_rgb_shift;
}
