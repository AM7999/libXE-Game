#include "Config.hpp"

#include "../../../extern/iniparser/src/iniparser.h"
#include <string>

bool Xenia::Config::getBoolValue(std::string root, std::string key) {
    dictionary *d = iniparser_load("config.ini");
    std::string fKey = root + ":" + key;
    bool val = iniparser_getboolean(d, fKey.c_str(), 100);
    // FIX: must use iniparser_freedict, not delete — iniparser manages its own memory
    iniparser_freedict(d);
    return val;
}
