#ifndef UTILITY_H
#define UTILITY_H

#include <QString>
#include <QtNetwork>

QString downloadWebPage(const QUrl &url);
int sepNumStrtoInt(const QString &str);
float percentagetoFloat(const QString &str);
float ChisquareFx(float x,int Freedom);
double independenttest(double a, double b, double c, double d);
template<class T>
T sign(T num)
{
    if(num > 0)
        return 1;
    else if(num < 0)
        return -1;
    else
        return 0;
}

#endif // UTILITY_H
