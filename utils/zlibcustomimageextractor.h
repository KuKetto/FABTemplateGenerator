#ifndef ZLIBEXTRACT_H
#define ZLIBEXTRACT_H

#include <QDebug>
#include <zip.h>
#include <fstream>
#include <filesystem>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include "../nlohmann-json/json.hpp"

class ZLibCustomImageExtractor
{
public:
    struct UncompressedBinaryDataFiles {
        cv::Mat image_data;
        nlohmann::json json_data;
    };
private:
    std::vector<unsigned char> compressed_data;
    UncompressedBinaryDataFiles files;

    void open(std::string file_path);
    bool extract(zip* zip_file);
public:
    ZLibCustomImageExtractor(std::string file_path);
    UncompressedBinaryDataFiles getBinaryDataFiles();
};

#endif // ZLIBEXTRACT_H
