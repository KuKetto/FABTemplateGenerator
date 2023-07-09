#ifndef IMAGE_H
#define IMAGE_H

#include <opencv2/opencv.hpp>
#include <QVector>
#include "../utils/zlibcustomimageextractor.h"
#include "cardpositiondata.h"

class Image
{
    std::string file_path;
    cv::Mat image;
    QVector<CardPositionData>* card_positions;

    bool is_template = false;
    bool is_image_open = false;
    void load_card_data(nlohmann::json json_data);
public:
    Image(const bool& is_template = false);
    ~Image();

    void set_file_path(std::string file_path);
    void open();
    void close();
    void show();

    std::string get_file_path() const {return file_path;}
    unsigned int get_card_positions_size() {return card_positions->size();}
    cv::Mat get_opencv_image_object() {return image;}
    void set_opencv_image_object(cv::Mat image) {this->image = image;}
    std::vector<cv::Point2f> get_card_positions_perspective(const int& index);
};

#endif // IMAGE_H
