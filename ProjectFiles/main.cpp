////////////////////////////////////////////////////////////////////////////////
/// Includes
////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "GridWalker/GridWalker.hpp"
#include "Timer/MeasureExecutionTimer.hpp"

int main()
{
    gw::Grid grid{ 6, 9 };
    
    {
        gw::GridWalker gridWalker{ grid };
        auto FunctionToMeasure{ std::bind( &gw::GridWalker::WalkPaths, &gridWalker, gw::Path{ grid }, gw::Coordinate{ 0, 0 } ) };
        xtm::MeasureExecutionTimer extimer{ FunctionToMeasure };
    }
    {
        gw::GridWalker gridWalker{ grid };
        auto FunctionToMeasure{ std::bind( &gw::GridWalker::StartWalkingPathsThreaded, &gridWalker, gw::Path{ grid }, gw::Coordinate{ 0, 0 } ) };
        xtm::MeasureExecutionTimer extimer{ FunctionToMeasure };
    }
}