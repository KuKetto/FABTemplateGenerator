#include "templatecontainer.h"

TemplateContainer::TemplateContainer(const std::string &directory, const unsigned int &image_count):
    image_count(image_count)
{
    if (!std::filesystem::exists(directory) || !std::filesystem::is_directory(directory))
        throw PathNotFoundException("InputDataContainer::InputDataContainer", directory);
    for (const auto& entry : std::filesystem::directory_iterator(directory))
    {
        if (std::filesystem::path(entry).extension() == ".zip") {
            Image* new_template = new Image(true);
            new_template->set_file_path(entry.path().c_str());
            try {
                unsigned int required_images = check_number_of_required_images(new_template);

                if (required_images <= image_count) {
                    templates.push_back(QPair<Image*, unsigned int>(new_template, required_images));
                    if (required_images > maximum_required_images) maximum_required_images = required_images;
                } else delete new_template;

            } catch (InvalidTemplateException& ex) {
                qDebug() << ex.what();
                delete new_template;
            }
        }
    }
}

TemplateContainer::~TemplateContainer()
{
    while (!templates.isEmpty()) {
        templates.first().first->close();
        delete templates.first().first;
        templates.pop_front();
    }
}

unsigned int TemplateContainer::check_number_of_required_images(Image *template_image)
{
    try {
        template_image->open();
    } catch (std::exception& ex) {
        throw InvalidTemplateException("TemplateContainer::check_number_of_required_images", ex.what());
    }
    unsigned int required_size = template_image->get_card_positions_size();
    template_image->close();

    return required_size;
}

Image *TemplateContainer::select_a_template()
{
    if (templates.empty()) throw NoValidInputsLeftException("TemplateContainer::select_a_template", "");
    return templates.at(Random::generate_integer(0, templates.size() - 1)).first;
}

void TemplateContainer::on_image_deleted()
{
    if (templates.empty()) throw BadUsageException("TemplateContainer::on_image_deleted",
                                "There were no templates left meaning all images should "
                                "have been finished and the generation should have been stopped. "
                                "Or there was no template used which only requires the minimum "
                                "amount of input images (1).");
    image_count--;
    if (maximum_required_images <= image_count) return;

    unsigned new_maximum = 0;

    QList<int> indexes_to_remove;
    int index = 0;
    for (auto& _template : templates) {
        if (_template.second > image_count) {
            _template.first->close();
            delete _template.first;
            indexes_to_remove.append(index);
        } else if (_template.second > new_maximum) new_maximum = _template.second;
        index++;
    }

    maximum_required_images = new_maximum;

    for (int i = indexes_to_remove.size() - 1; i >= 0; i--) {
        templates.removeAt(indexes_to_remove.at(i));
    }
}
