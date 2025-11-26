////////////////////////////////////////////////////////////////////////////////
/// Includes
////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "GridWalker/GridWalker.hpp"
#include "Timer/MeasureExecutionTimer.hpp"
#include "Recursion/DecimalToBinary.hpp"

int main()
{
    // std::cout << rec::DecToBin{}( 1290826745 ) << "\n";

    // std::cout << rec::BinToDec{}( std::string{ "1001111111001110000011110011001" } ) << "\n";

    const gw::Grid grid{ 6, 6 };
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
}