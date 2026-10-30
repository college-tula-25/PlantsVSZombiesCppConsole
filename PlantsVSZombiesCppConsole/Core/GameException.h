#pragma once
#include <exception>
#include <string>
#include <fstream>
#include <iostream>

class GameException : public std::exception
{
private:
    std::string message;

public:
    explicit GameException(const std::string& msg) : message(msg)
    {
        LogError();
    }

    const char* what() const noexcept override
    {
        return message.c_str();
    }

    void LogError() const
    {
        std::ofstream logFile("crash_log.txt", std::ios::app);
        if (logFile.is_open())
        {
            logFile << "[CRITICAL ERROR]: " << message << "\n";
            logFile.close();
        }
    }
};
