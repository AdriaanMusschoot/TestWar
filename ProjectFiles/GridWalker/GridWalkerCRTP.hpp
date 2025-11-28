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

    private:        
    };
} //gw

#endif //GRID_WALKER_CRTP