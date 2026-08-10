#pragma once

#include <QString>
#include <QStringList>

class QWidget;

namespace macos {
    void    apply_title_bar(QWidget& window, bool dark);
    QString select_file(QWidget& parent, const QString& title, const QStringList& allowed_extensions);
}
