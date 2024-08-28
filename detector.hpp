#ifndef DETECTOR_HPP
#define DETECTOR_HPP

#include <string>

struct Detect{
    std::string name;
    int score;
    bool detected = false;
};

class Detector {
public:
    Detect registryTest();
    
    Detect drivesTest();

    Detect servicesTest();

    Detect processTest();

    Detect filesTest();

    Detect muldivTest();

    Detect dllExportTest();

    int getTotalScore() { return totalScore; }
    int getScore() { return score; }
private:
    int totalScore = 0;
    int score = 0;
};

#endif // DETECTOR_HPP
