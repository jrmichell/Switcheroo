#include <iostream>
#include <print>

#include "menu.hpp"

FileType Menu::read_input_file_type() {
    std::print("Enter the relative file path: ");
    std::cin >> m_converter.get_input_file_path();

    fs::path input_path = m_converter.get_input_file_path();
    FileType file_type = m_converter.read_file_ext(input_path);

    return file_type;
}

void Menu::prompt_menu() {
    int option = 0;

    std::println("--- Switcheroo ---");
    do {
        std::println("\n[1] Convert");
        std::println("[2] Display File Content");
        std::println("[3] Remove Duplicates");
        std::println("[4] Quit\n");
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
            file_type = read_input_file_type();
            switch (file_type) {
                case FileType::CSV:
                case FileType::JSON:
                    m_converter.convert();
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
            m_converter.csv_display_contents(m_converter.get_input_file_path()); 
            break;
        case 3:
            file_type = read_input_file_type();
            if (file_type != FileType::CSV) {
                std::println("You can only remove duplicates from a CSV.");
                continue;
            }
            m_converter.csv_remove_duplicate_records();
            break;
        case 4:
            std::println("\nGoodbye!");
            break;
        default:
            std::println("\nPlease enter a valid option.");
        }
    } while (option != 4);
}
