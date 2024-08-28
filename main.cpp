#include <functional>
#include <iostream>
#include <string>
#include <synchapi.h>
#include <vector>
#include <windows.h>
#include "logger.hpp"
#include "detector.hpp"


int main() {
    std::cout << " ______            _                _      _            _             " << std::endl <<
" \\~~~~/  __      _(_)_ __   ___  __| | ___| |_ ___  ___| |_ ___  _ __ " << std::endl <<
"  \\  /   \\ \\ /\\ / / | '_ \\ / _ \\/ _` |/ _ \\ __/ _ \\/ __| __/ _ \\| '__|" << std::endl <<
"   []     \\ V  V /| | | | |  __/ (_| |  __/ ||  __/ (__| || (_) | | " << std::endl <<  
"   []      \\_/\\_/ |_|_| |_|\\___|\\__,_|\\___|\\__\\___|\\___|\\__\\___/|_| " << std::endl <<  
"  ----                                                      " << std::endl <<
"    WINEDETECTOR                V0.2.0       \n" << std::endl;

    Logger logger;
    Detector detector;
    
    std::vector<std::function<Detect()>> functptr = {
        [&detector]() { return detector.drivesTest(); },
        [&detector]() { return detector.registryTest(); },
        [&detector]() { return detector.servicesTest(); },
        [&detector]() { return detector.processTest(); },
        [&detector]() { return detector.filesTest(); },
        // [&detector]() { return detector.muldivTest(); },
        [&detector]() { return detector.dllExportTest(); }
    };
    for (int i = 0; i < functptr.size(); i++) {
        Detect detect = (functptr[i])();
        logger.log("[~] Tested " + (detect.name) +". Result: ");

        if (detect.detected) {
            logger.error("DETECTED\n");
        }
        else {
            logger.success("PASSED\n");
        }

        Sleep(500);
    
    }

    logger.log("\n");

    logger.warning("[+] Done! Test points: "+ std::to_string(detector.getScore()) + "/" + std::to_string(detector.getTotalScore()) + "\n");

    Sleep(5000);

    return 0;
}