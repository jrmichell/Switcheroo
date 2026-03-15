#include <QApplication>
#include <QFont>
#include <QGuiApplication>
#include <QScreen>
#include <QStyleHints>

#include "main_window.hpp"

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    MainWindow   window;

    app.setStyle("Fusion");

    window.show();
    app.exec();

    return 0;
}
