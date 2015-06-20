#ifndef UTILITY_H
#define UTILITY_H

#include <QString>
#include <QtNetwork>

QString downloadWebPage(const QUrl &url);
int sepNumStrtoInt(const QString &str);
float percentagetoFloat(const QString &str);

#endif // UTILITY_H
