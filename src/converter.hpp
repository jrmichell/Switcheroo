#pragma once

#include <filesystem>
#include <functional>
#include <string>

namespace fs = std::filesystem;

enum class FileType {
    None,
    Csv,
    Json
};

class Converter {
  public:
    Converter()  = default;
    ~Converter() = default;

    bool     convert();
    FileType read_file_ext(const fs::path& file_path) const;
    bool     csv_remove_duplicate_records();
    bool     csv_trim_whitespace();
    void     display_file_contents(const fs::path& file_path);

    void     set_logger(std::function<void(const std::string&)> logger) {
        logger_ = std::move(logger);
    }

    void set_input_file_path(fs::path path) {
        input_path_ = std::move(path);
    }

    const fs::path& get_input_file_path() const {
        return input_path_;
    }

    const fs::path& get_output_file_path() const {
        return output_path_;
    }

  private:
    bool                                    json_to_csv();
    bool                                    csv_to_json();
    bool                                    csv_validate_header(const fs::path& file_path);
    void                                    log(const std::string& message) const;

    fs::path                                input_path_;
    fs::path                                output_path_;
    std::function<void(const std::string&)> logger_;
};
