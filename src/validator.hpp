#pragma once

#include <filesystem>
#include <functional>
#include <string>

#include <jsoncons/json.hpp>

namespace fs = std::filesystem;

class Validator {
  public:
    Validator()  = default;
    ~Validator() = default;

    void set_logger(std::function<void(const std::string&)> logger) {
        logger_ = std::move(logger);
    }

    bool                  json_flatten(const fs::path& input_path);

    static jsoncons::json flatten_json(const jsoncons::json& document);
    static jsoncons::json prepare_for_csv(const jsoncons::json& document);

  private:
    static void                             flatten_value(const jsoncons::json& value, const std::string& prefix, jsoncons::json& result);
    void                                    log(const std::string& message) const;

    std::function<void(const std::string&)> logger_;
};
