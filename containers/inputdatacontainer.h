#ifndef INPUTDATACONTAINER_H
#define INPUTDATACONTAINER_H

#include "inputdata.h"
#include "image.h"
#include "config.h"

class InputDataContainer
{
    QVector<QPair<Image*,InputData>> inputs;
public:
    InputDataContainer(const std::string& directory, const Config& config);
    ~InputDataContainer();

    unsigned int get_image_count() const {return inputs.size();}
    QPair<Image*, unsigned int> select_an_input();
    bool on_image_used(const QPair<Image*, unsigned int>& image);
};

#endif // INPUTDATACONTAINER_H
