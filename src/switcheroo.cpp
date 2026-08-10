#include "main_window.hpp"
#include "theme_manager.hpp"

#include <QApplication>

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QApplication::setOrganizationName("Switcheroo");
    QApplication::setApplicationName("Switcheroo");
    app.setStyle("Fusion");

    const AppTheme theme = ThemeManager::load_theme();
    ThemeManager::apply_theme(app, theme);

    MainWindow window;
    window.show();

    return app.exec();
}
