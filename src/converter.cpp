#include <fstream>
#include <jsoncons/json.hpp>
#include <jsoncons_ext/csv/csv.hpp>
#include <print>

#include "converter.hpp"

using jsoncons::json;

namespace csv = jsoncons::csv;
namespace fs = std::filesystem;

void Converter::convert() {
    FileType input_file_type = read_input_file_ext();

    switch (input_file_type) {
    case FileType::CSV:
        csv_to_json();
        break;
    case FileType::JSON:
        json_to_csv();
        break;
    case FileType::NONE:
        std::println("An error occurred determining the file type of {}", file_path.string());
        break;
    }
}

bool Converter::json_to_csv() {
    std::ifstream input(file_path);
    if (!input) {
        std::println("An error occurred while reading {}.", file_path.string());
        return false;
    }
    json j = json::parse(input);

    fs::path output_path = file_path;
    output_path.replace_extension(".csv");

    std::ofstream output(output_path);
    if (!output) {
        println("An error occurred while opening {}.", output_path.string());
        return false;
    }
    csv::encode_csv(j, output);

    println("Successfully converted {} to {}.", file_path.string(), output_path.string());

    return true;
}

bool Converter::csv_to_json() {
    std::ifstream input(file_path);
    if (!input) {
        println("An error occurred while reading {}.", file_path.string());
        return false;
    }

    fs::path output_path = file_path;
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

    println("Successfully converted {} to {}.", file_path.string(), output_path.string());

    return true;
}

FileType Converter::read_input_file_ext() {
    std::string extension = file_path.extension().string();

    if (extension == ".csv")  {
        return FileType::CSV;
    }

    if (extension == ".json") {
        return FileType::JSON;
    }

    return FileType::NONE;
}

FileType Converter::convert_option() {
    fs::path file_path;
    std::print("Enter the relative file path: ");
    std::cin >> file_path;
    set_file_path(file_path);

    FileType file_type = read_input_file_ext();

    switch (file_type) {
        case FileType::CSV:
        case FileType::JSON:
            convert();
            break;
        case FileType::NONE:
            std::println("Failed to read file extension.");
            break;
    }

    return file_type;
}

void Converter::prompt_menu() {
    int option = 0;

    std::println("--- Switcheroo ---");
    do {
        std::println("\n[1] Convert");
        std::println("[2] Quit\n");
        std::print("Enter an option: ");

        if (!(std::cin >> option)) {
            std::println("\nPlease enter a valid option.");
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        FileType file_type = FileType::NONE;
        switch (option) {
        case 1:
            file_type = convert_option();
            if (file_type != FileType::NONE) {
                break;
            }
            break;
        case 2:
            std::println("\nGoodbye!");
            break;
        default:
            std::println("\nPlease enter a valid option.");
        }
    } while (option != 2);
}
