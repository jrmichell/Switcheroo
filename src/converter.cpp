#include <fstream>
#include <jsoncons_ext/csv/csv.hpp>
#include <print>

#include "converter.hpp"

using jsoncons::json;

namespace csv = jsoncons::csv;
namespace fs = std::filesystem;

void Converter::convert() {
    FileType input_file_type = read_file_ext(input_path);

    switch (input_file_type) {
    case FileType::CSV:
        csv_to_json();
        break;
    case FileType::JSON:
        json_to_csv();
        break;
    case FileType::NONE:
        std::println("An error occurred determining the file type of {}", input_path.string());
        break;
    }
}

bool Converter::json_to_csv() {
    std::ifstream input(input_path);
    if (!input) {
        std::println("An error occurred while reading {}.", input_path.string());
        return false;
    }
    json j = json::parse(input);

    output_path = input_path;
    output_path.replace_extension(".csv");

    std::ofstream output(output_path);
    if (!output) {
        println("An error occurred while opening {}.", output_path.string());
        return false;
    }
    csv::encode_csv(j, output);

    println("Successfully converted {} to {}.", input_path.string(), output_path.string());

    return true;
}

bool Converter::csv_to_json() {
    std::ifstream input(input_path);
    if (!input) {
        println("An error occurred while reading {}.", input_path.string());
        return false;
    }

    output_path = input_path;
    output_path.replace_extension(".json");

    std::ofstream output(output_path);
    if (!output) {
        println("An error occurred while opening {}.", output_path.string());
        return false;
    }

    csv::csv_options options;
    options.header_lines(1).mapping_kind(csv::csv_mapping_kind::n_objects);

    json result = csv::decode_csv<json>(input, options);
    result.dump(output, jsoncons::indenting::indent);

    println("Successfully converted {} to {}.", input_path.string(), output_path.string());

    return true;
}

FileType Converter::read_file_ext(const fs::path& file_path) {
    std::string extension = file_path.extension().string();

    if (extension == ".csv")  {
        return FileType::CSV;
    }

    if (extension == ".json") {
        return FileType::JSON;
    }

    return FileType::NONE;
}

void Converter::csv_display_contents(const fs::path& file_path) {
    std::string line;
    std::ifstream input_file(file_path);
    if (!input_file) {
        println("An error occurred while reading {}.", file_path.string());
        return;
    }

    std::println("\n[{}]", file_path.string());
    while (std::getline(input_file, line)) {
        std::println("{}", line);
    }
    input_file.close();
}

bool Converter::csv_remove_duplicate_records() {
    std::unordered_set<std::string> seen;
    std::vector<std::string> unique_lines;
    std::string line;
    std::ifstream input_file(input_path);
    if (!input_file) {
        println("An error occurred while reading {}.", input_path.string());
        return false;
    }

    int total_removed = 0;
    while (std::getline(input_file, line)) {
        if (seen.insert(line).second) {
            unique_lines.push_back(line);
        } else {
            total_removed++;
        }
    }
    input_file.close();

    std::ofstream output_file(input_path);
    for (const auto& line : unique_lines) {
        output_file << line << "\n";
    }

    if (total_removed == 0) {
        std::println("There were no duplicate records to remove.");
        return false;
    }
    std::println("There were {} duplicate record(s) removed.", total_removed);

    return true;
}

void Converter::flatten_helper(const json& j, const std::string& prefix, json& result) {
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

bool Converter::json_flatten() {
    std::ifstream input(input_path);
    if (!input) {
        std::println("An error occurred while reading {}.", input_path.string());
        return false;
    }
    json j = json::parse(input);

    json result;
    flatten_helper(j, "", result);

    std::ofstream output(input_path);
    if (!output) {
        std::println("An error occurred while opening {}.", input_path.string());
        return false;
    }
    result.dump(output, jsoncons::indenting::indent);

    std::println("Successfully flattened {}.", input_path.string());
    return true;
}
