#ifndef REMOTESERVER_H
#define REMOTESERVER_H

#include <QDialog>

namespace Ui {
class RemoteServer;
}

class RemoteServer : public QDialog
{
    Q_OBJECT

public:
    explicit RemoteServer(QWidget *parent = nullptr);
    ~RemoteServer();

private:
    Ui::RemoteServer *ui;
};

#endif // REMOTESERVER_H
