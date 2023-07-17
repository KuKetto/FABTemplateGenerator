#ifndef GENERATION_H
#define GENERATION_H

#include "containers/inputdatacontainer.h"
#include "containers/templatecontainer.h"
#include "containers/config.h"
#include "image-manipulation/imageoverlay.h"
#include "utils/configreader.h"
#include "utils/augmentationfactory.h"
#include <chrono>

class Generation
{
    InputDataContainer* input_data_container;
    TemplateContainer* template_container;
    std::string output_path;

    unsigned long long MAXIMUM_NUMBER_OF_ITERATIONS;
public:
    Generation(const std::string& input_directory, const std::string& template_directory, const std::string& config_path, const std::string& output_path, const std::string& class_file_output_path);
    ~Generation();

    void generate();
};

#endif // GENERATION_H
