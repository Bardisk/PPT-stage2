#include "remoteserver.h"
#include "ui_remoteserver.h"

RemoteServer::RemoteServer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RemoteServer)
{
    ui->setupUi(this);
}

RemoteServer::~RemoteServer()
{
    delete ui;
}
