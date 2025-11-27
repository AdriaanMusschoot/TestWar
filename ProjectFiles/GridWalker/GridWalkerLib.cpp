#include "GridWalkerLib.hpp"

#include "Grid.hpp"
#include "Path.hpp"
#include "Direction.hpp"

namespace gw
{
    std::vector< Direction > GetPossibleDirections(const Grid &grid, const Coordinate &currentCoordinate, Path &walkedPath)
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
    
    bool IsDirectionPossible( const Grid& grid, const Direction& direction, const Coordinate& currentCoordinate, Path& walkedPath )
    {
        Coordinate coordinateToTry{ currentCoordinate };
        coordinateToTry.MoveCoordinateByOne( direction );
        
        return not walkedPath.AlreadyVisited( coordinateToTry ) && grid.IsWithinGridBounds( coordinateToTry );
    }
} //gw
