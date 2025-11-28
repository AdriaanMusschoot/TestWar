#ifndef GRID_WALKER
#define GRID_WALKER

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

#include "Grid.hpp"
#include "Path.hpp"

#include <mutex>
#include <thread>
#include <condition_variable>

namespace gw
{
    class GridWalkerBase
    {
    public:
        ////////////////////////////////////////////////////////////////////////////////
        // Public Constructors & Destructors
        
        // Constructor
        GridWalkerBase( const Grid& gridToWalk );

        // Destructor
        virtual ~GridWalkerBase();

        ////////////////////////////////////////////////////////////////////////////////
        // Public Methods
        
        const Grid& GetGridReference() const;

        void AddPossiblePath( const Path& path );

        virtual void WalkPaths( Path pathSoFar, const Coordinate& startCoordinate ) = 0;

    private:
        ////////////////////////////////////////////////////////////////////////////////
        // Private Runtime Variables

        const Grid& m_GridToWalk;

        ////////////////////////////////////////////////////////////////////////////////
        // Private Init Variables

        std::mutex m_PathMutex{};
        std::vector< Path > m_PossiblePaths{};
    };

    ////////////////////////////////////////////////////////////////////////////////
    // RecursiveGridWalker
    ////////////////////////////////////////////////////////////////////////////////
    //
    class RecursiveGridWalker : public GridWalkerBase
    {
    public:
        ////////////////////////////////////////////////////////////////////////////////
        // Public Constructors & Destructors
        
        // Constructor
        RecursiveGridWalker( const Grid& gridToWalk );
        
        ////////////////////////////////////////////////////////////////////////////////
        // Public Methods
    
        virtual void WalkPaths( Path pathSoFar, const Coordinate& startCoordinate ) override;        
    };
    
    ////////////////////////////////////////////////////////////////////////////////
    /// DoubleThreadedGridWalker
    ////////////////////////////////////////////////////////////////////////////////
    //
    class DoubleThreadedGridWalker : public GridWalkerBase
    {
    public:
        ////////////////////////////////////////////////////////////////////////////////
        // Public Constructors & Destructors
        
        // Constructor
        DoubleThreadedGridWalker( const Grid& gridToWalk );
        
        ////////////////////////////////////////////////////////////////////////////////
        // Public Methods
    
        virtual void WalkPaths( Path pathSoFar, const Coordinate& startCoordinate ) override;
        
    private:
        ////////////////////////////////////////////////////////////////////////////////
        // Private Methods
        
        void WalkPathsSingle( Path pathSoFar, const Coordinate& startCoordinate );
    };

    ////////////////////////////////////////////////////////////////////////////////
    // LayeredThreadsGridWalker
    ////////////////////////////////////////////////////////////////////////////////
    //
    // Will spawn threads only once at a layer 
    // If it is to deep it will only spawn them from one branch
    // rendering it pretty much worse than double threaded since we start a lot of threads once for very little work
    //
    class ThreadsAtLayerGridWalker : public GridWalkerBase
    {
    public:
        ////////////////////////////////////////////////////////////////////////////////
        // Public Constructors & Destructors
        
        // Constructor
        ThreadsAtLayerGridWalker( const Grid& gridToWalk, int layerToSpawnThreads );
        
        // Destructor
        virtual ~ThreadsAtLayerGridWalker() override;

        ////////////////////////////////////////////////////////////////////////////////
        // Public Methods
    
        virtual void WalkPaths( Path pathSoFar, const Coordinate& startCoordinate ) override;
        
    private:
        ////////////////////////////////////////////////////////////////////////////////
        // Private Init Variables

        const int m_LayerToSpawnThreads{};

        std::mutex m_ThreadMutex{};
        std::vector< std::jthread > m_JThreads{};

        ////////////////////////////////////////////////////////////////////////////////
        // Private Methods
        
        void WalkPathsSingle( Path pathSoFar, const Coordinate& startCoordinate, int layer = 0 );
    };

    ////////////////////////////////////////////////////////////////////////////////
    /// ThreadsUntilLayerGridWalker
    ////////////////////////////////////////////////////////////////////////////////
    //
    // spawns threads until a certain layer 
    // if the layer is too low it will lose some performance
    // if it is to high it will execute too little work compared to starting a new thread
    // use while loop
    //
    class ThreadsUntilLayerGridWalker : public GridWalkerBase
    {
    public:
        ////////////////////////////////////////////////////////////////////////////////
        // Public Constructors & Destructors
        
        // Constructor
        ThreadsUntilLayerGridWalker( const Grid& gridToWalk, int layerToSpawnThreadsUntil );
        
        // Destructor
        virtual ~ThreadsUntilLayerGridWalker() override;

        ////////////////////////////////////////////////////////////////////////////////
        /// Public Methods
    
        virtual void WalkPaths( Path pathSoFar, const Coordinate& startCoordinate ) override;
        
    private:
        ////////////////////////////////////////////////////////////////////////////////
        // Private Init Variables

        const int m_LayerToSpawnThreadsUntil{};

        std::mutex m_AdditionalActiveThreadsMutex{};
        unsigned int m_AdditionalActiveThreads{};

        std::mutex m_ThreadMutex{};
        std::vector< std::jthread > m_JThreads{};

        ////////////////////////////////////////////////////////////////////////////////
        // Private Methods
        
        void WalkPathsSingle( Path pathSoFar, const Coordinate& startCoordinate, int layer = 0 );
    };

    ////////////////////////////////////////////////////////////////////////////////
    /// CVThreadsUntilLayerGridWalker
    ////////////////////////////////////////////////////////////////////////////////
    //
    // spawns threads until a certain layer 
    // if the layer is too low it will lose some performance
    // if it is to high it will execute too little work compared to starting a new thread
    // use conditian variable
    //
    class CVThreadsUntilLayerGridWalker : public GridWalkerBase
    {
    public:
        ////////////////////////////////////////////////////////////////////////////////
        // Public Constructors & Destructors
        
        // Constructor
        CVThreadsUntilLayerGridWalker( const Grid& gridToWalk, int layerToSpawnThreads );
        
        // Destructor
        virtual ~CVThreadsUntilLayerGridWalker() override;

        ////////////////////////////////////////////////////////////////////////////////
        // Public Methods
    
        virtual void WalkPaths( Path pathSoFar, const Coordinate& startCoordinate ) override;
        
    private:
        ////////////////////////////////////////////////////////////////////////////////
        // Private Init Variables

        const int m_LayerToSpawnThreadsUntil{};

        // protects variable m_AdditionalActiveThreads and m_ConditionVaricableThreadExecution
        // so they can safely merge at the end of control
        std::mutex m_AdditionalActiveThreadsMutex{};
        unsigned int m_AdditionalActiveThreads{};
        std::condition_variable m_ConditionVaricableThreadExecution{};

        // protects variable m_JThreads 
        // when adding new threads
        std::mutex m_ThreadMutex{};
        std::vector< std::jthread > m_JThreads{};
        
        ////////////////////////////////////////////////////////////////////////////////
        // Private Methods
        
        void WalkPathsSingle( Path pathSoFar, const Coordinate& startCoordinate, int layer = 0 );
    };
} //gw

#endif //GRID_WALKER