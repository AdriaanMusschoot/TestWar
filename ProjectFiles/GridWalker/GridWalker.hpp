#ifndef GRID_WALKER
#define GRID_WALKER

////////////////////////////////////////////////////////////////////////////////
/// Includes
////////////////////////////////////////////////////////////////////////////////

#include "Grid.hpp"
#include "Path.hpp"
#include <mutex>
#include <thread>

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
        GridWalker( const Grid& gridToWalk );
        
        // Destructor
        ~GridWalker();
    
        ////////////////////////////////////////////////////////////////////////////////
        /// Public Methods
    
        void WalkPaths( Path pathSoFar, const Coordinate& startCoordinate );
        
        void StartWalkingPathsDoubleThreaded( Path pathSoFar, const Coordinate& startCoordinate );
        
        void StartWalkingPathsMaxThreaded( Path pathSoFar, const Coordinate& startCoordinate, int layer = 0 );

        // void WalkPathWithSeperatePathCollection( const Coordinate& startCoordinate, int layer = 0 );
    private:
        ////////////////////////////////////////////////////////////////////////////////
        /// Private Init Variables
        
        const Grid& m_GridToWalk;
        
        std::mutex m_PathMutex;
        std::vector< Path > m_PossiblePaths; 

        std::mutex m_ThreadMutex;
        std::vector< std::jthread > m_JThreads;

        std::mutex m_ActiveThreadsMutex;
        unsigned int m_ActiveThreads; 
        
        ////////////////////////////////////////////////////////////////////////////////
        /// Private Methods
        
        static std::vector< Direction > GetPossibleDirections( const Grid& grid, const Coordinate& currentCoordinate, Path& walkedPath );
        static bool IsDirectionPossible( const Grid& grid, const Direction& direction, const Coordinate& currentCoordinate, Path& walkedPath );
    };
} //gw

#endif //GRID_WALKER