#pragma once

#include <filesystem>

#include <jsoncons/json.hpp>

namespace fs = std::filesystem;

class Validator {
    public:
        bool json_flatten(fs::path& input_path);

    private:
        void flatten_helper(const jsoncons::json& j, const std::string& prefix, jsoncons::json& result);
};
