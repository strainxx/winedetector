#include "logger.hpp"
#include <windows.h>


void setConsoleColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void Logger::error(std::string message){
    setConsoleColor(FOREGROUND_RED);
    rawlog(message);
    setConsoleColor(7);
}

void Logger::success(std::string message){
    setConsoleColor(FOREGROUND_GREEN);
    rawlog(message);
    setConsoleColor(7);
}

void Logger::warning(std::string message){
    setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    rawlog(message);
    setConsoleColor(7);
}

void Logger::log(std::string message){
    rawlog(message);
}