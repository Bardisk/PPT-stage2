#include <QtCore>
#ifndef USERCONFIG_H
#define USERCONFIG_H


class UserConfig
{
public:
    bool enableAutoQload;
    UserConfig();
    UserConfig(QJsonDocument *from);
};

#endif // USERCONFIG_H
