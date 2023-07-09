#ifndef TEMPLATECONTAINER_H
#define TEMPLATECONTAINER_H

#include "image.h"
#include "../utils/random.h"

class TemplateContainer
{
    QVector<QPair<Image*, unsigned int>> templates;
    unsigned int maximum_required_images = 0;
    unsigned int image_count;

    unsigned int check_number_of_required_images(Image* template_image);
public:
    TemplateContainer(const std::string& directory, const unsigned int& image_count);
    ~TemplateContainer();

    Image* select_a_template();
    void on_image_deleted();
};

#endif // TEMPLATECONTAINER_H
