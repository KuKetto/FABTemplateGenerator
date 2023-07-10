#ifndef INPUTDATACONTAINER_H
#define INPUTDATACONTAINER_H

#include "inputdata.h"
#include "image.h"
#include "config.h"

class InputDataContainer
{
    QVector<QPair<Image*,InputData>> inputs;
    QMap<unsigned, std::string> classes;

    double normalize_value(double value, double normalizing_factor);
    QPair<QPair<double, double>, QPair<double, double>> get_image_normalized_boundaries(QPair<cv::Point2f, cv::Point2f> boundaries, int image_width, int image_height);
public:
    struct PostImageUsedData {
        bool should_image_be_removed;
        unsigned class_id;
        QPair<double, double> boundary_size;
        QPair<double, double> image_center;
    };

    InputDataContainer(const std::string& directory, const Config& config);
    ~InputDataContainer();

    QMap<unsigned, std::string> get_classes() const {return classes;}
    unsigned int get_image_count() const {return inputs.size();}
    QPair<Image*, unsigned int> select_an_input();
    PostImageUsedData on_image_used(const QPair<Image*, unsigned int>& image, QVector<QPair<std::string, QPair<cv::Point2f, cv::Point2f>>>& boundaries, int image_width, int image_height);
    void clean_up_finished_image(const QPair<Image*, unsigned int>& image);
};

#endif // INPUTDATACONTAINER_H
