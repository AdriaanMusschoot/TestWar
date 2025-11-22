////////////////////////////////////////////////////////////////////////////////
/// Includes
////////////////////////////////////////////////////////////////////////////////

#include "GridWalker.hpp"

#include <cassert>
#include <algorithm>

////////////////////////////////////////////////////////////////////////////////
/// GridWalker
////////////////////////////////////////////////////////////////////////////////

GridWalker::GridWalker( int rows, int columns, const Coordinate& startCoordinate  )
    : m_GridToWalk{ rows, columns }
    , m_PossiblePaths{}
{
    assert( startCoordinate.m_RowIndex < rows );
    assert( startCoordinate.m_ColumnIndex < columns );

    WalkPaths( Path{ m_GridToWalk }, startCoordinate );

    std::cout << "Possible Paths: " << m_PossiblePaths.size() << "\n";
}

GridWalker::~GridWalker()
{
    std::cout << "\n\nGrid Walker Destroyed";
}

void GridWalker::WalkPaths( Path pathSoFar, const Coordinate& currentCoordinate )
{
    auto&[ currentPathCoordinate, currentPathDirection ] = pathSoFar.AddStep( currentCoordinate );
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
        m_PossiblePaths.emplace_back( pathSoFar );
        std::cout << pathSoFar << "\n";
    }
}

std::vector< Direction > GridWalker::GetPossibleDirections( const Grid& grid, const Coordinate& currentCoordinate, Path& walkedPath )
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

bool GridWalker::IsDirectionPossible( const Grid& grid, const Direction& direction, const Coordinate& currentCoordinate, Path& walkedPath )
{
    Coordinate coordinateToTry{ currentCoordinate };
    coordinateToTry.MoveCoordinateByOne( direction );

    return not walkedPath.AlreadyVisited( coordinateToTry ) && grid.IsWithinGridBounds( coordinateToTry );
}