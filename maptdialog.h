#ifndef MAPTDIALOG_H
#define MAPTDIALOG_H

#include <QDialog>
#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include <QLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>

class maptDialog : public QDialog
{
    Q_OBJECT
    QLineEdit *line1, *line2;
    void transMap();
    void find1();
    void find2();
public:
    maptDialog(QWidget *parent);
};

#endif // MAPTDIALOG_H
