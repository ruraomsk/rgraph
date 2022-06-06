#include "vertex.h"


Vertex::Vertex()
{

}

Vertex::Vertex(Cross cross)
{
    Region=cross.Region;
    Area=cross.Area;
    ID=cross.ID;
    Dgis=cross.Dgis;
    Name=cross.Name;
    Scale=cross.Scale;
}

Vertex::Vertex(QString state)
{
    QJsonParseError jerror;
    QJsonDocument jdoc=QJsonDocument::fromJson(state.toUtf8(),&jerror);
    if (jerror.error!=QJsonParseError::NoError) return;
    auto map=jdoc.toVariant().toMap();
    Region=map["region"].toInt();
    Area=map["area"].toInt();
    ID=map["id"].toInt();
    Scale=map["scale"].toFloat();
    Dgis=map["dgis"].toString();
    Name=map["name"].toString();
}

Vertex::Vertex(int region, int area, int id, QString dgis, QString name)
{
    Region=region;
    Area=area;
    ID=id;
    Dgis=dgis;
    Name=name;
    Scale=1.0;
}

const char *Vertex::Marshal()
{
    QJsonObject map;
    map["region"]=Region;
    map["area"]=Area;
    map["id"]=ID;
    map["scale"]=Scale;
    map["dgis"]=Dgis;
    map["name"]=Name;
    vstr=QJsonDocument(map).toJson();
    return vstr.constData();
}

long Vertex::getUID()
{
    return ((long)Region<<32)|((long)Area<<16)|(long)ID;
}
