#ifndef CONFIGREADER_H
#define CONFIGREADER_H

#include <QDebug>
#include <fstream>
#include "../nlohmann-json/json.hpp"

class ConfigReader
{
public:
    struct Config {
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
    };
private:
    enum DefaultConfig {
        IMAGE_COUNT = 150,
        AUGMENT_NOISE = 6,
        AUGMENT_LENS_BLUR = 10,
        AUGMENT_BILATERAL_BLUR = 10,
        AUGMENT_IMAGE_FLIP = 7,
        AUGMENT_CUT_OUT = 14,
        AUGMENT_BRIGHTNESS = 14,
        AUGMENT_RGB_SHIFT = 14
    };

    std::string file_path;
    Config config;
    void write_default_config();
public:
    ConfigReader(const std::string& file_path);

    bool read_config();
    Config get_config();
};

#endif // CONFIGREADER_H
