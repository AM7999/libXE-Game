#ifndef CONFIG_H
#define CONFIG_H

#include <string>


namespace Xenia {
    class Config {
        public:
            static bool getBoolValue(std::string root, std::string key);

            static bool fullscreen();
            static bool modLoading();
            static bool compression();
            static bool autoSave();

            static bool vsync();

            static int autoSaveInterval();

            static int volM();
            static int volMu();
            static int volSfx();

            static bool debug();
            static bool verboseLogging();
            static std::string logFile();
            static bool showFps();
};
}



#endif
