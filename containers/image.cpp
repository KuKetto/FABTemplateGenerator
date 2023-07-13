#include "image.h"

Image::Image(const bool &is_template):
    is_template(is_template)
{
    if (is_template) card_positions = new QVector<CardPositionData>();
}

Image::~Image()
{
    if (is_image_open) close();
    else {
        if (is_template) delete card_positions;
    }
}

void Image::set_file_path(const std::string &file_path)
{
    this->file_path = file_path;
}

void Image::open()
{
    if (file_path == std::string()) {
        throw FileNotFound("Image::open", "You haven't provided any file path. Use the Image::set_file_path before calling open.");
    }
    if (std::filesystem::path(file_path).extension() == ".zip" && !is_template) {
        throw BadUsageException("Image::open",
                                "Constructor was set as an image (not as an template) but a template (zip) file path was provided. This could have caused a SEGFAULT",
                                file_path,
                                "ZIP Archive",
                                "Image (JPG / PNG / WEBP / etc...)");
    }

    if (std::filesystem::path(file_path).extension() == ".zip") {
        ZLibCustomImageExtractor extractor(file_path);
        ZLibCustomImageExtractor::UncompressedBinaryDataFiles files = extractor.getBinaryDataFiles();
        load_card_data(files.json_data);
        image = files.image_data;

    } else {
        try {
            image = cv::imread(file_path, cv::IMREAD_COLOR);
            cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
        }
        catch (const cv::Exception& e) {
            throw LibraryException("Image::open", "OpenCV", e.what());
        }
        catch (const std::exception& e) {
            // Handle std::exception (OpenCV throws it's own and std exceptions as well.)
            throw LibraryException("Image::open", "OpenCV", e.what());
        }
    }

    is_image_open = true;
}

void Image::close(const bool& destruct)
{
    if (is_image_open) {
        if (is_template) {
            delete card_positions;
            if (!destruct) card_positions = new QVector<CardPositionData>();
        }
        image = cv::Mat_<unsigned char>(0,0);
        is_image_open = false;
    }
}

void Image::show() const
{
    cv::imshow("input", image);
    cv::waitKey(0);
}

void Image::load_card_data(nlohmann::json json_data)
{
    for (auto& template_position : json_data) {
        CardPositionData card_data;

        card_data.set_top_left_pos(std::pair<unsigned int, unsigned int>
            (template_position["top_left"]["x"], template_position["top_left"]["y"]));
        card_data.set_top_right_pos(std::pair<unsigned int, unsigned int>
            (template_position["top_right"]["x"], template_position["top_right"]["y"]));
        card_data.set_bottom_right_pos(std::pair<unsigned int, unsigned int>
            (template_position["bottom_right"]["x"], template_position["bottom_right"]["y"]));
        card_data.set_bottom_left_pos(std::pair<unsigned int, unsigned int>
            (template_position["bottom_left"]["x"], template_position["bottom_left"]["y"]));

        card_positions->push_back(card_data);
    }
}

std::vector<cv::Point2f> Image::get_card_positions_perspective(const int &index) const
{
    return this->card_positions->value(index).get_perspective_positions();
}
