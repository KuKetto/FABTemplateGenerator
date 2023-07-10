#include "inputdatacontainer.h"

InputDataContainer::InputDataContainer(const std::string& directory, const Config& config)
{   
    for (const auto& entry : std::filesystem::directory_iterator(directory))
    {
        if (std::filesystem::path(entry).extension() == ".jpg"
            || std::filesystem::path(entry).extension() == ".png"
            || std::filesystem::path(entry).extension() == ".webp"
            || std::filesystem::path(entry).extension() == ".bmp") {

            Image* new_image = new Image();
            new_image->set_file_path(entry.path().c_str());

            classes.insert(inputs.size(), entry.path().stem().string());
            InputData input_data(config, inputs.size(), entry.path().filename());

            inputs.push_back(QPair<Image*, InputData>(new_image, input_data));
        }
    }
}

InputDataContainer::~InputDataContainer()
{
    while (!inputs.isEmpty()) {
        inputs.first().first->close();
        delete inputs.first().first;
        inputs.pop_front();
    }
}

QPair<Image *, unsigned int> InputDataContainer::select_an_input()
{
    int random = Random::generate_integer(0, inputs.size() - 1);
    QPair<Image*,InputData> random_input = inputs.at(random);
    unsigned int operation = inputs[random].second.use_this_image();
    if (operation != 65535)
        return QPair<Image*, unsigned int>(random_input.first, operation);

    qDebug("this image cannot be used anymore");
    if (inputs.size() == 1) {
        throw std::invalid_argument("There was only 1 input with 0 usage left but the template required more than that. The generation will stop now.");
    } else {
        qDebug("Trying to brute-force a valid input instead of recursivly applying random");
        for (auto& input : inputs) {
            operation = input.second.use_this_image();
            if (operation != 65535) return QPair<Image*, unsigned int>(input.first, operation);
        }
    }

    throw std::invalid_argument("There were inputs only with 0 usage left but the template required more than that. The generation will stop now.");
}

InputDataContainer::PostImageUsedData InputDataContainer::on_image_used(const QPair<Image *, unsigned int> &image, QVector<QPair<std::string, QPair<cv::Point2f, cv::Point2f>>>& boundaries, int image_width, int image_height)
{
    PostImageUsedData data;
    for (int i = 0; i < inputs.size(); i++) {
        if (inputs.at(i).first->get_file_path() == image.first->get_file_path()) {
            data.class_id = inputs[i].second.get_class_id();
            QPair<cv::Point2f, cv::Point2f> boundary;
            int index = 0;
            for (auto& _boundary : boundaries) {
                if (_boundary.first == inputs.at(i).first->get_file_path()) {
                    boundary = _boundary.second;
                    break;
                }
                index++;
            }
            boundaries.removeAt(index);

            QPair<QPair<double, double>, QPair<double, double> > normalized_boundaries = get_image_normalized_boundaries(
                boundary, image_width, image_height
            );

            data.image_center = normalized_boundaries.first;
            data.boundary_size = normalized_boundaries.second;

            if (inputs[i].second.image_used(image.second)) {
                data.should_image_be_removed = true;
                return data;
            }
            data.should_image_be_removed = false;
            return data;
        }
    }

    data.should_image_be_removed = false;
    return data;
}

void InputDataContainer::clean_up_finished_image(const QPair<Image *, unsigned int> &image)
{
    for (int i = 0; i < inputs.size(); i++) {
        if (inputs.at(i).first->get_file_path() == image.first->get_file_path()) {
            inputs[i].first->close();
            delete inputs[i].first;
            inputs.removeAt(i);
            return;
        }
    }
}

double InputDataContainer::normalize_value(double value, double normalizing_factor)
{
    return value / normalizing_factor;
}

QPair<QPair<double, double>, QPair<double, double> > InputDataContainer::get_image_normalized_boundaries(QPair<cv::Point2f, cv::Point2f> boundaries, int image_width, int image_height)
{
    QPair<double, double> boundary_size;
    boundary_size.first = normalize_value((boundaries.second.x - boundaries.first.x) / 2, image_width);
    boundary_size.second = normalize_value((boundaries.second.y - boundaries.first.y) / 2, image_height);

    QPair<double, double> image_center;
    image_center.first = normalize_value((boundaries.first.x + boundaries.second.x) / 2, image_width);
    image_center.second = normalize_value((boundaries.first.y + boundaries.second.y) / 2, image_height);

    return QPair<QPair<double, double>, QPair<double, double>>(image_center, boundary_size);
}
