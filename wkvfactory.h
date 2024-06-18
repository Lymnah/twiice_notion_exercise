#ifndef WKVFACTORY_H
#define WKVFACTORY_H

#include "wkv.h"
#include <memory>

class WKVFactory
{
public:
    static std::unique_ptr<WKV> createSensor(const std::string &type);
};

#endif // WKVFACTORY_H
