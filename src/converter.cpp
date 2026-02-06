#include <fstream>
#include <jsoncons/json.hpp>
#include <jsoncons_ext/csv/csv.hpp>
#include <print>

#include "converter.hpp"

using jsoncons::json;

bool Converter::convert(std::string input_file_path, std::string output_file_path) {
    std::ifstream input(input_file_path);
    if (!input) {
        std::println("An error occured while reading {}.", input_file_path);
        return false;
    }
    json j = json::parse(input);
    input.close();

    std::ofstream output(output_file_path);
    if (!output) {
        std::println("An error occured while reading {}.", output_file_path);
        return false;
    }
    jsoncons::csv::encode_csv(j, output);
    output.close();

    std::println("Successfully converted {} to {}.", input_file_path, output_file_path);

    return true;
}
