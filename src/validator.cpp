#include "validator.hpp"

#include <charconv>
#include <fstream>
#include <map>
#include <string_view>

#include <jsoncons/json.hpp>

using jsoncons::json;

namespace {
    bool normalize_legacy_flattened_object(const json& value, json& result) {
        if (!value.is_object() || value.empty())
            return false;

        std::map<std::size_t, json> records;
        for (const auto& member : value.object_range()) {
            const std::string key             = std::string(member.key());
            const std::size_t closing_bracket = key.find(']');
            if (key.empty() || key.front() != '[' || closing_bracket == std::string::npos || closing_bracket + 1 >= key.size() || key[closing_bracket + 1] != '.' ||
                member.value().is_object() || member.value().is_array())
                return false;

            const std::string_view index_text(key.data() + 1, closing_bracket - 1);
            std::size_t            record_index = 0;
            const auto [end, error]             = std::from_chars(index_text.data(), index_text.data() + index_text.size(), record_index);
            if (error != std::errc{} || end != index_text.data() + index_text.size())
                return false;

            records[record_index][key.substr(closing_bracket + 2)] = member.value();
        }

        result = json::array();
        result.reserve(records.size());
        for (auto& entry : records)
            result.push_back(std::move(entry.second));
        return true;
    }
}

void Validator::flatten_value(const json& value, const std::string& prefix, json& result) {
    if (value.is_object()) {
        for (const auto& member : value.object_range()) {
            const std::string key = prefix.empty() ? std::string(member.key()) : prefix + "." + std::string(member.key());
            flatten_value(member.value(), key, result);
        }
    } else if (value.is_array()) {
        for (std::size_t i = 0; i < value.size(); ++i)
            flatten_value(value[i], prefix + "[" + std::to_string(i) + "]", result);
    } else {
        result[prefix] = value;
    }
}

json Validator::flatten_json(const json& document) {
    json result;
    flatten_value(document, "", result);
    return result;
}

json Validator::prepare_for_csv(const json& document) {
    // Flattened files use root-array keys such as "[0].name". Normalize that
    // representation back into records before passing it to the CSV encoder.
    json legacy_result;
    if (normalize_legacy_flattened_object(document, legacy_result))
        return legacy_result;
    if (document.is_array() && document.size() == 1 && normalize_legacy_flattened_object(document[0], legacy_result))
        return legacy_result;

    json result = json::array();
    if (document.is_array()) {
        result.reserve(document.size());
        for (const auto& record : document.array_range()) {
            json flattened_record;
            flatten_value(record, "", flattened_record);
            result.push_back(std::move(flattened_record));
        }
    } else {
        json flattened_record;
        flatten_value(document, "", flattened_record);
        result.push_back(std::move(flattened_record));
    }
    return result;
}

bool Validator::json_flatten(const fs::path& input_path) {
    std::ifstream input(input_path);
    if (!input) {
        log(std::format("An error occurred while reading {}.", input_path.string()));
        return false;
    }
    json j = json::parse(input);

    // Flatten the complete document, including its root array indexes.
    const json result = flatten_json(j);
    if (result == j) {
        log(std::format("{} is already flattened.", input_path.string()));
        return true;
    }

    std::ofstream output(input_path);
    if (!output) {
        log(std::format("An error occurred while opening {}.", input_path.string()));
        return false;
    }
    result.dump(output, jsoncons::indenting::indent);

    log(std::format("Successfully flattened {}.", input_path.string()));
    return true;
}

void Validator::log(const std::string& message) const {
    if (logger_)
        logger_(message);
}
