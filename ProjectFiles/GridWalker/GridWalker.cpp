////////////////////////////////////////////////////////////////////////////////
/// Includes
////////////////////////////////////////////////////////////////////////////////

#include "GridWalker.hpp"

#include <cassert>
#include <algorithm>
#include <future>

////////////////////////////////////////////////////////////////////////////////
/// GridWalker
////////////////////////////////////////////////////////////////////////////////

gw::GridWalker::GridWalker( const Grid& gridToWalk )
    : m_GridToWalk{ gridToWalk }
{
    m_JThreads.reserve( std::thread::hardware_concurrency() );
}

gw::GridWalker::~GridWalker()
{
    // need to call threads.join 
    // otherwise threads only dissapear at the end of this function 
    // and print is called before possible paths is filled up
    for ( std::jthread& jthread : m_JThreads )
    {
        jthread.join();
    }


    std::cout <<  m_PossiblePaths.size() << " possible paths\n";
    //join threads
}

void gw::GridWalker::WalkPaths( Path pathSoFar, const Coordinate& startCoordinate )
{
    auto&[ currentPathCoordinate, currentPathDirection ] = pathSoFar.AddStep( startCoordinate );
    std::vector< Direction > possibleDirections{ GetPossibleDirections( m_GridToWalk, currentPathCoordinate, pathSoFar ) };

    for ( const Direction& possibleDirection: possibleDirections )
    {
        currentPathDirection = possibleDirection;

        Coordinate nextCoordinate{ currentPathCoordinate };
        nextCoordinate.MoveCoordinateByOne( possibleDirection );
        
        WalkPaths( pathSoFar, nextCoordinate );
    }

    if ( pathSoFar.Length() == m_GridToWalk.Size() )
    {
        std::scoped_lock pathLock{ m_PathMutex };
        m_PossiblePaths.emplace_back( pathSoFar );
    }
}

void gw::GridWalker::StartWalkingPathsDoubleThreaded( Path pathSoFar, const Coordinate& startCoordinate )
{
    auto& [ currentPathCoordinate, currentPathDirection ] = pathSoFar.AddStep( startCoordinate );
    std::vector< Direction > possibleDirections{ GetPossibleDirections( m_GridToWalk, currentPathCoordinate, pathSoFar ) };

    std::vector< std::jthread > jthreads;
    for ( const Direction& possibleDirection : possibleDirections )
    {
        currentPathDirection = possibleDirection;

        Coordinate nextCoordinate{ currentPathCoordinate };
        nextCoordinate.MoveCoordinateByOne( possibleDirection );
        
        jthreads.emplace_back
        (
            std::jthread
            {
                [ =, this ]()
                {
                    WalkPaths( pathSoFar, nextCoordinate );
                }
            }
        );
    }
}

void gw::GridWalker::StartWalkingPathsMaxThreaded( Path pathSoFar, const Coordinate& startCoordinate, int layer )
{
    auto& [ currentPathCoordinate, currentPathDirection ] = pathSoFar.AddStep( startCoordinate );
    const std::vector< Direction > possibleDirections{ GetPossibleDirections( m_GridToWalk, currentPathCoordinate, pathSoFar ) };

    for ( const Direction& possibleDirection : possibleDirections )
    {
        currentPathDirection = possibleDirection;

        Coordinate nextCoordinate{ currentPathCoordinate };
        nextCoordinate.MoveCoordinateByOne( possibleDirection );
        
        if ( layer == 2 )
        {
            if ( std::scoped_lock threadLock{ m_ThreadMutex };
                 m_JThreads.size() < std::thread::hardware_concurrency() )
            {
                std::cout << "Threat created at layer " << layer << "\n";
                m_JThreads.emplace_back
                (
                    std::jthread
                    {
                        [ =, this ]()
                        {
                            StartWalkingPathsMaxThreaded( pathSoFar, nextCoordinate, layer + 1 );
                        }
                    }
                );

                continue;
            }
        }
        StartWalkingPathsMaxThreaded( pathSoFar, nextCoordinate, layer + 1 );
    }

    if ( pathSoFar.Length() == m_GridToWalk.Size() )
    {
        std::scoped_lock pathLock{ m_PathMutex };
        m_PossiblePaths.emplace_back( pathSoFar );
    }
}

std::vector<gw::Direction> gw::GridWalker::GetPossibleDirections(const Grid &grid, const Coordinate &currentCoordinate, Path &walkedPath)
{
    std::vector< Direction > possibleDirections{};
    possibleDirections.reserve( 4 );

    if ( IsDirectionPossible( grid, Direction::Up, currentCoordinate, walkedPath ) )
    {
        possibleDirections.emplace_back( Direction::Up );
    }

    if ( IsDirectionPossible( grid, Direction::Down, currentCoordinate, walkedPath ) )
    {
        possibleDirections.emplace_back( Direction::Down );
    }

    if ( IsDirectionPossible( grid, Direction::Left, currentCoordinate, walkedPath ) )
    {
        possibleDirections.emplace_back( Direction::Left );
    }

    if ( IsDirectionPossible( grid, Direction::Right, currentCoordinate, walkedPath ) )
    {
        possibleDirections.emplace_back( Direction::Right );
    }

    return possibleDirections;
}

bool gw::GridWalker::IsDirectionPossible( const Grid& grid, const Direction& direction, const Coordinate& currentCoordinate, Path& walkedPath )
{
    Coordinate coordinateToTry{ currentCoordinate };
    coordinateToTry.MoveCoordinateByOne( direction );

    return not walkedPath.AlreadyVisited( coordinateToTry ) && grid.IsWithinGridBounds( coordinateToTry );
}