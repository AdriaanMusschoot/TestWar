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
    class GridWalkerBase
    {
    public:
        ////////////////////////////////////////////////////////////////////////////////
        /// Public Constructors & Destructors
        
        // Constructor
        GridWalkerBase( const Grid& gridToWalk );

        // Destructor
        virtual ~GridWalkerBase();

        ////////////////////////////////////////////////////////////////////////////////
        /// Public Methods
        
        const Grid& GetGridReference() const;

        void AddPossiblePath( const Path& path );

        virtual void WalkPaths( Path pathSoFar, const Coordinate& startCoordinate ) = 0;

    private:
        ////////////////////////////////////////////////////////////////////////////////
        /// Private Runtime Variables

        const Grid& m_GridToWalk;

        ////////////////////////////////////////////////////////////////////////////////
        /// Private Init Variables

        std::mutex m_PathMutex{};
        std::vector< Path > m_PossiblePaths{};
    };

    ////////////////////////////////////////////////////////////////////////////////
    /// RecursiveGridWalker
    ////////////////////////////////////////////////////////////////////////////////
    ///
    class RecursiveGridWalker : public GridWalkerBase
    {
    public:
        ////////////////////////////////////////////////////////////////////////////////
        /// Public Constructors & Destructors
        
        // Constructor
        RecursiveGridWalker( const Grid& gridToWalk );
        
        ////////////////////////////////////////////////////////////////////////////////
        /// Public Methods
    
        virtual void WalkPaths( Path pathSoFar, const Coordinate& startCoordinate ) override;        
    };
    
    ////////////////////////////////////////////////////////////////////////////////
    /// DoubleThreadedGridWalker
    ////////////////////////////////////////////////////////////////////////////////
    ///
    class DoubleThreadedGridWalker : public GridWalkerBase
    {
    public:
        ////////////////////////////////////////////////////////////////////////////////
        /// Public Constructors & Destructors
        
        // Constructor
        DoubleThreadedGridWalker( const Grid& gridToWalk );
        
        ////////////////////////////////////////////////////////////////////////////////
        /// Public Methods
    
        virtual void WalkPaths( Path pathSoFar, const Coordinate& startCoordinate ) override;
        
    private:
        ////////////////////////////////////////////////////////////////////////////////
        /// Private Methods
        
        void WalkPathsSingle( Path pathSoFar, const Coordinate& startCoordinate );
    };

    ////////////////////////////////////////////////////////////////////////////////
    /// LayeredThreadsGridWalker
    ////////////////////////////////////////////////////////////////////////////////
    ///
    class LayeredThreadsGridWalker : public GridWalkerBase
    {
    public:
        ////////////////////////////////////////////////////////////////////////////////
        /// Public Constructors & Destructors
        
        // Constructor
        LayeredThreadsGridWalker( const Grid& gridToWalk, int layerToSpawnThreads );
        
        // Destructor
        virtual ~LayeredThreadsGridWalker() override;

        ////////////////////////////////////////////////////////////////////////////////
        /// Public Methods
    
        virtual void WalkPaths( Path pathSoFar, const Coordinate& startCoordinate ) override;
        
    private:
        ////////////////////////////////////////////////////////////////////////////////
        /// Private Init Variables

        const int m_LayerToSpawnThreads{};

        std::mutex m_ThreadMutex{};
        std::vector< std::jthread > m_JThreads{};

        ////////////////////////////////////////////////////////////////////////////////
        /// Private Methods
        
        void WalkPathsSingle( Path pathSoFar, const Coordinate& startCoordinate, int layer = 0 );
    };

    ////////////////////////////////////////////////////////////////////////////////
    /// MaxThreadedGridWalker
    ////////////////////////////////////////////////////////////////////////////////
    ///
    class MaxThreadedGridWalker : public GridWalkerBase
    {
    public:
        ////////////////////////////////////////////////////////////////////////////////
        /// Public Constructors & Destructors
        
        // Constructor
        MaxThreadedGridWalker( const Grid& gridToWalk, int layerToSpawnThreads );
        
        // Destructor
        virtual ~MaxThreadedGridWalker() override;

        ////////////////////////////////////////////////////////////////////////////////
        /// Public Methods
    
        virtual void WalkPaths( Path pathSoFar, const Coordinate& startCoordinate ) override;
        
    private:
        ////////////////////////////////////////////////////////////////////////////////
        /// Private Init Variables

        const int m_LayerToSpawnThreadsUntil{};

        std::mutex m_AdditionalActiveThreadsMutex{};
        unsigned int m_AdditionalActiveThreads{};

        std::mutex m_ThreadMutex{};
        std::vector< std::jthread > m_JThreads{};

        ////////////////////////////////////////////////////////////////////////////////
        /// Private Methods
        
        void WalkPathsSingle( Path pathSoFar, const Coordinate& startCoordinate, int layer = 0 );
    };
} //gw

#endif //GRID_WALKER