#include <print>

#include "converter.hpp"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::println("Usage: {} <input_file>", argv[0]);
        return 1;
    }

    std::string input_file_path = argv[1];

    Converter converter;
    converter.convert(input_file_path);

    return 0;
}
