#include "Config.hpp"

#include "../../../extern/iniparser/src/iniparser.h"
#include <string>

bool Xenia::Config::getBoolValue(std::string root, std::string key) {
    dictionary *d = iniparser_load("config.ini");
    std::string fKey = root + ":" + key;
    bool val = iniparser_getboolean(d, fKey.c_str(), 100);
    delete d;
    return val;
}

// bool Xenia::Config::fullscreen() {
//     dictionary *d = iniparser_load("config.ini");
//     bool val = iniparser_getboolean(d, "Game:Fullscreen", 100);
//     iniparser_freedict(d);

//     return val;
// }

// bool Xenia::Config::modLoading() {
//     dictionary *d = iniparser_load("config.ini");
//     bool val = iniparser_getboolean(d, "Game:ModLoading", 100);
//     iniparser_freedict(d);

//     return val;
// }

// bool Xenia::Config::compression() {
//     dictionary *d = iniparser_load("config.ini");
//     bool val = iniparser_getboolean(d, "Game:Compression", 100);
//     iniparser_freedict(d);

//     return val;
// }

// bool Xenia::Config::autoSave() {
//     dictionary *d = iniparser_load("config.ini");
//     bool val = iniparser_getboolean(d, "Game:AutoSave", 100);
//     iniparser_freedict(d);

//     return val;
// }

// int Xenia::Config::autoSaveInterval() {
//     dictionary *d = iniparser_load("config.ini");
//     int val = iniparser_getint(d, "Game:AutoSaveInterval", 100);
//     iniparser_freedict(d);

//     return val;
// }

// bool Xenia::Config::vsync() {
//     dictionary *d = iniparser_load("config.ini");
//     bool val = iniparser_getboolean(d, "Game:Vsync", 100);
//     iniparser_freedict(d);

//     return val;
// }

// bool Xenia::Config::debug() {
//     dictionary *d = iniparser_load("config.ini");
//     bool val = iniparser_getboolean(d, "Debug:Debug", 100);
//     iniparser_freedict(d);

//     return val;
// }
