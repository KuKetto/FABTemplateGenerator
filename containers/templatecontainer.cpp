#include "templatecontainer.h"

TemplateContainer::TemplateContainer(const std::string &directory, const unsigned int &image_count):
    image_count(image_count)
{
    for (const auto& entry : std::filesystem::directory_iterator(directory))
    {
        if (std::filesystem::path(entry).extension() == ".zip") {
            Image* new_template = new Image(true);
            new_template->set_file_path(entry.path().c_str());
            unsigned int required_images = check_number_of_required_images(new_template);

            if (required_images <= image_count) {
                templates.push_back
                    (QPair<Image*, unsigned int>(new_template, required_images)
                );
                if (required_images > maximum_required_images) maximum_required_images = required_images;
            } else delete new_template;
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
    template_image->open();
    unsigned int required_size = template_image->get_card_positions_size();
    template_image->close();

    return required_size;
}

Image *TemplateContainer::select_a_template()
{
    return templates.at(Random::generate_integer(0, templates.size() - 1)).first;
}

void TemplateContainer::on_image_deleted()
{
    image_count--;
    if (maximum_required_images <= image_count) return;

    unsigned new_maximum = 0;

    QList<int> indexes_to_remove;
    int index = 0;
    for (auto& _template : templates) {
        if (_template.second == maximum_required_images) {
            _template.first->close();
            delete _template.first;
            indexes_to_remove.append(index);
        } else if (_template.second > new_maximum) new_maximum = _template.second;
        index++;
    }

    for (int i = indexes_to_remove.size() - 1; i >= 0; i--) {
        templates.removeAt(indexes_to_remove.at(i));
    }
}
