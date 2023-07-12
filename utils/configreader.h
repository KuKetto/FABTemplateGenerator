#ifndef CONFIGREADER_H
#define CONFIGREADER_H

#include <QDebug>
#include <fstream>
#include "../nlohmann-json/json.hpp"
#include "../containers/config.h"

class ConfigReader
{
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

    bool read_config(const bool& write_config_if_it_does_not_exists = true);
    Config get_config();
};

#endif // CONFIGREADER_H
