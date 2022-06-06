#ifndef WAY_H
#define WAY_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QVariant>
#include <QMap>

#include "cross.h"
class Way
{
public:
    Way();
    Way (QString way);
    const char *Marshal();
    int GetSourceArea();
    int GetSourceID();
    int GetTargetArea();
    int GetTargetID();

    const char *SourceToString();
    const char *TargetToString();

    int Region;
    long Source;
    long Target;
    QString Start;
    QString Stop;
    int Lenght;
    int Time=0;
    QByteArray wstr;
};

#endif // WAY_H
