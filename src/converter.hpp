#pragma once

#include <filesystem>

namespace fs = std::filesystem;

enum class FileType {
    NONE = 0,

    CSV,
    JSON
};

class Converter {
    public:
        void prompt_menu();

        fs::path get_file_path() { return file_path; }
        void set_file_path(fs::path& file_path) { this->file_path = file_path; }

    private:
        void convert();
        bool json_to_csv();
        bool csv_to_json();
        FileType read_input_file_ext();
        FileType convert_option();

        fs::path file_path;
};
