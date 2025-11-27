#ifndef GRID_WALKER_CRTP
#define GRID_WALKER_CRTP

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
    /// GridWalkerBaseCRTP
    ////////////////////////////////////////////////////////////////////////////////
    ///
    template < typename DerivedClass >
    class CRTPGridWalkerBase
    {
    public:
        ////////////////////////////////////////////////////////////////////////////////
        /// Public Constructors & Destructors
        
        // Constructor
        CRTPGridWalkerBase( const Grid& gridToWalk )
            : m_GridToWalk{ gridToWalk }
        {
        }

        // Destructor
        virtual ~CRTPGridWalkerBase()
        {
            // std::cout << m_PossiblePaths.size() << " possible paths\n";
        }

        ////////////////////////////////////////////////////////////////////////////////
        /// Public Methods
        
        const Grid& GetGridReference() const
        {
            return m_GridToWalk;
        }

        void AddPossiblePath( const Path& path )
        {
            m_PossiblePaths.emplace_back( path );
        }

        void WalkPaths( Path pathSoFar, const Coordinate& startCoordinate )
        {
            static_cast< DerivedClass* >( this )->WalkPaths( pathSoFar, startCoordinate );
        }

    private:
        ////////////////////////////////////////////////////////////////////////////////
        /// Private Runtime Variables

        const Grid& m_GridToWalk;

        ////////////////////////////////////////////////////////////////////////////////
        /// Private Init Variables

        std::vector< Path > m_PossiblePaths; 
    };

    ////////////////////////////////////////////////////////////////////////////////
    /// GridWalker
    ////////////////////////////////////////////////////////////////////////////////
    ///
    class CRTPRecursiveGridWalker : public CRTPGridWalkerBase< CRTPRecursiveGridWalker >
    {
    public:
        ////////////////////////////////////////////////////////////////////////////////
        /// Public Constructors & Destructors
        
        // Constructor
        CRTPRecursiveGridWalker( const Grid& gridToWalk );
        
        // Destructor
        virtual ~CRTPRecursiveGridWalker() override;
    
        ////////////////////////////////////////////////////////////////////////////////
        /// Public Methods
    
        void WalkPaths( Path pathSoFar, const Coordinate& startCoordinate );

        // void StartWalkingPathsDoubleThreaded( Path pathSoFar, const Coordinate& startCoordinate );
        
        // void StartWalkingPathsThreadLayer( Path pathSoFar, const Coordinate& startCoordinate, int layer = 0 );

        // void StartWalkingPathsMaxThreaded( Path pathSoFar, const Coordinate &startCoordinate, int layer = 0 );

    private:
        ////////////////////////////////////////////////////////////////////////////////
        /// Private Init Variables
                
        // std::mutex m_PathMutex;

        // std::mutex m_ThreadMutex;
        // std::vector< std::jthread > m_JThreads;

        // std::mutex m_ActiveThreadsMutex;
        // unsigned int m_ActiveThreads; 
        
        ////////////////////////////////////////////////////////////////////////////////
        /// Private Methods
        
    };
} //gw

#endif //GRID_WALKER_CRTP