////////////////////////////////////////////////////////////////////////////////
/// Include
////////////////////////////////////////////////////////////////////////////////

#include "MeasureExecutionTimer.hpp"

#include <iostream>
#include <numeric>

////////////////////////////////////////////////////////////////////////////////
/// MeasureExecutionTimer
////////////////////////////////////////////////////////////////////////////////

xtm::MeasureExecutionTimer::MeasureExecutionTimer( std::function< void() > functionToMeasure, int iterations )
    : m_ElapsedTimes{}
{
    
    for ( int idx{}; idx < iterations; ++idx )
    {
        using TimePoint = std::chrono::time_point< std::chrono::system_clock >;
        TimePoint startTime{ std::chrono::system_clock::now() };
        functionToMeasure();
        TimePoint endTime{ std::chrono::system_clock::now() };

        std::chrono::duration< double > duration{ endTime - startTime };
        m_ElapsedTimes.emplace_back( duration.count() );
    }
}

xtm::MeasureExecutionTimer::~MeasureExecutionTimer()
{
    const unsigned int outlierNumber{ static_cast< unsigned int >( m_ElapsedTimes.size() * 0.05 ) };

    std::sort( m_ElapsedTimes.begin(), m_ElapsedTimes.end() );

    const double total{ std::accumulate( m_ElapsedTimes.begin() + outlierNumber, m_ElapsedTimes.end() - outlierNumber, 0.0 ) };
    const double avg{ total / m_ElapsedTimes.size() };

    std::cout << avg << " seconds\n";
}
