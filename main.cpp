#include <QApplication>
#include <QLocale>
#include <QtCore>
#include <QTranslator>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFont ff("Cambria", 11);
//    ff.setPointSize(11);
    a.setFont(ff);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "PPT-stage2_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    w.show();
    QObject::connect(&w, &MainWindow::doExit, &QApplication::quit);
    return a.exec();
}
