////////////////////////////////////////////////////////////////////////////////
/// Includes
////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "GridWalker/GridWalker.hpp"
#include "Timer/MeasureExecutionTimer.hpp"

int main()
{
    gw::Grid grid{ 5, 7 };
    
    {
        auto FunctionToMeasure 
        {
            [&]()
            {
                gw::GridWalker gridWalker{ grid };
                gridWalker.StartWalkingPathsMaxThreaded( gw::Path{ grid }, gw::Coordinate{ 0, 0 } );
            }
        };
        
        xtm::MeasureExecutionTimer extimer{ FunctionToMeasure };
    }
    {
        auto FunctionToMeasure 
        {
            [&]()
            {
                gw::GridWalker gridWalker{ grid };
                gridWalker.StartWalkingPathsDoubleThreaded( gw::Path{ grid }, gw::Coordinate{ 0, 0 } );
            }
        };
        
        xtm::MeasureExecutionTimer extimer{ FunctionToMeasure };
    }
    {
        auto FunctionToMeasure 
        {
            [&]()
            {
                gw::GridWalker gridWalker{ grid };
                gridWalker.WalkPaths( gw::Path{ grid }, gw::Coordinate{ 0, 0 } );
            }
        };
        xtm::MeasureExecutionTimer extimer{ FunctionToMeasure };
    }
}