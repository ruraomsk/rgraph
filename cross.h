#ifndef CROSS_H
#define CROSS_H
/*
 * Описание перекрестка
 */
#include <QJsonDocument>
#include <QString>
#include <QVariant>
#include <QMap>

class Cross
{
public:
    Cross(int region,int area,int id,QString dgis,QString name);
    Cross (QString state);
    int Region=0;
    int Area=0;
    int ID=0;
    QString Dgis="(0.00,0.00)";
    QString Name="undef";
    float Scale=1.0;
    long getUID();

};

#endif // CROSS_H
