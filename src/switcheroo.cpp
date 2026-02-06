#include <print>

#include "converter.hpp"

int main(int argc, char** argv) {
    if (argc != 3) {
        std::println("Usage: {} <input_json_file> <output_csv_file>", argv[0]);
        return 1;
    }

    std::string input_file_path = argv[1];
    std::string output_file_path = argv[2];

    Converter converter;
    if (!converter.convert(input_file_path, output_file_path)) {
        return 1;
    }

    return 0;
}
