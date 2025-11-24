#ifndef GRID_WALKER
#define GRID_WALKER

////////////////////////////////////////////////////////////////////////////////
/// Includes
////////////////////////////////////////////////////////////////////////////////

#include "Grid.hpp"
#include "Path.hpp"
#include <mutex>

namespace gw
{
    ////////////////////////////////////////////////////////////////////////////////
    /// GridWalker
    ////////////////////////////////////////////////////////////////////////////////
    ///
    class GridWalker
    {
        public:
        ////////////////////////////////////////////////////////////////////////////////
        /// Public Constructors & Destructors
        
        // Constructor
        GridWalker( Grid& gridToWalk );
        
        // Destructor
        ~GridWalker();
    
        ////////////////////////////////////////////////////////////////////////////////
        /// Public Methods
    
        void WalkPaths( Path pathSoFar, const Coordinate& startCoordinate );
        
        void StartWalkingPathsThreaded( Path pathSoFar, const Coordinate& startCoordinate );
    private:
        ////////////////////////////////////////////////////////////////////////////////
        /// Private Init Variables
        
        Grid& m_GridToWalk;
        
        std::mutex m_PathMutex;
        std::vector< Path > m_PossiblePaths; 
        
        ////////////////////////////////////////////////////////////////////////////////
        /// Private Methods
                
        static std::vector< Direction > GetPossibleDirections( const Grid& grid, const Coordinate& currentCoordinate, Path& walkedPath );
        static bool IsDirectionPossible( const Grid& grid, const Direction& direction, const Coordinate& currentCoordinate, Path& walkedPath );
    };
} //gw

#endif //GRID_WALKER