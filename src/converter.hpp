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
        void convert();
        FileType read_file_ext(fs::path& file_path);

        bool csv_remove_duplicate_records();
        void csv_display_contents(fs::path& file_path);

        fs::path& get_input_file_path() { return input_path; }
        fs::path& get_output_file_path() { return output_path; }

    private:
        bool json_to_csv();
        bool csv_to_json();
        FileType convert_option();

        fs::path input_path;
        fs::path output_path;
};
