////////////////////////////////////////////////////////////////////////////////
/// Includes
////////////////////////////////////////////////////////////////////////////////

#include "GridWalker.hpp"

#include <cassert>
#include <algorithm>
#include <thread>
#include <future>

////////////////////////////////////////////////////////////////////////////////
/// GridWalker
////////////////////////////////////////////////////////////////////////////////

gw::GridWalker::GridWalker(  Grid& gridToWalk )
    : m_GridToWalk{ gridToWalk }
    , m_PossiblePaths{}
{
}

gw::GridWalker::~GridWalker()
{
    std::cout << m_PossiblePaths.size() << " possible paths\n";
}

void gw::GridWalker::WalkPaths( Path pathSoFar, const Coordinate& startCoordinate )
{
    auto&[ currentPathCoordinate, currentPathDirection ] = pathSoFar.AddStep( startCoordinate );
    std::vector< Direction > possibleDirections{ GetPossibleDirections( m_GridToWalk, currentPathCoordinate, pathSoFar ) };

    for ( const Direction& possibleDirection: possibleDirections )
    {
        Coordinate nextCoordinate{ currentPathCoordinate };
        nextCoordinate.MoveCoordinateByOne( possibleDirection );
        currentPathDirection = possibleDirection;
        WalkPaths( pathSoFar, nextCoordinate );
    }

    if ( pathSoFar.Length() == m_GridToWalk.Size() )
    {
        std::scoped_lock pathLock{ m_PathMutex };
        m_PossiblePaths.emplace_back( pathSoFar );
    }
}

void gw::GridWalker::StartWalkingPathsThreaded( Path pathSoFar, const Coordinate& startCoordinate )
{
    auto&[ currentPathCoordinate, currentPathDirection ] = pathSoFar.AddStep( startCoordinate );
    std::vector< Direction > possibleDirections{ GetPossibleDirections( m_GridToWalk, currentPathCoordinate, pathSoFar ) };

    std::vector< std::jthread > jthreads{};
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

std::vector< gw::Direction > gw::GridWalker::GetPossibleDirections( const Grid& grid, const Coordinate& currentCoordinate, Path& walkedPath )
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