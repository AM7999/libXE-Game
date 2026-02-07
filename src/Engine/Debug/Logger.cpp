#include "Logger.hpp"

#include <SDL3/SDL_timer.h>
#include <SDL3/SDL.h>
#include <iostream>
#include <ctime>
#include <stdexcept>

#include <chrono>
#include <iomanip>
#include <sstream>

namespace Xenia {
    int Logger::logCt;

    static std::string current_time_hms() {
        using namespace std::chrono;
        auto now = system_clock::now();
        std::time_t tt = system_clock::to_time_t(now);
        std::tm tm = *std::localtime(&tt);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%H:%M:%S");
        return oss.str();
    }

    void Logger::logMessage(std::string message) {
        logCt++;
        #ifdef VITA
        #endif
        std::cout << "(" << current_time_hms() << ") [LOG]: " << message << "\n";
    }
    void Logger::logWarning(std::string message) {
        logCt++;
        #ifdef VITA
        #endif
        std::cout << "(" << current_time_hms() << ") [WARN]: " << message << "\n";
    }
    void Logger::logError(std::string message, int eType) {
        if(eType == 1) {
            #ifdef VITA
            #endif
            std::cout << "(" << current_time_hms() << ") [CRIT]: " << message << "\n";
            SDL_Delay(3000);
            throw std::runtime_error(message);
        } else if(eType == 0) {
            logCt++;
            #ifdef VITA
            #endif
            std::cout << "(" << current_time_hms() << ") [ERR]: " << message << "\n";
        }
    }

    int Logger::getLogCt() {
        return logCt;
    }
}
