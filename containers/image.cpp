#include "image.h"

Image::Image()
{
    card_positions = new QVector<CardPositionData>();
}

Image::~Image()
{
    if (is_image_open) close();
}

void Image::set_file_path(std::string file_path)
{
    this->file_path = file_path;
}

void Image::open()
{
    if (file_path == std::string()) {
        qDebug() << "Can't open the file: You haven't set a file path for the Image class.";
        return;
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
        } catch (...) {
            qDebug() << "OpenCV image input read exception";
            return;
        }
    }

    is_image_open = true;
}

void Image::close()
{
    delete card_positions;
    is_image_open = false;
}

void Image::show()
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

std::vector<cv::Point2f> Image::get_card_positions_perspective(const int &index)
{
    return this->card_positions->value(index).get_perspective_positions();
}
