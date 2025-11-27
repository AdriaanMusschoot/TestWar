////////////////////////////////////////////////////////////////////////////////
/// Includes
////////////////////////////////////////////////////////////////////////////////

#include "GridWalker.hpp"
#include "GridWalkerLib.hpp"
#include "Direction.hpp"

////////////////////////////////////////////////////////////////////////////////
/// GridWalkerBase
////////////////////////////////////////////////////////////////////////////////

gw::GridWalkerBase::GridWalkerBase( const Grid& gridToWalk )
    : m_GridToWalk{ gridToWalk }
{
}

gw::GridWalkerBase::~GridWalkerBase()
{
    std::cout << m_PossiblePaths.size() << " possible paths\n";
}

const gw::Grid &gw::GridWalkerBase::GetGridReference() const
{
    return m_GridToWalk;
}

void gw::GridWalkerBase::AddPossiblePath( const Path& path )
{
    std::scoped_lock pathLock{ m_PathMutex };
    m_PossiblePaths.emplace_back( path );
}

////////////////////////////////////////////////////////////////////////////////
/// RecursiveGridWalker
////////////////////////////////////////////////////////////////////////////////

gw::RecursiveGridWalker::RecursiveGridWalker( const Grid& gridToWalk )
    : GridWalkerBase( gridToWalk )
{
}

void gw::RecursiveGridWalker::WalkPaths( Path pathSoFar, const Coordinate& startCoordinate )
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


////////////////////////////////////////////////////////////////////////////////
/// DoubleThreadedGridWalker
////////////////////////////////////////////////////////////////////////////////

gw::DoubleThreadedGridWalker::DoubleThreadedGridWalker( const Grid& gridToWalk )
    : GridWalkerBase( gridToWalk )
{
}

void gw::DoubleThreadedGridWalker::WalkPaths( Path pathSoFar, const Coordinate& startCoordinate )
{
    auto& [ currentPathCoordinate, currentPathDirection ] = pathSoFar.AddStep( startCoordinate );
    std::vector< Direction > possibleDirections{ GetPossibleDirections( GetGridReference(), currentPathCoordinate, pathSoFar ) };

    std::vector< std::jthread > jthreads;
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
                    WalkPathsSingle( pathSoFar, nextCoordinate );
                }
            }
        );
    }
}

void gw::DoubleThreadedGridWalker::WalkPathsSingle(Path pathSoFar, const Coordinate &startCoordinate)
{
    auto&[ currentPathCoordinate, currentPathDirection ] = pathSoFar.AddStep( startCoordinate );
    std::vector< Direction > possibleDirections{ GetPossibleDirections( GetGridReference(), currentPathCoordinate, pathSoFar ) };

    for ( const Direction& possibleDirection: possibleDirections )
    {
        currentPathDirection = possibleDirection;

        Coordinate nextCoordinate{ currentPathCoordinate };
        nextCoordinate.MoveCoordinateByOne( possibleDirection );
        
        WalkPathsSingle( pathSoFar, nextCoordinate );
    }

    if ( pathSoFar.Length() == GetGridReference().Size() )
    {
        AddPossiblePath( pathSoFar );
    }
}

////////////////////////////////////////////////////////////////////////////////
/// LayeredThreadsGridWalker
////////////////////////////////////////////////////////////////////////////////

gw::LayeredThreadsGridWalker::LayeredThreadsGridWalker( const Grid& gridToWalk, int layerToSpawnThreads )
    : GridWalkerBase( gridToWalk )
    , m_LayerToSpawnThreads{ layerToSpawnThreads }
{
}

gw::LayeredThreadsGridWalker::~LayeredThreadsGridWalker()
{
    // there is only the main thread that can spawn other threads
    // so when execution arrives here all possible threads will have been spawned
    for ( std::jthread& jThread : m_JThreads )
    {
        jThread.join();
    }
}

void gw::LayeredThreadsGridWalker::WalkPaths( Path pathSoFar, const Coordinate& startCoordinate )
{
    WalkPathsSingle( pathSoFar, startCoordinate );
}

void gw::LayeredThreadsGridWalker::WalkPathsSingle( Path pathSoFar, const Coordinate& startCoordinate, int layer )
{
    auto& [ currentPathCoordinate, currentPathDirection ] = pathSoFar.AddStep( startCoordinate );
    const std::vector< Direction > possibleDirections{ GetPossibleDirections( GetGridReference(), currentPathCoordinate, pathSoFar ) };

    for ( const Direction& possibleDirection : possibleDirections )
    {
        currentPathDirection = possibleDirection;

        Coordinate nextCoordinate{ currentPathCoordinate };
        nextCoordinate.MoveCoordinateByOne( possibleDirection );
        
        if ( layer == m_LayerToSpawnThreads )
        {
            if ( std::scoped_lock threadLock{ m_ThreadMutex };
                 m_JThreads.size() < std::thread::hardware_concurrency() )
            {
                m_JThreads.emplace_back
                (
                    std::jthread
                    {
                        [ =, this ]()
                        {
                            WalkPathsSingle( pathSoFar, nextCoordinate, layer + 1 );
                        }
                    }
                );

                continue;
            }
        }
        WalkPathsSingle( pathSoFar, nextCoordinate, layer + 1 );
    }

    if ( pathSoFar.Length() == GetGridReference().Size() )
    {
        AddPossiblePath( pathSoFar );
    }
}

////////////////////////////////////////////////////////////////////////////////
/// MaxThreadedGridWalker
////////////////////////////////////////////////////////////////////////////////

gw::MaxThreadedGridWalker::MaxThreadedGridWalker( const Grid& gridToWalk, int layerToSpawnThreadsUntil )
    : GridWalkerBase( gridToWalk )
    , m_LayerToSpawnThreadsUntil{ layerToSpawnThreadsUntil }
{
}

gw::MaxThreadedGridWalker::~MaxThreadedGridWalker()
{
    // other threads than main can spawn threads 
    // so main execution will be here before threads spawned by others are here
    // so we keep this thread looping until we have no more active threads
    // we also don't need to join them then anymore since they will be joined and only after that is active threads decreased
    do 
    {

    } while ( m_AdditionalActiveThreads != 0 );

    // we still join the threads because the final thread to finish will set the additional active threads value to zero before it terminates
    for ( std::jthread& jthread : m_JThreads )
    {
        jthread.join();
    }

    // sounds like all we need is a condition variable actually
}

void gw::MaxThreadedGridWalker::WalkPaths( Path pathSoFar, const Coordinate& startCoordinate )
{
    WalkPathsSingle( pathSoFar, startCoordinate );
}

void gw::MaxThreadedGridWalker::WalkPathsSingle( Path pathSoFar, const Coordinate& startCoordinate, int layer )
{
    auto& [ currentPathCoordinate, currentPathDirection ] = pathSoFar.AddStep( startCoordinate );
    const std::vector< Direction > possibleDirections{ GetPossibleDirections( GetGridReference(), currentPathCoordinate, pathSoFar ) };

    for ( const Direction& possibleDirection : possibleDirections )
    {
        currentPathDirection = possibleDirection;

        Coordinate nextCoordinate{ currentPathCoordinate };
        nextCoordinate.MoveCoordinateByOne( possibleDirection );
        
        if ( layer <= m_LayerToSpawnThreadsUntil )
        {
            if ( std::scoped_lock activeThreadLock{ m_AdditionalActiveThreadsMutex };
                 m_AdditionalActiveThreads <= std::thread::hardware_concurrency() )
            {
                std::scoped_lock threadLock{ m_ThreadMutex };
                
                m_JThreads.emplace_back
                (
                    std::jthread
                    {
                        [ =, this ]()
                        {
                            WalkPathsSingle( pathSoFar, nextCoordinate, layer + 1 );
                            
                            std::scoped_lock activeThreadLock{ m_AdditionalActiveThreadsMutex };
                            --m_AdditionalActiveThreads;
                        }
                    }
                );

                ++m_AdditionalActiveThreads;

                continue;
            }
        }
        WalkPathsSingle( pathSoFar, nextCoordinate, layer + 1 );
    }

    if ( pathSoFar.Length() == GetGridReference().Size() )
    {
        AddPossiblePath( pathSoFar );
    }
}

////////////////////////////////////////////////////////////////////////////////
/// CVMaxThreadedGridWalker
////////////////////////////////////////////////////////////////////////////////

gw::CVMaxThreadedGridWalker::CVMaxThreadedGridWalker( const Grid& gridToWalk, int layerToSpawnThreadsUntil )
    : GridWalkerBase( gridToWalk )
    , m_LayerToSpawnThreadsUntil{ layerToSpawnThreadsUntil }
{
}

gw::CVMaxThreadedGridWalker::~CVMaxThreadedGridWalker()
{
    std::unique_lock lockActiveThreads{ m_AdditionalActiveThreadsMutex };
    m_ConditionVaricableThreadExecution.wait( lockActiveThreads, [&](){ return m_AdditionalActiveThreads == 0; } );
}

void gw::CVMaxThreadedGridWalker::WalkPaths( Path pathSoFar, const Coordinate& startCoordinate )
{
    WalkPathsSingle( pathSoFar, startCoordinate );
}

void gw::CVMaxThreadedGridWalker::WalkPathsSingle( Path pathSoFar, const Coordinate& startCoordinate, int layer )
{
    auto& [ currentPathCoordinate, currentPathDirection ] = pathSoFar.AddStep( startCoordinate );
    const std::vector< Direction > possibleDirections{ GetPossibleDirections( GetGridReference(), currentPathCoordinate, pathSoFar ) };

    for ( const Direction& possibleDirection : possibleDirections )
    {
        currentPathDirection = possibleDirection;

        Coordinate nextCoordinate{ currentPathCoordinate };
        nextCoordinate.MoveCoordinateByOne( possibleDirection );
        
        if ( layer <= m_LayerToSpawnThreadsUntil )
        {
            if ( std::scoped_lock activeThreadLock{ m_AdditionalActiveThreadsMutex };
                 m_AdditionalActiveThreads <= std::thread::hardware_concurrency() )
            {
                std::scoped_lock threadLock{ m_ThreadMutex };
                
                m_JThreads.emplace_back
                (
                    std::jthread
                    {
                        [ =, this ]()
                        {
                            WalkPathsSingle( pathSoFar, nextCoordinate, layer + 1 );

                            std::scoped_lock activeThreadLock{ m_AdditionalActiveThreadsMutex };
                            --m_AdditionalActiveThreads;
                            m_ConditionVaricableThreadExecution.notify_one();
                        }
                    }
                );

                ++m_AdditionalActiveThreads;

                continue;
            }
        }
        WalkPathsSingle( pathSoFar, nextCoordinate, layer + 1 );
    }

    if ( pathSoFar.Length() == GetGridReference().Size() )
    {
        AddPossiblePath( pathSoFar );
    }
}