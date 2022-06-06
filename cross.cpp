#include "cross.h"

Cross::Cross(int region, int area, int id, QString dgis, QString name)
{
    Region=region;
    Area=area;
    ID=id;
    Dgis=dgis;
    Name=name;
}

Cross::Cross(QString state)
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

long Cross::getUID()
{
    return ((long)Region<<32)|((long)Area<<16)|(long)ID;
}
