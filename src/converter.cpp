#include <filesystem>
#include <fstream>
#include <jsoncons/json.hpp>
#include <jsoncons_ext/csv/csv.hpp>
#include <print>

#include "converter.hpp"

using jsoncons::json;

namespace csv = jsoncons::csv;
namespace fs = std::filesystem;

void Converter::convert(std::string& input_file_path) {
    FileType input_file_type = read_input_file_ext(input_file_path);

    switch (input_file_type) {
    case FileType::CSV:
        csv_to_json(input_file_path);
        break;
    case FileType::JSON:
        json_to_csv(input_file_path);
        break;
    case FileType::NONE:
        std::println("An error occurred determining the file type of {}", input_file_path);
        break;
    }
}

bool Converter::json_to_csv(std::string& input_file_path) {
    std::ifstream input(input_file_path);
    if (!input) {
        println("An error occurred while reading {}.", input_file_path);
        return false;
    }
    json j = json::parse(input);

    fs::path input_path(input_file_path);
    std::string csv_output_file_path = input_path.replace_extension(".csv");

    std::ofstream output(csv_output_file_path);
    if (!output) {
        println("An error occurred while opening {}.", csv_output_file_path);
        return false;
    }
    csv::encode_csv(j, output);

    println("Successfully converted {} to {}.", input_file_path, csv_output_file_path);

    return true;
}

bool Converter::csv_to_json(std::string& input_file_path) {
    std::ifstream input(input_file_path);
    if (!input) {
        println("An error occurred while reading {}.", input_file_path);
        return false;
    }

    fs::path input_path(input_file_path);
    std::string json_output_file_path = input_path.replace_extension(".json");

    std::ofstream output(json_output_file_path);
    if (!output) {
        println("An error occurred while opening {}.", json_output_file_path);
        return false;
    }

    csv::csv_options options;
    options.header_lines(1).mapping_kind(csv::csv_mapping_kind::n_objects);

    json result = csv::decode_csv<json>(input, options);
    result.dump(output, jsoncons::indenting::indent);

    println("Successfully converted {} to {}.", input_file_path, json_output_file_path);

    return true;
}

FileType Converter::read_input_file_ext(std::string& input_file_path) {
    fs::path path = input_file_path;
    std::string extension = path.extension().string();

    if (extension == ".csv")  {
        return FileType::CSV;
    }

    if (extension == ".json") {
        return FileType::JSON;
    }

    return FileType::NONE;
}
