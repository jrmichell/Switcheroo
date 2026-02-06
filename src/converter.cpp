#include <fstream>
#include <jsoncons/json.hpp>
#include <jsoncons_ext/csv/csv.hpp>
#include <print>

#include "converter.hpp"

using jsoncons::json;

void Converter::convert(std::string input_file_path, std::string output_file_path) {
    std::ifstream input(input_file_path);
    json j = json::parse(input);
    input.close();

    std::ofstream output(output_file_path);
    jsoncons::csv::encode_csv(j, output);
    output.close();

    std::println("Converted.");
}
