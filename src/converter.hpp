#pragma once

#include <filesystem>

#include <jsoncons/json.hpp>

#include "validator.hpp"

namespace fs = std::filesystem;

enum class FileType {
    NONE = 0,

    CSV,
    JSON
};

class Converter {
  public:
    Converter(Validator& validator) : validator_(validator) {}
    ~Converter() = default;

    void     convert();
    FileType read_file_ext(const fs::path& file_path);
    bool     csv_remove_duplicate_records();
    bool     csv_trim_whitespace();
    void     display_file_contents(const fs::path& file_path);

    void     set_logger(std::function<void(const std::string&)> logger) {
        logger_ = std::move(logger);
    }

    fs::path& get_input_file_path() {
        return input_path_;
    }
    fs::path& get_output_file_path() {
        return output_path_;
    }

    Validator& get_validator() {
        return validator_;
    }

  private:
    bool                                    json_to_csv();
    bool                                    csv_to_json();
    FileType                                convert_option();
    bool                                    csv_validate_header(const fs::path& file_path);
    void                                    log(const std::string& message);

    fs::path                                input_path_;
    fs::path                                output_path_;
    Validator                               validator_;
    std::function<void(const std::string&)> logger_;
};
