#include "generation.h"

Generation::Generation(const std::string &input_directory, const std::string &template_directory, const std::string &config_path, const std::string &output_path):
    output_path(output_path)
{
    ConfigReader config_reader(config_path);
    if (!config_reader.read_config()) throw std::invalid_argument("Bad config path");

    input_data_container = new InputDataContainer(input_directory, config_reader.get_config());
    template_container = new TemplateContainer(template_directory, input_data_container->get_image_count());

    MAXIMUM_NUMBER_OF_ITERATIONS = input_data_container->get_image_count() * config_reader.get_config().get_image_count() + 1;
}

Generation::~Generation()
{
    delete input_data_container;
    delete template_container;
}

void Generation::generate()
{
    int iterator = 0;
    qDebug() << "maximum iteration count: " << MAXIMUM_NUMBER_OF_ITERATIONS;
    MAXIMUM_NUMBER_OF_ITERATIONS = 10;
    while (input_data_container->get_image_count() > 0 && iterator <= MAXIMUM_NUMBER_OF_ITERATIONS) {
        qDebug() << "iteration: " << iterator;
        Image* template_image = template_container->select_a_template();
        qDebug() << "\ttemplate selected";
        template_image->open();

        Augmentation augmentation;
        ImageOverlay overlay;
        QVector<Image*> overlay_inputs;
        QVector<QPair<Image *, unsigned int>> selected_inputs;
        for (int i = 0; i < overlay.set_template_image(template_image); i++) {
            QPair<Image *, unsigned int> selected_input = input_data_container->select_an_input();
            qDebug() << "\tan input was selected";

            // we need copies because augmentation overwrites the image
            Image* overlay_input = new Image();
            overlay_input->set_file_path(selected_input.first->get_file_path());
            overlay_input->open();

            switch (selected_input.second) {
            case OPERATION_TYPE::NOISE:
                augmentation.apply_noise(overlay_input, Random::generate_double(30, 45));
                break;
            case OPERATION_TYPE::LENS_BLUR:
                augmentation.apply_lens_blur(overlay_input, Random::generate_double(1, 3));
                break;
            case OPERATION_TYPE::BILATERAL_BLUR:
                augmentation.apply_bilateral_blur(overlay_input, Random::generate_integer(15, 50), Random::generate_integer(80, 220), Random::generate_integer(35, 70));
                break;
            case OPERATION_TYPE::IMAGE_FLIP:
                augmentation.flip_image(overlay_input);
                break;
            case OPERATION_TYPE::CUT_OUT:
                augmentation.cut_out_image(overlay_input, Random::generate_integer(4, 12));
                break;
            case OPERATION_TYPE::BRIGHTNESS_ADJUSTION:
                augmentation.adjust_brightness(overlay_input, Random::generate_double(0.75, 1.25));
                break;
            case OPERATION_TYPE::RGB_SHIFT:
                augmentation.shift_rgb(overlay_input, Random::generate_double(0.9, 1.1), Random::generate_double(0.9, 1.1), Random::generate_double(0.9, 1.1));
                break;
            default:
                break;
            }

            selected_inputs.push_back(selected_input);
            overlay_inputs.push_back(overlay_input);
        }
        qDebug() << "\tinput selection finished";

        overlay.set_images_to_overlay(overlay_inputs);
        cv::Mat result = overlay.overlay();
        qDebug() << "\toverlay finished";

        std::vector<int> compression_params;
        compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
        compression_params.push_back(40);
        qDebug() << "\tresult has been written";

        cv::imwrite(output_path + "/output_" + std::to_string(iterator) + ".jpg", result, compression_params);
        for (int i = 0; i < selected_inputs.size(); i++) {
            overlay_inputs[i]->close();
            delete overlay_inputs[i];

            if (input_data_container->on_image_used(selected_inputs[i])) template_container->on_image_deleted();
        }
        qDebug() << "\tinput copies has been cleaned up";

        template_image->close();
        iterator++;
        qDebug() << "iteration: " << iterator - 1 << " finished";
    }
}
