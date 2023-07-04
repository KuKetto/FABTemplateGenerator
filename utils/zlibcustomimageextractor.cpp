#include "zlibcustomimageextractor.h"

ZLibCustomImageExtractor::ZLibCustomImageExtractor(std::string file_path)
{
    this->open(file_path);
}

ZLibCustomImageExtractor::UncompressedBinaryDataFiles ZLibCustomImageExtractor::getBinaryDataFiles()
{
    return files;
}

bool ZLibCustomImageExtractor::extract(zip* _zip_file)
{
    // Extract the image file
    zip_file* image_file = zip_fopen(_zip_file, "image.jpg", 0);
    if (!image_file) {
        zip_close(_zip_file);
        return false;
    }

    // Get the size of the image file
    zip_stat_t image_stats;
    zip_stat(_zip_file, "image.jpg", 0, &image_stats);

    // Read the image file into memory
    char* image_data = new char[image_stats.size];
    zip_fread(image_file, image_data, image_stats.size);
    zip_fclose(image_file);

    // Create an OpenCV Mat object from the image data
    files.image_data = cv::imdecode(cv::Mat(1, image_stats.size, CV_8UC1, image_data), cv::IMREAD_COLOR);
    delete[] image_data;

    // Extract the JSON file
    zip_file* json_file = zip_fopen(_zip_file, "image_data.json", 0);
    if (!json_file) {
        zip_close(_zip_file);
        return false;
    }

    // Get the size of the text file
    zip_stat_t json_stats;
    zip_stat(_zip_file, "image_data.json", 0, &json_stats);

    // Read the text file into memory
    char* json_data = new char[json_stats.size + 1]; // Increase size by 1 for null terminator
    zip_fread(json_file, json_data, json_stats.size);
    zip_fclose(json_file);

    // Null-terminate the string
    json_data[json_stats.size] = '\0';

    // Remove trailing whitespace or line-ending characters
    int i = json_stats.size - 1;
    while (i >= 0 && (json_data[i] == ' ' || json_data[i] == '\r' || json_data[i] == '\n')) {
        json_data[i] = '\0';
        i--;
    }

    // Create a nlohmann json object from the text data
    try {
        files.json_data = nlohmann::json::parse(json_data);
    } catch (const nlohmann::json::parse_error& e) {
        // Handle JSON parsing error
        qDebug() << "Failed to parse JSON: " << e.what();
        delete[] json_data;
        zip_close(_zip_file);
        return false;
    }

    delete[] json_data;
    zip_close(_zip_file);

    return true;
}

void ZLibCustomImageExtractor::open(std::string file_path)
{
    // Open the zip file
    int error;
    zip* _zip_file = zip_open(file_path.c_str(), 0, &error);
    if (!_zip_file) {
        // Handle error
        qDebug() << "Failed to open the zip file, error code: " << error;
        return;
    }

    // extract the files
    if (!this->extract(_zip_file)) {
        qDebug() << "Failed to extract the zip file.";
        return;
    }
}
