#include "way.h"

Way::Way()
{

}

Way::Way(QString way)
{
    QJsonParseError jerror;
    QJsonDocument jdoc=QJsonDocument::fromJson(way.toUtf8(),&jerror);
    if (jerror.error!=QJsonParseError::NoError) return;
    auto map=jdoc.toVariant().toMap();
    Region=map["region"].toInt();
    Source=map["source"].toLongLong();
    Target=map["target"].toLongLong();
    LineSource=map["lsource"].toInt();
    LineTarget=map["ltarget"].toInt();
    Start=map["starts"].toString();
    Stop=map["stops"].toString();
    Lenght=map["lenght"].toInt();
    if (Lenght<=0) Lenght=1;
    Time=map["time"].toInt();
}

const char *Way::Marshal()
{
    QJsonObject map;
    map["region"]=Region;
    map["source"]=(long long)Source;
    map["target"]=(long long)Target;
    map["lsource"]=LineSource;
    map["target"]=LineTarget;
    map["starts"]=Start;
    map["stops"]=Stop;
    map["lenght"]=Lenght;
    map["time"]=Time;
    wstr=QJsonDocument(map).toJson();
    return wstr.constData();
}

int Way::GetSourceArea()
{
    return (int)(Source>>16)&0xffff;
}

int Way::GetSourceID()
{
    return (int)(Source)&0xffff;
}
int Way::GetTargetArea()
{
    return (int)(Target>>16)&0xffff;
}

int Way::GetTargetID()
{
    return (int)(Target)&0xffff;
}

const char *Way::SourceToString()
{
    wstr=QString::asprintf("[%d %d %d]",Region,(int)(Source>>16)&0xffff,(int)(Source)&0xffff).toLocal8Bit();
    return wstr.constData();
}

const char *Way::TargetToString()
{
    wstr=QString::asprintf("[%d %d %d]",Region,(int)(Target>>16)&0xffff,(int)(Target)&0xffff).toLocal8Bit();
    return wstr.constData();
}
