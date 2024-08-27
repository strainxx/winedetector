#include <iostream>
#include <windows.h>
#include "logger.hpp"
#include "detector.hpp"

int main() {
    std::cout << "                                                   .''''." << std::endl << 
"                                                  /,.--. )" << std::endl << 
"                             .'``.        __   __((\\- -(\\)" << std::endl << 
"                    _______.'     \\_.-''''  ``'  /)) - .\\|" << std::endl << 
"   __....::::::::::'''''''/    .   \'''''''::::::(/ `-'`.)" << std::endl << 
".:'::.  .  o ~ .  ~  o ~ /    /     '.o ~ . _.....--- `  \\" << std::endl << 
" ':. ':::::.___.____,___/   ,'_\\      \\ _.-'___..___..._,'" << std::endl << 
"   ':.  o~  `::::::::::::::::::::::::::::::::::::::::'  (\\" << std::endl << 
"    `':.  o ~  o   ..   o  ,  ~  \\ . o~   -.  ~   .'.:'\'(" << std::endl << 
"       ':.  ,..   o  ~   o  . ,  'o.    ~ o   ~ o'.:'  \\(/" << std::endl << 
"         '.   o   ~   .    ~    o ~ ',o :  :  .' .' ('\\/ |" << std::endl << 
"           '-._    ~    o  , o  ,  .  ~._ _.o_.-'  \\/ ) (" << std::endl << 
"               '- .._  .    ~    ~      _.. -'" << std::endl << 
"                     ''' - .,.,. - '''" << std::endl << 
"                          (:' .:)" << std::endl << 
"                           :| '|" << std::endl << 
"                           |. ||" << std::endl << 
"                           || :|" << std::endl << 
"                           :| |'" << std::endl << 
"                           || :|" << std::endl << 
"                           '| ||" << std::endl << 
"                           |: ':" << std::endl << 
"                           || :|" << std::endl << 
"                     __..--:| |'--..__" << std::endl << 
"               _...-'  _.' |' :| '.__ '-..._" << std::endl << 
"             / -  ..---    '''''   ---...  _ \\" << std::endl << 
"             \\  _____  ..--   --..     ____  /" << std::endl << 
"              '-----....._________.....-----'" << std::endl <<
"    WINEDETECTOR                V0.1.0       " << std::endl;

    Logger logger;
    Detector detector;
    int stage1Detects = 0;
    logger.success("\n\n[+] Stage 1 started\n");
    Sleep(1000);
    logger.log("[~] Registry test ");
    Detect registryTest = detector.registryTest();
    if(registryTest.detected) {
        logger.error("DETECTED\n");
        stage1Detects+= 1;
    } else {
        logger.success("PASSED\n");
    }
    Sleep(500);
    logger.log("[~] Drives test ");
    Detect drivesTest = detector.drivesTest();
    if(drivesTest.detected) {
        logger.error("DETECTED\n");
        stage1Detects+= 1;
    } else {
        logger.success("PASSED\n");
    }
    Sleep(500);
    logger.log("[~] Files test ");
    Detect filesTest = detector.filesTest();
    if(filesTest.detected) {
        logger.error("DETECTED\n");
        stage1Detects+= 1;
    } else {
        logger.success("PASSED\n");
    }

    
    if(stage1Detects >= 1) {
        logger.error("[+] Stage 1 failed (" + std::to_string(stage1Detects) + "/3)" +"\n");
    } else {
        logger.success("[+] Stage 1 passed\n");
    }
    // logger.log("\n");

    int stage2Detects = 0;
    logger.success("\n\n[+] Stage 2 started\n");
    Sleep(1000);
    logger.log("[~] Services test ");
    Detect servicesTest = detector.servicesTest();
    if(servicesTest.detected) {
        logger.error("DETECTED\n");
        stage2Detects+= 1;
    } else {
        logger.success("PASSED\n");
    }
    Sleep(500);

    logger.log("[~] Process test ");
    Detect processTest = detector.processTest();
    if(processTest.detected) {
        logger.error("DETECTED\n");
        stage2Detects+= 1;
    } else {
        logger.success("PASSED\n");
    }
    Sleep(500);
    if(stage2Detects >= 1) {
        logger.error("[+] Stage 2 failed (" + std::to_string(stage2Detects) + "/2)" +"\n");
    } else {
        logger.success("[+] Stage 2 passed\n");
    }
    

    return 0;
}