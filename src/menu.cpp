#include <iostream>
#include <print>

#include "menu.hpp"

void Menu::prompt_data_validation_menu() {
    int option = 0;

    do {
        std::println("[1] Remove Duplicates (CSV)");
        std::println("[2] Flatten (JSON)");
        std::println("[3] Trim Whitespace (CSV)");
        std::println("[4] Back\n");
        std::print("Enter an option: ");

        if (!(std::cin >> option)) {
            std::println("\nPlease enter a valid option.");
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        std::println();

        FileType file_type = FileType::NONE;
        switch (option) {
        case 1:
            file_type = read_input_file_type();
            if (file_type != FileType::CSV) {
                std::println("You can only remove duplicates from a CSV.");
                continue;
            }
            converter.csv_remove_duplicate_records();
            break;
        case 2:
            file_type = read_input_file_type();
            if (file_type != FileType::JSON) {
                std::println("You can only flatten JSON data.");
                continue;
            }
            converter.json_flatten();
            break;
        case 3:
            file_type = read_input_file_type();
            if (file_type != FileType::CSV) {
                std::println("You can only trim whitespace from a CSV.");
                continue;
            }
            converter.csv_trim_whitespace();
            break;
        case 4:
            break;
        default:
            std::println("\nPlease enter a valid option.");
        }
    } while (option != 4);
}

FileType Menu::read_input_file_type() {
    std::print("Enter the relative file path: ");
    std::cin >> converter.get_input_file_path();

    fs::path input_path = converter.get_input_file_path();
    FileType file_type = converter.read_file_ext(input_path);

    return file_type;
}

void Menu::prompt_menu() {
    int option = 0;

    std::println("--- Switcheroo ---\n");
    do {
        std::println("[1] Convert");
        std::println("[2] Display File Content");
        std::println("[3] Data Validation");
        std::println("[4] Quit\n");
        std::print("Enter an option: ");

        if (!(std::cin >> option)) {
            std::println("\nPlease enter a valid option.");
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        std::println();

        FileType file_type = FileType::NONE;
        switch (option) {
        case 1:
            file_type = read_input_file_type();
            switch (file_type) {
                case FileType::CSV:
                case FileType::JSON:
                    converter.convert();
                    break;
                case FileType::NONE:
                    std::println("Failed to read file extension.");
                    break;
            }
            break;
        case 2:
            file_type = read_input_file_type();
            if (file_type == FileType::NONE) {
                std::println("Failed to read file extension.");
                continue;
            }
            converter.csv_display_contents(converter.get_input_file_path());
            break;
        case 3:
            prompt_data_validation_menu();
            break;
        case 4:
            std::println("Goodbye!");
            break;
        default:
            std::println("\nPlease enter a valid option.");
        }
    } while (option != 4);
}
