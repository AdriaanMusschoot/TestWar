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