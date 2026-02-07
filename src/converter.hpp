#pragma once

#include <string>

enum class FileType {
    NONE,

    CSV,
    JSON
};

class Converter {
    public:
        void convert(std::string& input_file_path);

    private:
        FileType read_input_file_ext(std::string& input_file_path);
        bool json_to_csv(std::string& input_file_path);
        bool csv_to_json(std::string& input_file_path);
};
