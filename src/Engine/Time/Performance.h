#pragma once
#ifndef CHECKPERF_H
#define CHECKPERF_H
#include <Engine/Core.h>
#include <chrono>
#include <ratio>
#include <thread>
#include <string>
#include <map>
#include <iostream>

typedef std::map<std::string, 
    std::chrono::time_point<std::chrono::high_resolution_clock>> ClockMap;

class  Performance {
public:    
    static void Start(const std::string& testName = "")
    {
        m_tests[testName] = std::chrono::high_resolution_clock::now();
    }

    static void End(const std::string& testName = "")
    {
        try {
            auto endTime = std::chrono::high_resolution_clock::now();
            auto startTime = m_tests.at(testName);
            std::chrono::duration<double, std::milli> ms = endTime - startTime;
            std::cout << "[Performance: " << testName << "] took " <<
                ms.count() << " ms\n";
            Performance::m_tests.erase(testName);
        } catch(std::out_of_range e) {
            std::cerr << "Performance check error! Test with name \"" << 
                testName << "\" could not be found.\n";
        }
    }
    
private:
    static ClockMap m_tests;

};

#endif
