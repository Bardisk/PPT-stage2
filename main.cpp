//蒼天は円蓋の如く　陸地は棋局に似たり
//世人は黒白に分れ　往来して栄辱を争う
//栄える者は自ずから安々たり　辱めらるる者は定めて碌々たらん

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
