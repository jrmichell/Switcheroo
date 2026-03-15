#pragma once

#include <functional>
#include <string>
#include <filesystem>

#include <jsoncons/json.hpp>

namespace fs = std::filesystem;

class Validator {
  public:
    Validator()  = default;
    ~Validator() = default;

    void set_logger(std::function<void(const std::string&)> logger) {
        logger_ = std::move(logger);
    }

    bool json_flatten(fs::path& input_path);

  private:
    void                                    flatten_helper(const jsoncons::json& j, const std::string& prefix, jsoncons::json& result);
    void                                    log(const std::string& message);

    std::function<void(const std::string&)> logger_;
};
