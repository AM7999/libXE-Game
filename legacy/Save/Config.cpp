#include "Config.hpp"

#include "../../extern/iniparser/src/iniparser.h"
#include <string>

bool Config::fullscreen() {
    dictionary *d = iniparser_load("config.ini");
    bool val = iniparser_getboolean(d, "Game:Fullscreen", 100);
    iniparser_freedict(d);

    return val;
}

bool Config::modLoading() {
    dictionary *d = iniparser_load("config.ini");
    bool val = iniparser_getboolean(d, "Game:ModLoading", 100);
    iniparser_freedict(d);

    return val;
}

bool Config::compression() {
    dictionary *d = iniparser_load("config.ini");
    bool val = iniparser_getboolean(d, "Game:Compression", 100);
    iniparser_freedict(d);

    return val;
}

bool Config::autoSave() {
    dictionary *d = iniparser_load("config.ini");
    bool val = iniparser_getboolean(d, "Game:AutoSave", 100);
    iniparser_freedict(d);

    return val;
}

int Config::autoSaveInterval() {
    dictionary *d = iniparser_load("config.ini");
    int val = iniparser_getint(d, "Game:AutoSaveInterval", 100);
    iniparser_freedict(d);

    return val;
}

bool Config::vsync() {
    dictionary *d = iniparser_load("config.ini");
    bool val = iniparser_getboolean(d, "Game:Vsync", 100);
    iniparser_freedict(d);

    return val;
}

bool Config::debug() {
    dictionary *d = iniparser_load("config.ini");
    bool val = iniparser_getboolean(d, "Debug:Debug", 100);
    iniparser_freedict(d);

    return val;
}