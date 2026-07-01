#include <QApplication>
#include <QDir>
#include <QMessageBox>
#include <QTranslator>
#include <QLocale>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Muat terjemahan berdasarkan locale
    QTranslator translator;
    QString locale = QLocale::system().name();
    if (locale.startsWith("id")) {
        translator.load(":/translations/walper_id.qm");
    } else {
        translator.load(":/translations/walper_en.qm");
    }
    app.installTranslator(&translator);

    // Buat folder ~/.config/walper/ jika belum ada
    QString configDir = QDir::homePath() + "/.config/walper";
    QDir dir;
    if (!dir.mkpath(configDir)) {
        QMessageBox::critical(nullptr,
            QApplication::translate("QApplication", "Error"),
            QApplication::translate("QApplication", "Failed to create directory ") + configDir);
        return 1;
    }

    MainWindow window;
    window.show();

    return app.exec();
}
