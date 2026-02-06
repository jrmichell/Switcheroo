#include <fstream>
#include <jsoncons/json.hpp>
#include <jsoncons_ext/csv/csv.hpp>
#include <print>

#include "jsoncons_ext/csv/encode_csv.hpp"

using jsoncons::json;

int main(int argc, char** argv) {
    if (argc != 3) {
        std::println("Usage: {} <input_json_file> <output_csv_file>", argv[0]);
        return 1;
    }

    std::ifstream input(argv[1]);
    json j = json::parse(input);
    input.close();
    
    std::ofstream output(argv[2]);
    jsoncons::csv::encode_csv(j, output);
    output.close();
    
    std::println("Converted.");
    
    return 0;
}
