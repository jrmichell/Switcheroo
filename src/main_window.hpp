#pragma once

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QLabel>

#include "converter.hpp"
#include "validator.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() = default;

  private slots:
    void on_convert_clicked();
    void on_display_clicked();
    void on_remove_duplicates_clicked();
    void on_flatten_clicked();
    void on_trim_whitespace_clicked();

  private:
    void            log(const QString& message);

    QPushButton*    convert_btn_;
    QPushButton*    display_btn_;
    QPushButton*    remove_duplicates_btn_;
    QPushButton*    flatten_btn_;
    QPushButton*    trim_whitespace_btn_;
    QPushButton*    close_btn_;
    QPlainTextEdit* output_log_;
    QLabel*         file_label_;

    Validator       validator_;
    Converter       converter_;
};
