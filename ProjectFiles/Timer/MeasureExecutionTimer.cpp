////////////////////////////////////////////////////////////////////////////////
/// Include
////////////////////////////////////////////////////////////////////////////////

#include "MeasureExecutionTimer.hpp"

#include <iostream> 

////////////////////////////////////////////////////////////////////////////////
/// MeasureExecutionTimer
////////////////////////////////////////////////////////////////////////////////

xtm::MeasureExecutionTimer::MeasureExecutionTimer( std::function< void() > functionToMeasure )
    : m_StartTime{ std::chrono::system_clock::now() }
{
    functionToMeasure();
}

xtm::MeasureExecutionTimer::~MeasureExecutionTimer()
{
    auto endTime{ std::chrono::system_clock::now() };
    std::chrono::duration< double > timePassed{ endTime - m_StartTime };

    std::cout << timePassed.count() << " seconds\n";
}
