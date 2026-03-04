#pragma once

#include "converter.hpp"
#include "validator.hpp"

class Menu {
    public:
        Menu(Converter& converter) 
            : converter(converter) {}

        FileType read_input_file_type();
        void prompt_menu();

    private:
        void prompt_data_validation_menu(Validator& validator);
        Converter converter;
};
