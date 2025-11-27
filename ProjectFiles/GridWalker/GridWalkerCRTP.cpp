////////////////////////////////////////////////////////////////////////////////
/// Includes
////////////////////////////////////////////////////////////////////////////////

#include "GridWalkerCRTP.hpp"
#include "GridWalkerLib.hpp"

#include <cassert>
#include <algorithm>
#include <future>

////////////////////////////////////////////////////////////////////////////////
/// GridWalker
////////////////////////////////////////////////////////////////////////////////

gw::CRTPRecursiveGridWalker::CRTPRecursiveGridWalker( const Grid& gridToWalk )
    : CRTPGridWalkerBase( gridToWalk )
{
}

gw::CRTPRecursiveGridWalker::~CRTPRecursiveGridWalker()
{
    // need to call threads.join 
    // otherwise threads only dissapear at the end of this function 
    // and print is called before possible paths is filled up
    // need while loop because not only this thread spawns threads
    // and we want to catch all of them when they are created later
    // do 
    // {
    //     for ( std::jthread& jthread : m_JThreads )
    //     {
    //         jthread.join();
    //     }

    //     m_JThreads.clear();

    // } while ( m_ActiveThreads != 0 );


    // std::cout <<  m_PossiblePaths.size() << " possible paths\n";
    //join threads
}

void gw::CRTPRecursiveGridWalker::WalkPaths( Path pathSoFar, const Coordinate& startCoordinate )
{
    auto&[ currentPathCoordinate, currentPathDirection ] = pathSoFar.AddStep( startCoordinate );
    std::vector< Direction > possibleDirections{ GetPossibleDirections( GetGridReference(), currentPathCoordinate, pathSoFar ) };

    for ( const Direction& possibleDirection: possibleDirections )
    {
        currentPathDirection = possibleDirection;

        Coordinate nextCoordinate{ currentPathCoordinate };
        nextCoordinate.MoveCoordinateByOne( possibleDirection );
        
        WalkPaths( pathSoFar, nextCoordinate );
    }

    if ( pathSoFar.Length() == GetGridReference().Size() )
    {
        AddPossiblePath( pathSoFar );
    }
}

// void gw::RecursiveGridWalker::StartWalkingPathsDoubleThreaded( Path pathSoFar, const Coordinate& startCoordinate )
// {
//     auto& [ currentPathCoordinate, currentPathDirection ] = pathSoFar.AddStep( startCoordinate );
//     std::vector< Direction > possibleDirections{ GetPossibleDirections( m_GridToWalk, currentPathCoordinate, pathSoFar ) };

//     std::vector< std::jthread > jthreads;
//     for ( const Direction& possibleDirection : possibleDirections )
//     {
//         currentPathDirection = possibleDirection;

//         Coordinate nextCoordinate{ currentPathCoordinate };
//         nextCoordinate.MoveCoordinateByOne( possibleDirection );
        
//         jthreads.emplace_back
//         (
//             std::jthread
//             {
//                 [ =, this ]()
//                 {
//                     WalkPaths( pathSoFar, nextCoordinate );
//                 }
//             }
//         );
//     }
// }

// void gw::RecursiveGridWalker::StartWalkingPathsThreadLayer( Path pathSoFar, const Coordinate& startCoordinate, int layer )
// {
//     auto& [ currentPathCoordinate, currentPathDirection ] = pathSoFar.AddStep( startCoordinate );
//     const std::vector< Direction > possibleDirections{ GetPossibleDirections( m_GridToWalk, currentPathCoordinate, pathSoFar ) };

//     for ( const Direction& possibleDirection : possibleDirections )
//     {
//         currentPathDirection = possibleDirection;

//         Coordinate nextCoordinate{ currentPathCoordinate };
//         nextCoordinate.MoveCoordinateByOne( possibleDirection );
        
//         if ( layer == 2 )
//         {
//             if ( std::scoped_lock threadLock{ m_ThreadMutex };
//                  m_JThreads.size() < std::thread::hardware_concurrency() )
//             {
//                 std::cout << "Threat created at layer " << layer << "\n";
//                 m_JThreads.emplace_back
//                 (
//                     std::jthread
//                     {
//                         [ =, this ]()
//                         {
//                             StartWalkingPathsMaxThreaded( pathSoFar, nextCoordinate, layer + 1 );
//                         }
//                     }
//                 );

//                 continue;
//             }
//         }
//         StartWalkingPathsMaxThreaded( pathSoFar, nextCoordinate, layer + 1 );
//     }

//     if ( pathSoFar.Length() == m_GridToWalk.Size() )
//     {
//         std::scoped_lock pathLock{ m_PathMutex };
//         m_PossiblePaths.emplace_back( pathSoFar );
//     }
// }

// void gw::RecursiveGridWalker::StartWalkingPathsMaxThreaded( Path pathSoFar, const Coordinate& startCoordinate, int layer )
// {
//     auto& [ currentPathCoordinate, currentPathDirection ] = pathSoFar.AddStep( startCoordinate );
//     const std::vector< Direction > possibleDirections{ GetPossibleDirections( m_GridToWalk, currentPathCoordinate, pathSoFar ) };

//     for ( const Direction& possibleDirection : possibleDirections )
//     {
//         currentPathDirection = possibleDirection;

//         Coordinate nextCoordinate{ currentPathCoordinate };
//         nextCoordinate.MoveCoordinateByOne( possibleDirection );
        
//         if ( layer <= 5 )
//         {
//             if ( std::scoped_lock activeThreadLock{ m_ActiveThreadsMutex };
//                  m_ActiveThreads <= std::thread::hardware_concurrency() )
//             {
//                 std::cout << "Threat created at layer " << layer << "\n";
//                 std::cout << "Threat nr " << m_JThreads.size() + 1 << " created\n";
//                 std::scoped_lock threadLock{ m_ThreadMutex };
                
//                 m_JThreads.emplace_back
//                 (
//                     std::jthread
//                     {
//                         [ =, this ]()
//                         {
//                             StartWalkingPathsThreadLayer( pathSoFar, nextCoordinate, layer + 1 );
//                             {
//                                 std::scoped_lock activeThreadLock{ m_ActiveThreadsMutex };
//                                 --m_ActiveThreads;
//                             }
//                         }
//                     }
//                 );
//                 ++m_ActiveThreads;

//                 continue;
//             }
//         }
//         StartWalkingPathsThreadLayer( pathSoFar, nextCoordinate, layer + 1 );
//     }

//     if ( pathSoFar.Length() == m_GridToWalk.Size() )
//     {
//         std::scoped_lock pathLock{ m_PathMutex };
//         m_PossiblePaths.emplace_back( pathSoFar );
//     }
// }