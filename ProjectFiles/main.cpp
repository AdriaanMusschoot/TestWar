////////////////////////////////////////////////////////////////////////////////
/// Includes
////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "GridWalker/GridWalker.hpp"
#include "GridWalker/GridWalkerCRTP.hpp"
#include "benchmark/benchmark.h"
#include "Timer/MeasureExecutionTimer.hpp"

constexpr int rows{ 6 };
constexpr int columns{ rows };
constexpr int iterations{ 1 };

template< typename T >
concept GridWalkerInheritance = std::is_base_of_v< gw::GridWalkerBase, T >;

template< GridWalkerInheritance T, typename... Args > 
void BM_WalkGrid_Inheritance( benchmark::State& state, Args&&... args )
{
    const gw::Grid grid{ rows, columns };

    using GridWalker = std::unique_ptr< gw::GridWalkerBase >;
    GridWalker gridWalker{ std::make_unique< T >( grid, std::forward< Args >( args )... ) };

    for( auto _ : state )
    {
        gridWalker->WalkPaths( gw::Path{ grid }, gw::Coordinate{ 0, 0 } );
    }
    
    benchmark::DoNotOptimize( gridWalker );
    state.SetItemsProcessed( state.iterations() );
}

template< typename T >
concept GridWalkerCRTP = std::is_base_of_v< gw::CRTPGridWalkerBase< T >, T >;

template< GridWalkerCRTP T, typename... Args > 
void BM_WalkGrid_CRTP( benchmark::State& state, Args&&... args )
{
    const gw::Grid grid{ rows, columns };

    using GridWalker = std::unique_ptr< gw::CRTPGridWalkerBase< T > >;
    GridWalker gridWalker{ std::make_unique< T >( grid, std::forward< Args >( args )... ) };

    for( auto _ : state )
    {
        gridWalker->WalkPaths( gw::Path{ grid }, gw::Coordinate{ 0, 0 } );
    }
    
    benchmark::DoNotOptimize( gridWalker );
    state.SetItemsProcessed( state.iterations() );
}

// BENCHMARK( []( benchmark::State& state ){ BM_WalkGrid_Inheritance< gw::RecursiveGridWalker >( state ); } )->Iterations( iterations );
// BENCHMARK( []( benchmark::State& state ){ BM_WalkGrid_CRTP< gw::CRTPRecursiveGridWalker >( state ); } )->Iterations( iterations );

BENCHMARK( []( benchmark::State& state ){ BM_WalkGrid_Inheritance< gw::StartThreadedGridWalker >( state ); } )->Iterations( iterations );
// BENCHMARK( []( benchmark::State& state ){ BM_WalkGrid_Inheritance< gw::ThreadsAtLayerGridWalker >( state , 2 ); } )->Iterations( iterations );
// BENCHMARK( []( benchmark::State& state ){ BM_WalkGrid_Inheritance< gw::ThreadsUntilLayerGridWalker >( state , 2 ); } )->Iterations( iterations );

BENCHMARK_MAIN();