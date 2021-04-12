#pragma once

#include <chrono>
#include <ratio>
#include <thread>
#include <string>
#include <map>

namespace SDG
{

typedef std::map<std::string,
        std::chrono::time_point<std::chrono::high_resolution_clock>> ClockMap;

class Performance {
public:
    static void Start(const std::string& testName = "");
    static void End(const std::string& testName = "");

private:
    static ClockMap m_tests;
};

}
