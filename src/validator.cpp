#include <fstream>
#include <print>

#include <jsoncons/json.hpp>

#include "validator.hpp"

using jsoncons::json;

void Validator::flatten_helper(const json& j, const std::string& prefix, json& result) {
    if (j.is_object()) {
      for (const auto& member : j.object_range()) {
          std::string key = prefix.empty() ? std::string(member.key())
                                           : prefix + "." + std::string(member.key());
          flatten_helper(member.value(), key, result);
      }
    } else if (j.is_array()) {
      for (std::size_t i = 0; i < j.size(); ++i) {
          flatten_helper(j[i], prefix + "[" + std::to_string(i) + "]", result);
      }
    } else {
      result[prefix] = j;
    }
}

bool Validator::json_flatten(fs::path& input_path) {
    // FIX: Flattening works; however, if we try to convert a flattened JSON file
    // to CSV, it does not account for the fact that it is flattened and is
    // formatted incorrectly. Add a way to verify if a file has been already 
    // flattened, and then be able to either revert it before converting, or be 
    // able to convert with it being flattened.
    std::ifstream input(input_path);
    if (!input) {
        std::println("An error occurred while reading {}.\n", input_path.string());
        return false;
    }
    json j = json::parse(input);

    json result;
    flatten_helper(j, "", result);

    std::ofstream output(input_path);
    if (!output) {
        std::println("An error occurred while opening {}.\n", input_path.string());
        return false;
    }
    result.dump(output, jsoncons::indenting::indent);

    std::println("Successfully flattened {}.\n", input_path.string());
    return true;
}
