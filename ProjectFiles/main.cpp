////////////////////////////////////////////////////////////////////////////////
/// Includes
////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "GridWalker/GridWalker.hpp"
#include "GridWalker/GridWalkerCRTP.hpp"
#include "Timer/MeasureExecutionTimer.hpp"
#include "Recursion/DecimalToBinary.hpp"

template< typename T, typename...Args > 
void WalkGridTimed( const gw::Grid& grid, Args&&... args )
{
    constexpr int NrRepititions{ 1 };
    constexpr int NrObjects{ 1 };

    using GridWalker = std::unique_ptr< gw::GridWalkerBase >;
    std::vector< GridWalker > gridWalkers{};
    for ( int idx{}; idx < NrObjects; ++idx )
    {
        gridWalkers.emplace_back( std::make_unique< T >( grid, std::forward< Args >( args )... ) );
    }
    
    auto FunctionToMeasure 
    {
        [ & ]()
        {
            for ( GridWalker& gridWalker : gridWalkers )
            {
                gridWalker->WalkPaths( gw::Path{ grid }, gw::Coordinate{ 0, 0 } );
            }
        }
    };
    
    xtm::MeasureExecutionTimer extimer{ FunctionToMeasure, NrRepititions };
    
    gridWalkers.clear();
}

int main()
{
    const gw::Grid grid{ 5, 5 };
    
    //fastest to slowest
    WalkGridTimed< gw::MaxThreadedGridWalker >( grid, 3 );
    WalkGridTimed< gw::LayeredThreadsGridWalker >( grid, 1 );
    WalkGridTimed< gw::DoubleThreadedGridWalker >( grid );
    WalkGridTimed< gw::RecursiveGridWalker >( grid );
}