#include "generation.h"

Generation::Generation(const std::string &input_directory, const std::string &template_directory, const std::string &config_path, const std::string &output_path, const std::string& class_file_output_path):
    output_path(output_path)
{
    ConfigReader config_reader(config_path);
    if (!config_reader.read_config()) throw std::invalid_argument("Bad config path");

    input_data_container = new InputDataContainer(input_directory, config_reader.get_config());
    template_container = new TemplateContainer(template_directory, input_data_container->get_image_count());

    MAXIMUM_NUMBER_OF_ITERATIONS = input_data_container->get_image_count() * config_reader.get_config().get_image_count();

    QMap<unsigned, std::string> classes = input_data_container->get_classes();
    std::ofstream image_annotations(class_file_output_path + "/classes.txt");
    for (int i = 0; i < classes.size(); i++) {
        image_annotations << classes[i];
        if (i < classes.size() - 1) image_annotations << "\n";
    }
    image_annotations.close();
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
    //MAXIMUM_NUMBER_OF_ITERATIONS = 20;
    while (input_data_container->get_image_count() > 0 && iterator <= MAXIMUM_NUMBER_OF_ITERATIONS) {
        std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
        Image* template_image = template_container->select_a_template();
        template_image->open();

        Augmentation augmentation;
        ImageOverlay overlay;
        QVector<Image*> overlay_inputs;
        QVector<QPair<Image *, unsigned int>> selected_inputs;
        for (int i = 0; i < overlay.set_template_image(template_image); i++) {
            QPair<Image *, unsigned int> selected_input = input_data_container->select_an_input();

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

        overlay.set_images_to_overlay(overlay_inputs);
        ImageOverlay::OverlayedResult result = overlay.overlay();

        cv::Mat compressed_image;
        cv::resize(result.overlayed_image, compressed_image, cv::Size(1280, 720));

        cv::imwrite(output_path + "/images/train/output_" + std::to_string(iterator) + ".jpg", compressed_image);
        std::ofstream image_annotations(output_path + "/labels/train/output_" + std::to_string(iterator) + ".txt");
        QVector<int> images_to_be_removed;
        for (int i = 0; i < selected_inputs.size(); i++) {
            overlay_inputs[i]->close();
            delete overlay_inputs[i];
            InputDataContainer::PostImageUsedData post_image_used_data = input_data_container->on_image_used(selected_inputs[i], result.input_overlay_positions, result.image_width, result.image_height);
            if (post_image_used_data.should_image_be_removed) {

                bool is_image_already_scheduled_for_deletion = false;
                for (int j = 0; j < images_to_be_removed.size(); j++)
                    if (selected_inputs[i].first->get_file_path() == selected_inputs[j].first->get_file_path()) {
                        is_image_already_scheduled_for_deletion = true;
                        break;
                    }
                if (!is_image_already_scheduled_for_deletion) {
                    images_to_be_removed.push_back(i);
                }
            }

            image_annotations << post_image_used_data.class_id << " "
                              << post_image_used_data.image_center.first
                              << " " << post_image_used_data.image_center.second
                              << " " << post_image_used_data.boundary_size.first
                              << " " << post_image_used_data.boundary_size.second;
            if (i < selected_inputs.size() - 1) image_annotations << "\n";
        }
        image_annotations.close();

        for (int j = 0; j < images_to_be_removed.size(); j++) {
            input_data_container->clean_up_finished_image(selected_inputs[j]);
            template_container->on_image_deleted();
        }

        template_image->close();
        iterator++;
        std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
        std::chrono::duration<double> duration = end - start;
        qDebug() << "iteration " << iterator - 1 << " finished in " << duration.count() << "seconds\n\t" << input_data_container->get_image_count() << " image left\n---";
    }
}
