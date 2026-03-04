#include "menu.hpp"
#include "validator.hpp"

int main(void) {
    Validator validator;
    Converter converter(validator);
    Menu menu(converter);

    menu.prompt_menu();

    return 0;
}
