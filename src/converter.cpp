#include <fstream>
#include <jsoncons/json.hpp>
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
    output_path.replace_extension(".csv");

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

FileType Converter::read_file_ext(fs::path& file_path) {
    std::string extension = file_path.extension().string();

    if (extension == ".csv")  {
        return FileType::CSV;
    }

    if (extension == ".json") {
        return FileType::JSON;
    }

    return FileType::NONE;
}
