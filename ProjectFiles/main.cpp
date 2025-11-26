////////////////////////////////////////////////////////////////////////////////
/// Includes
////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "GridWalker/GridWalker.hpp"
#include "Timer/MeasureExecutionTimer.hpp"
#include "Recursion/DecimalToBinary.hpp"

int main()
{
    std::cout << rec::DecToBin{}( 1290826745 ) << "\n";

    std::cout << rec::BinToDec{}( std::string{ "1001111111001110000011110011001" } ) << "\n";
}