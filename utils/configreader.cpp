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

        config.set_image_count(json_config["image_count"]);
        config.set_augment_noise(json_config["augment_noise"]);
        config.set_augment_lens_blur(json_config["augment_lens_blur"]);
        config.set_augment_bilateral_blur(json_config["augment_bilateral_blur"]);
        config.set_augment_image_flip(json_config["augment_image_flip"]);
        config.set_augment_cut_out(json_config["augment_cut_out"]);
        config.set_augment_brightness(json_config["augment_brightness"]);
        config.set_augment_rgb_shift(json_config["augment_rgb_shift"]);
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

            config.set_image_count(DefaultConfig::IMAGE_COUNT);
            config.set_augment_noise(DefaultConfig::AUGMENT_NOISE);
            config.set_augment_lens_blur(DefaultConfig::AUGMENT_LENS_BLUR);
            config.set_augment_bilateral_blur(DefaultConfig::AUGMENT_BILATERAL_BLUR);
            config.set_augment_image_flip(DefaultConfig::AUGMENT_IMAGE_FLIP);
            config.set_augment_cut_out(DefaultConfig::AUGMENT_CUT_OUT);
            config.set_augment_brightness(DefaultConfig::AUGMENT_BRIGHTNESS);
            config.set_augment_rgb_shift(DefaultConfig::AUGMENT_RGB_SHIFT);

            write_default_config();
        } else {
            qDebug() << "Exception: " << e.what();
            return false;
        }
    }

    config.set_augment_count(config.get_augment_noise() + config.get_augment_lens_blur() + config.get_augment_bilateral_blur()
                             + config.get_augment_image_flip() + config.get_augment_cut_out() + config.get_augment_brightness()
                             + config.get_augment_rgb_shift());

    if (config.get_augment_count() > config.get_image_count()) {
        qDebug() << "You want to create more augments than images (augment count > image_count). Please check your config file";
        return false;
    }
    config.set_normal_image_count(config.get_image_count() - config.get_augment_count());
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

Config ConfigReader::get_config()
{
    return config;
}
