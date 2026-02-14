#include <iostream>
#include <print>

#include "converter.hpp"

FileType convert_option(Converter& converter, std::string& file_path);
void prompt_menu(Converter& converter);

int main(int argc, char** argv) {
    Converter converter;
    prompt_menu(converter);

    return 0;
}

FileType convert_option(Converter& converter, std::string& file_path) {
    std::print("Enter the relative file path: ");
    std::cin >> file_path;

    FileType file_type = converter.read_input_file_ext(file_path);

    switch (file_type) {
        case FileType::CSV:
        case FileType::JSON:
            converter.convert(file_path);
            break;
        case FileType::NONE:
            std::println("Failed to read file extension.");
            break;
    }

    return file_type;
}

void prompt_menu(Converter& converter) {
    std::string file_path = "";
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
            file_type = convert_option(converter, file_path);
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
