#pragma once

#include <filesystem>
#include <jsoncons/json.hpp>

namespace fs = std::filesystem;

enum class FileType {
    NONE = 0,

    CSV,
    JSON
};

class Converter {
    public:
        void convert();
        FileType read_file_ext(const fs::path& file_path);

        bool csv_remove_duplicate_records();
        bool csv_trim_whitespace();
        void csv_display_contents(const fs::path& file_path);

        bool json_flatten();

        fs::path& get_input_file_path() { return input_path; }
        fs::path& get_output_file_path() { return output_path; }

    private:
        bool json_to_csv();
        bool csv_to_json();
        FileType convert_option();
        void flatten_helper(const jsoncons::json& j, const std::string& prefix, jsoncons::json& result);
        bool csv_validate_header(const fs::path& file_path);

        fs::path input_path;
        fs::path output_path;
};
