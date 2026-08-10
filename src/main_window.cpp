#include "main_window.hpp"

#ifdef Q_OS_MACOS
#include "macos_platform.hpp"
#endif

#include <QApplication>
#include <QFileDialog>
#include <QFileInfo>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QShowEvent>
#include <QVBoxLayout>

namespace {
    QPushButton* make_button(const QString& text, const QString& style = {}, const QString& tooltip = {}) {
        auto* button = new QPushButton(text);
        if (!style.isEmpty())
            button->setProperty("buttonStyle", style);
        if (!tooltip.isEmpty())
            button->setToolTip(tooltip);
        button->setCursor(Qt::PointingHandCursor);
        return button;
    }

    QLabel* make_section_title(const QString& text) {
        auto* label = new QLabel(text.toUpper());
        label->setObjectName("sectionTitle");
        return label;
    }

    QFrame* make_card() {
        auto* card = new QFrame();
        card->setObjectName("card");
        return card;
    }
}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), theme_(ThemeManager::load_theme()), validator_(), converter_() {
    setWindowTitle("Switcheroo");
    setMinimumSize(900, 600);
    resize(1180, 760);

    build_ui();
    connect_actions();
    update_theme_button();

    auto logger = [this](const std::string& message) { log(QString::fromStdString(message)); };
    validator_.set_logger(logger);
    converter_.set_logger(logger);
}

void MainWindow::build_ui() {
    auto* root = new QWidget();
    root->setObjectName("appRoot");
    auto* root_layout = new QVBoxLayout(root);
#ifdef Q_OS_MACOS
    // The content extends beneath the transparent native title bar.
    root_layout->setContentsMargins(28, 28, 28, 24);
#else
    root_layout->setContentsMargins(28, 22, 28, 24);
#endif
    root_layout->setSpacing(20);

    auto* title = new QLabel("Switcheroo");
    title->setObjectName("pageTitle");
    auto* subtitle = new QLabel("Convert and clean structured data without the friction.");
    subtitle->setObjectName("pageSubtitle");

    auto* heading_layout = new QVBoxLayout();
    heading_layout->setContentsMargins(0, 0, 0, 0);
    heading_layout->setSpacing(2);
    heading_layout->addWidget(title);
    heading_layout->addWidget(subtitle);

    theme_btn_ = make_button({}, {}, "Switch between light and dark appearance");
    theme_btn_->setObjectName("themeButton");
    close_btn_ = make_button("Quit", "quiet");

    auto* header = new QHBoxLayout();
    header->setContentsMargins(0, 0, 0, 0);
    header->setSpacing(8);
    header->addLayout(heading_layout);
    header->addStretch();
    header->addWidget(theme_btn_);
    header->addWidget(close_btn_);
    root_layout->addLayout(header);

    auto* actions_card = make_card();
    actions_card->setFixedWidth(300);
    auto* actions_layout = new QVBoxLayout(actions_card);
    actions_layout->setContentsMargins(18, 18, 18, 18);
    actions_layout->setSpacing(10);

    auto* actions_title = new QLabel("File actions");
    actions_title->setObjectName("selectedName");
    auto* actions_hint = new QLabel("Choose a file when you start an action.");
    actions_hint->setObjectName("sectionHint");
    actions_hint->setWordWrap(true);
    actions_layout->addWidget(actions_title);
    actions_layout->addWidget(actions_hint);
    actions_layout->addSpacing(6);

    convert_btn_ = make_button("Convert CSV / JSON", "primary", "Convert between CSV and JSON");
    display_btn_ = make_button("Preview file", {}, "Display the selected file's contents");
    actions_layout->addWidget(convert_btn_);
    actions_layout->addWidget(display_btn_);
    actions_layout->addSpacing(12);
    actions_layout->addWidget(make_section_title("Data tools"));

    remove_duplicates_btn_ = make_button("Remove duplicate rows", {}, "Remove duplicate records from a CSV file");
    flatten_btn_           = make_button("Flatten nested JSON", {}, "Flatten nested fields while preserving records");
    trim_whitespace_btn_   = make_button("Trim CSV whitespace", {}, "Trim surrounding whitespace from CSV fields");
    actions_layout->addWidget(remove_duplicates_btn_);
    actions_layout->addWidget(flatten_btn_);
    actions_layout->addWidget(trim_whitespace_btn_);
    actions_layout->addStretch();
    actions_layout->addWidget(make_section_title("Selected file"));

    auto* file_card = new QFrame();
    file_card->setObjectName("fileCard");
    auto* file_layout = new QVBoxLayout(file_card);
    file_layout->setContentsMargins(12, 10, 12, 10);
    file_layout->setSpacing(3);
    file_name_label_ = new QLabel("No file selected");
    file_name_label_->setObjectName("selectedName");
    file_path_label_ = new QLabel("Your most recent selection appears here.");
    file_path_label_->setObjectName("selectedPath");
    file_path_label_->setWordWrap(true);
    file_layout->addWidget(file_name_label_);
    file_layout->addWidget(file_path_label_);
    actions_layout->addWidget(file_card);

    auto* output_card   = make_card();
    auto* output_layout = new QVBoxLayout(output_card);
    output_layout->setContentsMargins(18, 16, 18, 18);
    output_layout->setSpacing(12);

    auto* output_title = new QLabel("Activity");
    output_title->setObjectName("selectedName");
    auto* output_hint = new QLabel("Results, file previews, and errors appear below.");
    output_hint->setObjectName("sectionHint");
    auto* output_heading = new QVBoxLayout();
    output_heading->setSpacing(2);
    output_heading->addWidget(output_title);
    output_heading->addWidget(output_hint);

    clear_btn_          = make_button("Clear", "quiet", "Clear the activity output");
    auto* output_header = new QHBoxLayout();
    output_header->addLayout(output_heading);
    output_header->addStretch();
    output_header->addWidget(clear_btn_);
    output_layout->addLayout(output_header);

    output_log_ = new QPlainTextEdit();
    output_log_->setReadOnly(true);
    output_log_->setLineWrapMode(QPlainTextEdit::NoWrap);
    output_log_->setPlaceholderText("Ready when you are. Choose an action to get started.");
    output_layout->addWidget(output_log_, 1);

    auto* content = new QHBoxLayout();
    content->setSpacing(18);
    content->addWidget(actions_card);
    content->addWidget(output_card, 1);
    root_layout->addLayout(content, 1);

    status_label_ = new QLabel("Ready");
    status_label_->setObjectName("statusLabel");
    root_layout->addWidget(status_label_);

    setCentralWidget(root);
}

void MainWindow::showEvent(QShowEvent* event) {
    QMainWindow::showEvent(event);
#ifdef Q_OS_MACOS
    // Qt may recreate native window styling while showing the window.
    macos::apply_title_bar(*this, theme_ == AppTheme::Dark);
#endif
}

void MainWindow::connect_actions() {
    connect(convert_btn_, &QPushButton::clicked, this, &MainWindow::on_convert_clicked);
    connect(display_btn_, &QPushButton::clicked, this, &MainWindow::on_display_clicked);
    connect(remove_duplicates_btn_, &QPushButton::clicked, this, &MainWindow::on_remove_duplicates_clicked);
    connect(flatten_btn_, &QPushButton::clicked, this, &MainWindow::on_flatten_clicked);
    connect(trim_whitespace_btn_, &QPushButton::clicked, this, &MainWindow::on_trim_whitespace_clicked);
    connect(theme_btn_, &QPushButton::clicked, this, &MainWindow::on_theme_clicked);
    connect(clear_btn_, &QPushButton::clicked, output_log_, &QPlainTextEdit::clear);
    connect(close_btn_, &QPushButton::clicked, this, &MainWindow::close);
}

QString MainWindow::select_file(const QString& caption, const QString& filter, const QStringList& allowed_extensions) {
#ifdef Q_OS_MACOS
    static_cast<void>(filter);
    const QString path = macos::select_file(*this, caption, allowed_extensions);
#else
    static_cast<void>(allowed_extensions);
    const QString path = QFileDialog::getOpenFileName(this, caption, {}, filter);
#endif
    if (!path.isEmpty())
        set_selected_file(path);
    return path;
}

void MainWindow::set_selected_file(const QString& path) {
    const QFileInfo file(path);
    file_name_label_->setText(file.fileName());
    file_path_label_->setText(file.absolutePath());
    file_path_label_->setToolTip(file.absoluteFilePath());
    set_status(QString("Selected %1").arg(file.fileName()));
}

void MainWindow::set_status(const QString& message) {
    status_label_->setText(message);
}

void MainWindow::update_theme_button() {
    theme_btn_->setText(theme_ == AppTheme::Dark ? "Light mode" : "Dark mode");
}

void MainWindow::log(const QString& message) {
    QString display_message = message;
    while (display_message.endsWith('\n') || display_message.endsWith('\r'))
        display_message.chop(1);

    output_log_->appendPlainText(display_message);
    set_status("Operation complete");
}

void MainWindow::on_convert_clicked() {
    const QString path = select_file("Select a CSV or JSON file", "CSV and JSON (*.csv *.json)", {"csv", "json"});
    if (path.isEmpty())
        return;

    output_log_->clear();
    set_status("Converting file...");
    converter_.set_input_file_path(path.toStdString());
    converter_.convert();
}

void MainWindow::on_display_clicked() {
    const QString path = select_file("Select a CSV or JSON file", "CSV and JSON (*.csv *.json)", {"csv", "json"});
    if (path.isEmpty())
        return;

    output_log_->clear();
    set_status("Loading preview...");
    converter_.display_file_contents(path.toStdString());
}

void MainWindow::on_remove_duplicates_clicked() {
    const QString path = select_file("Select a CSV file", "CSV files (*.csv)", {"csv"});
    if (path.isEmpty())
        return;

    output_log_->clear();
    set_status("Removing duplicate rows...");
    converter_.set_input_file_path(path.toStdString());
    converter_.csv_remove_duplicate_records();
}

void MainWindow::on_flatten_clicked() {
    const QString path = select_file("Select a JSON file", "JSON files (*.json)", {"json"});
    if (path.isEmpty())
        return;

    output_log_->clear();
    set_status("Flattening JSON...");
    validator_.json_flatten(path.toStdString());
}

void MainWindow::on_trim_whitespace_clicked() {
    const QString path = select_file("Select a CSV file", "CSV files (*.csv)", {"csv"});
    if (path.isEmpty())
        return;

    output_log_->clear();
    set_status("Trimming whitespace...");
    converter_.set_input_file_path(path.toStdString());
    converter_.csv_trim_whitespace();
}

void MainWindow::on_theme_clicked() {
    theme_ = theme_ == AppTheme::Dark ? AppTheme::Light : AppTheme::Dark;
    ThemeManager::apply_theme(*qApp, theme_);
    ThemeManager::save_theme(theme_);
    update_theme_button();
#ifdef Q_OS_MACOS
    macos::apply_title_bar(*this, theme_ == AppTheme::Dark);
#endif
    set_status(QString("%1 mode enabled").arg(ThemeManager::display_name(theme_)));
}
