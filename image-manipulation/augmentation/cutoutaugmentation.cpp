#include "cutoutaugmentation.h"

CutOutAugmentation::CutOutAugmentation(Image *image_container, const double &radius):
    radius(radius), Augmentation(image_container)
{

}

void CutOutAugmentation::augment()
{
    Random random;
    std::vector<cv::Point> inpainting_points;
    cv::Mat image = image_container->get_opencv_image_object();

    bool is_triangular_mode = random.generate_integer(0, 1) == 1 ? true : false;
    if (is_triangular_mode) {
        // triangle
        bool is_left_side = random.generate_integer(0, 1) == 1 ? true : false;
        bool is_top_side = random.generate_integer(0, 1) == 1 ? true : false;

        double from_side_percentage = random.generate_double(20, 50) / 100;
        double to_side_percentage = random.generate_double(0, 10) / 100;

        // starting point
        if (is_left_side) inpainting_points.push_back(cv::Point(0, image.rows * from_side_percentage));
        else inpainting_points.push_back(cv::Point(image.cols, image.rows * from_side_percentage));

        // corner
        if (is_left_side) {
            if (is_top_side) inpainting_points.push_back(cv::Point(0, 0));
            else inpainting_points.push_back(cv::Point(0, image.rows));
        } else {
            if (is_top_side) inpainting_points.push_back(cv::Point(image.cols, 0));
            else inpainting_points.push_back(cv::Point(image.cols, image.rows));
        }

        // destination point
        if (is_left_side) {
            if (is_top_side) inpainting_points.push_back(cv::Point((1 - to_side_percentage) * image.cols, 0));
            else inpainting_points.push_back(cv::Point((1 - to_side_percentage) * image.cols, image.rows));
        } else {
            if (is_top_side) inpainting_points.push_back(cv::Point(image.cols * to_side_percentage, 0));
            else inpainting_points.push_back(cv::Point(image.cols * to_side_percentage, image.rows));
        }
    } else {
        // rectangular
        for (int i = 0; i < 4; i++) {
            bool is_left_side = i % 2 == 0;
            bool is_top_side = i < 2;

            double x_pos = random.generate_double(
                is_left_side ? 0 : image.cols / 2,
                is_left_side ? image.cols / 2 : image.cols
                );

            double y_pos = random.generate_double(
                is_top_side ? 0 : image.rows / 2,
                is_top_side ? image.rows / 2 : image.rows
                );

            inpainting_points.push_back(cv::Point(x_pos, y_pos));
        }
    }

    cv::Mat mask = cv::Mat::zeros(image.size(), CV_8UC1);
    cv::fillConvexPoly(mask, inpainting_points, cv::Scalar(255));

    cv::Mat inpainted_image;
    cv::inpaint(image, mask, inpainted_image, radius, cv::INPAINT_TELEA);

    image_container->set_opencv_image_object(inpainted_image);
}
