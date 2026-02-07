#ifndef LOGGER_H
#define LOGGER_H

#include <string>

namespace Xenia {
    class Logger {
        public:
            static void logMessage(std::string message);
            static void logError(std::string message, int eType);
            static void logWarning(std::string message);

            static int getLogCt();
        private:
            static int logCt;
    };
}

#endif
