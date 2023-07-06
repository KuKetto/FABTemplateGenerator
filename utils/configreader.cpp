#include "configreader.h"

ConfigReader::ConfigReader(const std::string &file_path):
    file_path(file_path)
{

}

bool ConfigReader::read_config()
{
    try {
        std::ifstream config_file(file_path);
        if (!config_file.is_open()) {
            throw std::runtime_error("File not found.");
        }

        nlohmann::json json_config = nlohmann::json::parse(config_file);

        config.image_count = json_config["image_count"];
        config.augment_noise = json_config["augment_noise"];
        config.augment_lens_blur = json_config["augment_lens_blur"];
        config.augment_bilateral_blur = json_config["augment_bilateral_blur"];
        config.augment_image_flip = json_config["augment_image_flip"];
        config.augment_cut_out = json_config["augment_cut_out"];
        config.augment_brightness = json_config["augment_brightness"];
        config.augment_rgb_shift = json_config["augment_rgb_shift"];
    } catch (const std::exception& e) {
        if (const nlohmann::json::parse_error* parse_error = dynamic_cast<const nlohmann::json::parse_error*>(&e)) {
            qDebug() << "The config file has a json parse error. Please check your config file! Exception: " << parse_error->what();
            return false;
        }

        if (const std::out_of_range* out_of_range_error = dynamic_cast<const std::out_of_range*>(&e)) {
            qDebug() << "The config file is missing a required field. Please check your config file! Exception: " << out_of_range_error->what();
            return false;
        }

        if (const std::runtime_error* runtime_error = dynamic_cast<const std::runtime_error*>(&e)) {
            qDebug() << "The config file does not exists. Proceeding with the default values. A config file will be generated. Exception: " << runtime_error->what();

            config.image_count = DefaultConfig::IMAGE_COUNT;
            config.augment_noise = DefaultConfig::AUGMENT_NOISE;
            config.augment_lens_blur = DefaultConfig::AUGMENT_LENS_BLUR;
            config.augment_bilateral_blur = DefaultConfig::AUGMENT_BILATERAL_BLUR;
            config.augment_image_flip = DefaultConfig::AUGMENT_IMAGE_FLIP;
            config.augment_cut_out = DefaultConfig::AUGMENT_CUT_OUT;
            config.augment_brightness = DefaultConfig::AUGMENT_BRIGHTNESS;
            config.augment_rgb_shift = DefaultConfig::AUGMENT_RGB_SHIFT;

            write_default_config();
        } else {
            qDebug() << "Exception: " << e.what();
            return false;
        }
    }

    config.augment_count = config.augment_noise + config.augment_lens_blur + config.augment_bilateral_blur
                            + config.augment_image_flip + config.augment_cut_out + config.augment_brightness
                            + config.augment_rgb_shift;

    if (config.augment_count > config.image_count) {
        qDebug() << "You want to create more augments than images (augment count > image_count). Please check your config file";
        return false;
    }
    config.normal_image_count = config.image_count - config.augment_count;
    return true;
}

void ConfigReader::write_default_config()
{
    qDebug() << "Generating config file in the current working directory";
    file_path = "./config.json";

    nlohmann::json json_config;

    json_config["image_count"] = DefaultConfig::IMAGE_COUNT;
    json_config["augment_noise"] = DefaultConfig::AUGMENT_NOISE;
    json_config["augment_lens_blur"] = DefaultConfig::AUGMENT_LENS_BLUR;
    json_config["augment_bilateral_blur"] = DefaultConfig::AUGMENT_BILATERAL_BLUR;
    json_config["augment_image_flip"] = DefaultConfig::AUGMENT_IMAGE_FLIP;
    json_config["augment_cut_out"] = DefaultConfig::AUGMENT_CUT_OUT;
    json_config["augment_brightness"] = DefaultConfig::AUGMENT_BRIGHTNESS;
    json_config["augment_rgb_shift"] = DefaultConfig::AUGMENT_RGB_SHIFT;

    std::ofstream config_file(file_path);
    config_file << std::setw(4) << json_config << std::endl;
    config_file.close();
}

ConfigReader::Config ConfigReader::get_config()
{
    return config;
}
