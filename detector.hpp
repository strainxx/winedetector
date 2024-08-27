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
};

#endif // DETECTOR_HPP
