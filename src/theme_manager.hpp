#pragma once

#include <QString>

class QApplication;

enum class AppTheme {
    Light,
    Dark
};

class ThemeManager {
  public:
    static AppTheme load_theme();
    static void     apply_theme(QApplication& app, AppTheme theme);
    static void     save_theme(AppTheme theme);
    static QString  display_name(AppTheme theme);

  private:
    static QString style_sheet(AppTheme theme);
};
