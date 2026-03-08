#include <algorithm>
#include <fstream>
#include <sstream>
#include <jsoncons_ext/csv/csv.hpp>
#include <print>

#include "converter.hpp"

using jsoncons::json;

namespace csv = jsoncons::csv;
namespace fs  = std::filesystem;

void Converter::convert() {
    FileType input_file_type = read_file_ext(input_path);

    switch (input_file_type) {
        case FileType::CSV: csv_to_json(); break;
        case FileType::JSON: json_to_csv(); break;
        case FileType::NONE: std::println("An error occurred determining the file type of {}\n", input_path.string()); break;
    }
}

bool Converter::json_to_csv() {
    std::ifstream input(input_path);
    if (!input) {
        std::println("An error occurred while reading {}.\n", input_path.string());
        return false;
    }
    json j = json::parse(input);

    output_path = input_path;
    output_path.replace_extension(".csv");

    std::ofstream output(output_path);
    if (!output) {
        std::println("An error occurred while opening {}.\n", output_path.string());
        return false;
    }
    csv::encode_csv(j, output);

    std::println("Successfully converted {} to {}.\n", input_path.string(), output_path.string());

    return true;
}

bool Converter::csv_validate_header(const fs::path& file_path) {
    std::ifstream input(file_path);
    if (!input)
        return false;

    std::string first_line;
    if (!std::getline(input, first_line) || first_line.empty()) {
        std::println("CSV file {} must have a header row.\n", file_path.string());
        return false;
    }

    std::stringstream ss(first_line);
    std::string       field;
    while (std::getline(ss, field, ',')) {
        size_t start = field.find_first_not_of(" \t");
        size_t end   = field.find_last_not_of(" \t");
        if (start != std::string::npos)
            field = field.substr(start, end - start + 1);

        bool is_numeric = !field.empty() && std::all_of(field.begin(), field.end(), [](char c) { return std::isdigit(c) || c == '.' || c == '-'; });

        if (is_numeric) {
            std::println("CSV file {} must have a header row.\n", file_path.string());
            return false;
        }
    }

    return true;
}

bool Converter::csv_to_json() {
    if (!csv_validate_header(input_path))
        return false;

    std::ifstream input(input_path);
    if (!input) {
        std::println("An error occurred while reading {}.\n", input_path.string());
        return false;
    }

    output_path = input_path;
    output_path.replace_extension(".json");

    std::ofstream output(output_path);
    if (!output) {
        std::println("An error occurred while opening {}.\n", output_path.string());
        return false;
    }

    csv::csv_options options;
    options.header_lines(1).mapping_kind(csv::csv_mapping_kind::n_objects);

    json result = csv::decode_csv<json>(input, options);
    result.dump(output, jsoncons::indenting::indent);

    std::println("Successfully converted {} to {}.\n", input_path.string(), output_path.string());

    return true;
}

FileType Converter::read_file_ext(const fs::path& file_path) {
    std::string extension = file_path.extension().string();

    if (extension == ".csv") {
        return FileType::CSV;
    }

    if (extension == ".json") {
        return FileType::JSON;
    }

    return FileType::NONE;
}

void Converter::csv_display_contents(const fs::path& file_path) {
    std::string   line;
    std::ifstream input_file(file_path);
    if (!input_file) {
        std::println("An error occurred while reading {}.\n", file_path.string());
        return;
    }

    std::println("\n[{}]", file_path.string());
    while (std::getline(input_file, line)) {
        std::println("{}", line);
    }
    std::println();
    input_file.close();
}

bool Converter::csv_remove_duplicate_records() {
    if (!csv_validate_header(input_path))
        return false;

    std::unordered_set<std::string> seen;
    std::vector<std::string>        unique_lines;
    std::string                     line;
    std::ifstream                   input_file(input_path);
    if (!input_file) {
        std::println("An error occurred while reading {}.\n", input_path.string());
        return false;
    }

    int total_removed = 0;
    while (std::getline(input_file, line)) {
        if (seen.insert(line).second) {
            unique_lines.push_back(line);
        } else {
            total_removed++;
        }
    }
    input_file.close();

    std::ofstream output_file(input_path);
    for (const auto& line : unique_lines) {
        output_file << line << "\n";
    }

    if (total_removed == 0) {
        std::println("There were no duplicate records to remove.\n");
        return false;
    }
    std::println("There were {} duplicate record(s) removed.\n", total_removed);

    return true;
}

static std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end   = s.find_last_not_of(" \t\r\n");
    return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
}

bool Converter::csv_trim_whitespace() {
    if (!csv_validate_header(input_path))
        return false;

    std::ifstream input_file(input_path);
    if (!input_file) {
        std::println("An error occurred while reading {}.\n", input_path.string());
        return false;
    }

    std::vector<std::string> result_lines;
    std::string              line;
    while (std::getline(input_file, line)) {
        std::string result_line;
        std::string field;
        bool        in_quotes   = false;
        bool        first_field = true;

        for (size_t i = 0; i <= line.size(); ++i) {
            char c = (i < line.size()) ? line[i] : '\0';
            if (c == '"') {
                in_quotes = !in_quotes;
                field += c;
            } else if ((c == ',' || i == line.size()) && !in_quotes) {
                if (!first_field)
                    result_line += ',';
                result_line += trim(field);
                field.clear();
                first_field = false;
            } else {
                field += c;
            }
        }
        result_lines.push_back(result_line);
    }
    input_file.close();

    std::ofstream output_file(input_path);
    for (const auto& l : result_lines) {
        output_file << l << "\n";
    }

    std::println("Successfully trimmed whitespace in {}.\n", input_path.string());
    return true;
}
