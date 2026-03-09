#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QFileDialog>

#include "main_window.hpp"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), validator_(), converter_(validator_) {
    setWindowTitle("Switcheroo");
    resize(1280, 720);

    convert_btn_           = new QPushButton("Convert");
    display_btn_           = new QPushButton("Display File Content");
    remove_duplicates_btn_ = new QPushButton("Remove Duplicates");
    flatten_btn_           = new QPushButton("Flatten JSON");
    trim_whitespace_btn_   = new QPushButton("Trim Whitespace");
    close_btn_             = new QPushButton("Close");

    output_log_ = new QPlainTextEdit();
    output_log_->setReadOnly(true);

    file_label_ = new QLabel("No file selected");
    file_label_->setWordWrap(true);

    QVBoxLayout* validation_layout = new QVBoxLayout();
    validation_layout->addWidget(remove_duplicates_btn_);
    validation_layout->addWidget(flatten_btn_);
    validation_layout->addWidget(trim_whitespace_btn_);

    QGroupBox* validation_group = new QGroupBox("Data Validation");
    validation_group->setLayout(validation_layout);

    QVBoxLayout* left_layout = new QVBoxLayout();
    left_layout->addWidget(convert_btn_);
    left_layout->addWidget(display_btn_);
    left_layout->addWidget(validation_group);
    left_layout->addWidget(file_label_);
    left_layout->addStretch();
    left_layout->addWidget(close_btn_);

    QHBoxLayout* main_layout = new QHBoxLayout();
    main_layout->addLayout(left_layout);
    main_layout->addWidget(output_log_, 1);

    QWidget* central = new QWidget();
    central->setLayout(main_layout);
    setCentralWidget(central);

    connect(convert_btn_, &QPushButton::clicked, this, &MainWindow::on_convert_clicked);
    connect(display_btn_, &QPushButton::clicked, this, &MainWindow::on_display_clicked);
    connect(remove_duplicates_btn_, &QPushButton::clicked, this, &MainWindow::on_remove_duplicates_clicked);
    connect(flatten_btn_, &QPushButton::clicked, this, &MainWindow::on_flatten_clicked);
    connect(trim_whitespace_btn_, &QPushButton::clicked, this, &MainWindow::on_trim_whitespace_clicked);
    connect(close_btn_, &QPushButton::clicked, this, &MainWindow::close);

    auto logger = [this](const std::string& msg) { log(QString::fromStdString(msg)); };
    validator_.set_logger(logger);
    converter_.set_logger(logger);
}

void MainWindow::log(const QString& message) {
    output_log_->appendPlainText(message);
}

void MainWindow::on_convert_clicked() {
    QString path = QFileDialog::getOpenFileName(this, "Select File", "", "CSV/JSON Files (*.csv *.json)");
    if (path.isEmpty())
        return;
    file_label_->setText(path);

    output_log_->clear();
    converter_.get_input_file_path() = path.toStdString();
    converter_.convert();
}

void MainWindow::on_display_clicked() {
    QString path = QFileDialog::getOpenFileName(this, "Select File", "", "CSV/JSON Files (*.csv *.json)");
    if (path.isEmpty())
        return;
    file_label_->setText(path);

    output_log_->clear();
    converter_.display_file_contents(path.toStdString());
}

void MainWindow::on_remove_duplicates_clicked() {
    QString path = QFileDialog::getOpenFileName(this, "Select File", "", "CSV Files (*.csv)");
    if (path.isEmpty())
        return;
    file_label_->setText(path);

    output_log_->clear();
    converter_.get_input_file_path() = path.toStdString();
    converter_.csv_remove_duplicate_records();
}

void MainWindow::on_flatten_clicked() {
    QString path = QFileDialog::getOpenFileName(this, "Select File", "", "JSON Files (*.json)");
    if (path.isEmpty())
        return;
    file_label_->setText(path);

    output_log_->clear();
    converter_.get_input_file_path() = path.toStdString();
    validator_.json_flatten(converter_.get_input_file_path());
}

void MainWindow::on_trim_whitespace_clicked() {
    QString path = QFileDialog::getOpenFileName(this, "Select File", "", "CSV Files (*.csv)");
    if (path.isEmpty())
        return;
    file_label_->setText(path);

    output_log_->clear();
    converter_.get_input_file_path() = path.toStdString();
    converter_.csv_trim_whitespace();
}
