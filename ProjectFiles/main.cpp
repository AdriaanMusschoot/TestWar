////////////////////////////////////////////////////////////////////////////////
/// Includes
////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "GridWalker/GridWalker.hpp"
#include "GridWalker/GridWalkerCRTP.hpp"
#include "benchmark/benchmark.h"

constexpr int rows{ 2 };
constexpr int columns{ rows };

void WalkGridOOP( benchmark::State& state )
{
    const gw::Grid grid{ rows, columns };

    using GridWalker = std::unique_ptr< gw::GridWalkerBase >;
    GridWalker gridWalker{ std::make_unique< gw::RecursiveGridWalker >( grid ) };

    for( auto _ : state )
    {
        gridWalker->WalkPaths( gw::Path{ grid }, gw::Coordinate{ 0, 0 } );
    }
    
    benchmark::DoNotOptimize( gridWalker );
    state.SetItemsProcessed( state.iterations() );
}


void WalkGridCRTP( benchmark::State& state )
{
    const gw::Grid grid{ rows, columns };

    using GridWalker = std::unique_ptr< gw::CRTPGridWalkerBase< gw::CRTPRecursiveGridWalker > >;
    GridWalker gridWalker{ std::make_unique< gw::CRTPRecursiveGridWalker >( grid ) };

    for( auto _ : state )
    {
        gridWalker->WalkPaths( gw::Path{ grid }, gw::Coordinate{ 0, 0 } );
    }
    
    benchmark::DoNotOptimize( gridWalker );
    state.SetItemsProcessed( state.iterations() );
}

constexpr int iterations{ 1000000 };
BENCHMARK( WalkGridOOP )->Iterations( iterations );
BENCHMARK( WalkGridCRTP )->Iterations( iterations );

BENCHMARK_MAIN();