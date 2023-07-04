#include <QCoreApplication>
#include "containers/image.h"
#include "image-manipulation/imageoverlay.h"
#include "image-manipulation/augmentation.h"

void demo() {
    // First we need to load the template image.
    Image template_image;
    template_image.set_file_path("YOUR TEMPLATE PATH /template/image.zip");
    template_image.open();

    // Set the image as template, we will get the number of required images returned.
    ImageOverlay overlay;
    unsigned short required_image_count = overlay.set_template_image(&template_image);
    qDebug() << required_image_count; // Output: 6

    // Open the images we want to overlay with.
    QVector<Image*> inputs;
    for (int i = 0; i < required_image_count; i++) {
        inputs.push_back(new Image());
        inputs[i]->set_file_path("YOUR INPUT IMAGE PATH /input/input" + std::to_string(i+1) + ".webp");
        inputs[i]->open();
    }

    // To make the dataset flexible we need much data
    // of one singular item (card) and we also need to augment it.
    Augmentation augmentation;
    augmentation.apply_bilateral_blur(inputs[1], 43, 200, 64);

    // Set the inputs and overlay
    overlay.set_images_to_overlay(inputs);
    cv::Mat result = overlay.overlay();

    // Write the output to your destination path
    std::vector<int> compression_params;
    compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
    compression_params.push_back(40); // Adjust the quality value as per your requirements (0-100)

    cv::imwrite("YOUR OUTPUT IMAGE PATH /output/output.jpg", result, compression_params);

    qDebug() << "Finished!";

    // Dynamic memory deallocation - the close method is called inside the destructor
    template_image.close();
    for (auto& input_image : inputs) delete input_image;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    demo();

    return a.exec();
}
