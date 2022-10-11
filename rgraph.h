#ifndef RGRAPH_H
#define RGRAPH_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#include "cross.h"
#include "vertex.h"
#include "way.h"
class DBConnect{
public:
    DBConnect();
    QString HostName;
    int Port;
    QString DBname;
    QString UserName;
    QString Password;
};

class  Rgraph
{
public:
    Rgraph(int region,DBConnect dbc);
    void Save();
    ~Rgraph();
    Cross GetCross(int area,int id);
    bool AddVertex(Cross cross);
    bool DeleteVertex(Cross cross);
    bool AddPoint(int number,QString position,QString name);
    bool DeletePoint(int number);
    int GetNumberPoint();
    bool AddWay(Cross source,Cross target,int lenght,int time);
    bool AddWayToPoint(Cross source,int number,int lenght,int time);
    bool AddWayFromPoint(int number,Cross target,int lenght,int time);
    bool DeleteWay(Cross source,Cross target);
    bool DeleteWayToPoint(Cross source,int number);
    bool DeleteWayFromPoint(int number,Cross target);
    QList<Vertex> ReadAllVertexes();
    QList<Vertex> ReadAllPoints();
    QList<Way> GetWaysFromCross(Cross cross);
    QList<Way> GetWaysFromPoint(int number);
    bool OkSql=false;
    QList<QString> errors;
private:
    bool verify(Cross cross);
    int Region; //Номер региона в котором работаем
    QMap<long,Vertex> vertexes;
    QSqlDatabase db;
};

#endif // RGRAPH_H
