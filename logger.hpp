#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <iostream>

class Logger {
public:
    void error(std::string message);
    void success(std::string message);
    void warning(std::string message);
    void log(std::string message);
private:
    void rawlog(std::string message) {
        std::cout << message;
    }
};

#endif // LOGGER_HPP
