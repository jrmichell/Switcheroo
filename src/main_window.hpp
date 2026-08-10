#pragma once

#include "converter.hpp"
#include "theme_manager.hpp"
#include "validator.hpp"

#include <QMainWindow>
#include <QStringList>

class QLabel;
class QPlainTextEdit;
class QPushButton;
class QShowEvent;
class QString;
class QWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() = default;

  protected:
    void showEvent(QShowEvent* event) override;

  private slots:
    void on_convert_clicked();
    void on_display_clicked();
    void on_remove_duplicates_clicked();
    void on_flatten_clicked();
    void on_trim_whitespace_clicked();
    void on_theme_clicked();

  private:
    void            build_ui();
    void            connect_actions();
    QString         select_file(const QString& caption, const QString& filter, const QStringList& allowed_extensions);
    void            set_selected_file(const QString& path);
    void            set_status(const QString& message);
    void            update_theme_button();
    void            log(const QString& message);

    QPushButton*    convert_btn_           = nullptr;
    QPushButton*    display_btn_           = nullptr;
    QPushButton*    remove_duplicates_btn_ = nullptr;
    QPushButton*    flatten_btn_           = nullptr;
    QPushButton*    trim_whitespace_btn_   = nullptr;
    QPushButton*    theme_btn_             = nullptr;
    QPushButton*    clear_btn_             = nullptr;
    QPushButton*    close_btn_             = nullptr;
    QPlainTextEdit* output_log_            = nullptr;
    QLabel*         file_name_label_       = nullptr;
    QLabel*         file_path_label_       = nullptr;
    QLabel*         status_label_          = nullptr;

    AppTheme        theme_;
    Validator       validator_;
    Converter       converter_;
};
