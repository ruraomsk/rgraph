#include "rgraph.h"


Rgraph::Rgraph(int region, DBConnect dbc)
{
    this->Region=region;
    db=QSqlDatabase::addDatabase("QPSQL");db.setHostName(dbc.HostName);
    db.setPort(dbc.Port);
    db.setDatabaseName(dbc.DBname);
    db.setUserName(dbc.UserName);
    db.setPassword(dbc.Password);
    OkSql=db.open();
    if (!OkSql){
        QSqlError last=db.lastError();

        errors.append(last.databaseText());
        errors.append(last.driverText());
        return;
    }
    QSqlQuery quevertex(QString::asprintf("select state from public.vertex where region=%d;",Region));
    while (quevertex.next()) {
        QString state=quevertex.value(0).toString();
        auto vertex=Vertex(state);
        vertexes[vertex.getUID()]=vertex;
    }
    QSqlQuery queways(QString::asprintf("select info from public.ways where region=%d;",Region));
    while (queways.next()){
        QString info=queways.value(0).toString();
        auto way=Way(info);
        bool e=false;
        if (!vertexes.contains(way.Source)){
            e=true;
            errors.append(QString::asprintf("нет такого узла %s ",way.SourceToString()));
        }
        if (!vertexes.contains(way.Target)){
            e=true;
            errors.append(QString::asprintf("нет такого узла %s ",way.TargetToString()));
        }
        if (e) continue;
        vertexes[way.Source].ways[way.Target]=way;
    }
}

void Rgraph::Save()
{
    QSqlQuery delvertex(QString::asprintf("delete from public.vertex where region=%d;",Region));
    QSqlQuery delway(QString::asprintf("delete from public.ways where region=%d;",Region));
    foreach(auto v,vertexes){
        QSqlQuery addvertex(QString::asprintf(
                                "insert into public.vertex (region,uids,state) values (%d,%ld,'%s');",
                                Region,v.getUID(),v.Marshal()));
        foreach (auto w, v.ways) {
            QSqlQuery addway(QString::asprintf(
                                 "insert into public.ways (region,source,target,info) values (%d,%ld,%ld,'%s');",
                                 Region,w.Source,w.Target,w.Marshal()));
        }
    }
}

Rgraph::~Rgraph()
{
    Save();
    db.close();
}

Cross Rgraph::GetCross(int area, int id)
{
    QSqlQuery quecross(QString::asprintf("select state from public.\"cross\" where region=%d and area=%d and id=%d;",Region,area,id));
    while(quecross.next()){
        QString state=quecross.value(0).toString();
        auto cross=Cross(state);
        return cross;
    }
    return Cross(0,0,0,"","");
}

bool Rgraph::AddVertex(Cross cross)
{
    if (verify(cross)){
        return false;
    }
    auto v=Vertex(cross);
    vertexes[v.getUID()]=v;
    return true;
}

bool Rgraph::DeleteVertex(Cross cross)
{
    if (!verify(cross)) {
        return false;
    }
    vertexes.remove(cross.getUID());
    foreach(auto v,vertexes){
        v.ways.remove(cross.getUID());
    }
    return true;
}

bool Rgraph::AddPoint(int number, QString position, QString name)
{
    auto cross=Cross(Region,0,number,position,name);
    if (verify(cross)){
        return false;
    }
    auto v=Vertex(cross);
    vertexes[v.getUID()]=v;
    return true;
}

bool Rgraph::DeletePoint(int number)
{
    auto cross=Cross(Region,0,number,"","");
    if (!verify(cross)) {
        return false;
    }
    vertexes.remove(cross.getUID());
    foreach(auto v,vertexes){
        v.ways.remove(cross.getUID());
    }
    return true;
}


int Rgraph::GetNumberPoint()
{
    int number=0;
    foreach(auto v,vertexes){
        if (v.Area==0){
            number=v.ID>number?v.ID:number;
        }
    }
    return ++number;
}

bool Rgraph::AddWay(Cross source, Cross target, int lenght)
{
    if (!verify(source) ||!verify(target) ) return false;
    Way way;
    way.Region=Region;
    way.Source=source.getUID();
    way.Target=target.getUID();
    way.Start=source.Dgis;
    way.Stop=target.Dgis;
    way.Lenght=lenght;
    vertexes[way.Source].ways[way.Target]=way;
    return true;
}

bool Rgraph::AddWayToPoint(Cross source, int number, int lenght)
{
    Cross target=Cross(Region,0,number,"","");
    if (!vertexes.contains(target.getUID())) return false;
    auto p=vertexes[target.getUID()];
    target.Dgis=p.Dgis;
    target.Name=p.Name;
    return AddWay(source,target,lenght);
}

bool Rgraph::AddWayFromPoint(int number, Cross target, int lenght)
{
    Cross source=Cross(Region,0,number,"","");
    if (!vertexes.contains(source.getUID())) return false;
    auto p=vertexes[source.getUID()];
    source.Dgis=p.Dgis;
    source.Name=p.Name;
    return AddWay(source,target,lenght);
}

bool Rgraph::DeleteWay(Cross source, Cross target)
{
    if (!verify(source) ||!verify(target) ) return false;
    vertexes[source.getUID()].ways.remove(target.getUID());
    return true;
}

bool Rgraph::DeleteWayToPoint(Cross source, int number)
{
    Cross target=Cross(Region,0,number,"","");
    if (!vertexes.contains(target.getUID())) return false;
    return DeleteWay(source,target);
}

bool Rgraph::DeleteWayFromPoint(int number, Cross target)
{
    Cross source=Cross(Region,0,number,"","");
    if (!vertexes.contains(source.getUID())) return false;
    return DeleteWay(source,target);
}

QList<Vertex> Rgraph::ReadAllVertexes()
{
    QList<Vertex> list;
    foreach (auto v, vertexes) {
        if (v.Area!=0) {
            list.append(v);
        }
    }
    return list;
}

QList<Vertex> Rgraph::ReadAllPoints()
{
    QList<Vertex> list;
    foreach (auto v, vertexes) {
        if (v.Area==0) {
            list.append(v);
        }
    }
    return list;
}

QList<Way> Rgraph::GetWaysFromCross(Cross cross)
{
    QList<Way> list;
    if (!verify(cross)) return list;
    auto v=vertexes[cross.getUID()];
    foreach(auto w,v.ways){
        list.append(w);
    }
    return list;
}

QList<Way> Rgraph::GetWaysFromPoint(int number)
{
    QList<Way> list;
    auto cross=Cross(Region,0,number,"","");
    if (!verify(cross)) return list;
    auto v=vertexes[cross.getUID()];
    foreach(auto w,v.ways){
        list.append(w);
    }
    return list;
}

bool Rgraph::verify(Cross cross)
{
    if (cross.Region!=Region) return false;
    return vertexes.contains(cross.getUID());
}

DBConnect::DBConnect()
{

}
