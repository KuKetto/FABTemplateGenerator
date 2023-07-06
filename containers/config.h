#ifndef CONFIG_H
#define CONFIG_H


class Config
{
    unsigned image_count;
    unsigned normal_image_count;
    unsigned augment_count;
    unsigned augment_noise;
    unsigned augment_lens_blur;
    unsigned augment_bilateral_blur;
    unsigned augment_image_flip;
    unsigned augment_cut_out;
    unsigned augment_brightness;
    unsigned augment_rgb_shift;
public:
    Config(){};
    unsigned int get_image_count() const;
    void set_image_count(unsigned int new_image_count);

    unsigned int get_normal_image_count() const;
    void set_normal_image_count(unsigned int new_normal_image_count);

    unsigned int get_augment_count() const;
    void set_augment_count(unsigned int new_augment_count);

    unsigned int get_augment_noise() const;
    void set_augment_noise(unsigned int new_augment_noise);

    unsigned int get_augment_lens_blur() const;
    void set_augment_lens_blur(unsigned int new_augment_lens_blur);

    unsigned int get_augment_bilateral_blur() const;
    void set_augment_bilateral_blur(unsigned int new_augment_bilateral_blur);

    unsigned int get_augment_image_flip() const;
    void set_augment_image_flip(unsigned int new_augment_image_flip);

    unsigned int get_augment_cut_out() const;
    void set_augment_cut_out(unsigned int new_augment_cut_out);

    unsigned int get_augment_brightness() const;
    void set_augment_brightness(unsigned int new_augment_brightness);

    unsigned int get_augment_rgb_shift() const;
    void set_augment_rgb_shift(unsigned int new_augment_rgb_shift);
};

#endif // CONFIG_H
