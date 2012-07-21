#ifndef IOFACTORY_H
#define IOFACTORY_H

#include "abstractio.h"

#include <QMap>

class IOFactory
{
public:
    typedef AbstractIO* (*CreateIO)();
    static IOFactory *Instance();
    bool RegisterIO(QString name, CreateIO createFunc);
    bool UnregisterIO(QString name);
    AbstractIO *getIO(QString name);
private:
    typedef QMap<QString, CreateIO> CallbackMap;
    CallbackMap callbacks;
    IOFactory(){}
    IOFactory(const IOFactory&){}
    static IOFactory *instance;
};

#endif // IOFACTORY_H
