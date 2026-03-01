#pragma once

#include "converter.hpp"

class Menu {
    public:
        Menu(Converter& converter) 
            : converter(converter) {}

        FileType read_input_file_type();
        void prompt_menu();

    private:
        void prompt_data_validation_menu();
        Converter converter;
};
