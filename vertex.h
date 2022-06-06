#ifndef VERTEX_H
#define VERTEX_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QVariant>
#include <QMap>

#include "cross.h"
#include "way.h"
class Vertex
{
public:
    Vertex();
    Vertex(Cross cross);
    Vertex (QString state);
    Vertex (int region,int area,int id,QString dgis,QString name);
    const char* Marshal();
    long getUID();
    QMap<long,Way> ways;
    int Region;
    int Area;
    int ID;
    QString Dgis;
    float Scale;
    QString Name;
    QByteArray vstr;
};

#endif // VERTEX_H
