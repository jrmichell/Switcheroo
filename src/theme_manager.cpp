#include "theme_manager.hpp"

#include <QApplication>
#include <QColor>
#include <QPalette>
#include <QSettings>

namespace {
    constexpr auto theme_key = "appearance/theme";
}

AppTheme ThemeManager::load_theme() {
    QSettings     settings;
    const QString saved_theme = settings.value(theme_key).toString();

    if (saved_theme == "light")
        return AppTheme::Light;
    if (saved_theme == "dark")
        return AppTheme::Dark;

    return AppTheme::Dark;
}

void ThemeManager::apply_theme(QApplication& app, AppTheme theme) {
    const bool dark = theme == AppTheme::Dark;

    QPalette   palette;
    palette.setColor(QPalette::Window, QColor(dark ? "#0f172a" : "#f4f7fb"));
    palette.setColor(QPalette::WindowText, QColor(dark ? "#e5e7eb" : "#172033"));
    palette.setColor(QPalette::Base, QColor(dark ? "#111827" : "#ffffff"));
    palette.setColor(QPalette::AlternateBase, QColor(dark ? "#182235" : "#f8fafc"));
    palette.setColor(QPalette::Text, QColor(dark ? "#e5e7eb" : "#172033"));
    palette.setColor(QPalette::Button, QColor(dark ? "#1e293b" : "#ffffff"));
    palette.setColor(QPalette::ButtonText, QColor(dark ? "#e5e7eb" : "#172033"));
    palette.setColor(QPalette::Highlight, QColor("#6366f1"));
    palette.setColor(QPalette::HighlightedText, QColor("#ffffff"));
    palette.setColor(QPalette::ToolTipBase, QColor(dark ? "#1e293b" : "#ffffff"));
    palette.setColor(QPalette::ToolTipText, QColor(dark ? "#f8fafc" : "#172033"));
    palette.setColor(QPalette::PlaceholderText, QColor(dark ? "#64748b" : "#94a3b8"));
    palette.setColor(QPalette::Disabled, QPalette::Text, QColor(dark ? "#64748b" : "#94a3b8"));
    palette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(dark ? "#64748b" : "#94a3b8"));

    app.setPalette(palette);
    app.setStyleSheet(style_sheet(theme));
}

void ThemeManager::save_theme(AppTheme theme) {
    QSettings settings;
    settings.setValue(theme_key, theme == AppTheme::Dark ? "dark" : "light");
}

QString ThemeManager::display_name(AppTheme theme) {
    return theme == AppTheme::Dark ? "Dark" : "Light";
}

QString ThemeManager::style_sheet(AppTheme theme) {
    const bool    dark        = theme == AppTheme::Dark;
    const QString background  = dark ? "#0f172a" : "#f4f7fb";
    const QString surface     = dark ? "#172033" : "#ffffff";
    const QString surface_alt = dark ? "#111827" : "#f8fafc";
    const QString border      = dark ? "#2a3850" : "#dce3ee";
    const QString text        = dark ? "#e5e7eb" : "#172033";
    const QString muted       = dark ? "#94a3b8" : "#64748b";
    const QString hover       = dark ? "#23314a" : "#eef2ff";

    return QString(R"(
        QMainWindow, QWidget#appRoot { background: %1; color: %5; }
        QLabel { color: %5; background: transparent; }
        QLabel#pageTitle { font-size: 25px; font-weight: 750; }
        QLabel#pageSubtitle, QLabel#sectionHint, QLabel#selectedPath, QLabel#statusLabel {
            color: %6;
        }
        QLabel#sectionTitle { font-size: 12px; font-weight: 700; color: %6; }
        QLabel#selectedName { font-size: 14px; font-weight: 650; }
        QFrame#card, QFrame#fileCard {
            background: %2; border: 1px solid %4; border-radius: 14px;
        }
        QFrame#fileCard { background: %3; border-radius: 10px; }
        QPushButton {
            background: %2; color: %5; border: 1px solid %4; border-radius: 9px;
            min-height: 38px; padding: 0 14px; font-weight: 600; text-align: left;
        }
        QPushButton:hover { background: %7; border-color: #818cf8; }
        QPushButton:pressed { background: %3; }
        QPushButton[buttonStyle="primary"] {
            background: #6366f1; color: white; border-color: #6366f1;
        }
        QPushButton[buttonStyle="primary"]:hover {
            background: #4f46e5; border-color: #4f46e5;
        }
        QPushButton[buttonStyle="quiet"] {
            background: transparent; border-color: transparent; color: %6;
        }
        QPushButton[buttonStyle="quiet"]:hover { background: %7; color: %5; }
        QPushButton#themeButton { text-align: center; min-width: 92px; }
        QPlainTextEdit {
            background: %3; color: %5; border: 1px solid %4; border-radius: 10px;
            padding: 12px; selection-background-color: #6366f1;
            font-family: "SFMono-Regular", "Cascadia Code", "Consolas", monospace;
            font-size: 13px;
        }
        QPlainTextEdit:focus { border-color: #818cf8; }
        QScrollBar:vertical { background: transparent; width: 10px; margin: 4px 2px; }
        QScrollBar::handle:vertical { background: %4; border-radius: 4px; min-height: 28px; }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }
        QToolTip { background: %2; color: %5; border: 1px solid %4; padding: 5px; }
    )")
        .arg(background, surface, surface_alt, border, text, muted, hover);
}
