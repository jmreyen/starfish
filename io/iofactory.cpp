#include "io/iofactory.h"

IOFactory *IOFactory::instance = 0;

IOFactory *IOFactory::Instance()
{
    if(!instance)
        instance = new IOFactory;
    return instance;
}


bool IOFactory::RegisterIO(QString name, CreateIO createFunc)
{
    callbacks.insert(name, createFunc) != callbacks.end();
    return true;
}

bool IOFactory::UnregisterIO(QString name)
{
    callbacks.remove(name);
    return true;
}

AbstractIO *IOFactory::getIO(QString name)
{
    CallbackMap::const_iterator itr = callbacks.find(name);
    if (itr == callbacks.end())
        return 0;
    else
        return (itr.value())();
}
