/**
 * Author: Aaron Ishibashi
 * Date: 3/28/2020
 *
 */
#include "Performance.h"
#include <iostream>

namespace SDG
{

SDG::ClockMap SDG::Performance::m_tests = SDG::ClockMap();

inline void
SDG::Performance::Start(const std::string &testName)
{
    m_tests[testName] = std::chrono::high_resolution_clock::now();
}

inline void
SDG::Performance::End(const std::string &testName)
{
    try {
        auto endTime = std::chrono::high_resolution_clock::now();
        auto startTime = m_tests.at(testName);
        std::chrono::duration<double, std::milli> ms = endTime - startTime;
        std::cout << "[Performance: " << testName << "] took " <<
                  ms.count() << " ms\n";
        Performance::m_tests.erase(testName);
    } catch(const std::out_of_range &e) {
        std::cerr << "Performance check error! Test with name \"" <<
                  testName << "\" could not be found.\n";
    }
}

}

