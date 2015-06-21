#include "utility.h"
#include <QRegExp>
#include <QtMath>

QString downloadWebPage(const QUrl &url)
{
    QNetworkRequest request;
    QSslConfiguration config;

    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    config.setProtocol(QSsl::TlsV1SslV3);

    request.setSslConfiguration(config);
    request.setUrl(url);

    QNetworkAccessManager manager;
    QNetworkReply *reply = manager.get(request);

    QEventLoop eventLoop;
    QObject::connect(&manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    eventLoop.exec();       //block until finish
    return reply->readAll();
}

int sepNumStrtoInt(const QString &str)
{
    QString s = str;
    s.replace(",", "");

    return s.toInt();
}

float percentagetoFloat(const QString &str)
{
    static QRegExp rx("^([+-]?\\d+(\\.\\d+)?)%$");
    if(rx.indexIn(str) >= 0)
        return rx.cap(1).toFloat() /100;
    else
        return 0;
}

float NormalPx(float x)
{
    float f;
    f=1.0/sqrt(2.0*M_PI)*exp(-x*x/2.0);
    return f;
}

float NormalFx(float x)
{
    float prob,t,temp;
    int i,n,flag;
    temp=x;
    if(x<0)
        x=-x;
    n=30;
    if(x>=0&&x<=3.0)
    {
        t=0.0;
        for(i=n;i>=1;i--)
        {
            if(i%2==1) flag=-1;
            else flag=1;
            t=flag*i*x*x/(2.0*i+1.0+t);
        }
        prob=0.5+NormalPx(x)*x/(1.0+t);
    }
    else if(x>3.0)
    {t=0.0;
        for(i=n;i>=1;i--)
            t=1.0*i/(x+t);
        prob=1-NormalPx(x)/(x+t);
    }
    x=temp;
    if(x<0)
        prob=1.0-prob;
    return prob;
}

float ChisquareFx(float x,int Freedom)
{
    float f,h,prob;

    f=exp(-x/2.0)/sqrt(2*M_PI*x);
    h=2.0*NormalFx(sqrt(x))-1.0;

    prob=1-h;

    return prob;
}

double independenttest(double a, double b, double c, double d)
{
    double s = a * d - b * c;
    double a1 = (a + b + c + d) * pow(s, 2);
    double a2 = (a + b) * (c + d) * (a + c) * (b + d);
    return sign(s) * a1 / a2;
}
