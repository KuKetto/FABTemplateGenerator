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
    QPair<Image*,InputData> random_input = inputs.at(Random::generate_integer(0, inputs.size() - 1));
    return QPair<Image*, unsigned int>(random_input.first, random_input.second.use_this_image());
}

bool InputDataContainer::on_image_used(const QPair<Image *, unsigned int> &image)
{
    for (int i = 0; i < inputs.size(); i++) {
        if (inputs.at(i).first->get_file_path() == image.first->get_file_path()) {
            if (inputs[i].second.image_used(image.second)) {
                inputs[i].first->close();
                delete inputs[i].first;
                inputs.removeAt(i);
                return true;
            }
            return false;
        }
    }

    return false;
}
