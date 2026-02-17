#include "menu.hpp"

int main(void) {
    Converter converter;
    Menu menu(converter);
    menu.prompt_menu();

    return 0;
}
