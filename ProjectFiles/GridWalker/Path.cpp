////////////////////////////////////////////////////////////////////////////////
/// Includes
////////////////////////////////////////////////////////////////////////////////

#include "Path.hpp"

#include <algorithm>

////////////////////////////////////////////////////////////////////////////////
/// Path
////////////////////////////////////////////////////////////////////////////////

Path::Path( Grid& grid )
    : m_GridReference{ grid }
{
}

bool Path::AlreadyVisited( const Coordinate &coordinate ) const
{
    return std::find( m_PathElements.cbegin(), m_PathElements.cend(), coordinate ) != m_PathElements.cend();
}

PathElement& Path::AddStep( const Coordinate& coordinate )
{
    return m_PathElements.emplace_back( PathElement{ coordinate, Direction::Undefined } );
}

std::size_t Path::Length() const
{
    return m_PathElements.size();
}

int Path::FindCoordinateIndex( int rowIndex, int columnIndex ) const
{
    auto it = std::find( m_PathElements.cbegin(), m_PathElements.cend(), Coordinate{ rowIndex, columnIndex } );

    return std::distance( m_PathElements.cbegin(), it);
}

std::ostream &operator<<( std::ostream& outputStream, const Path& path )
{
    for ( int rowIdx{}; rowIdx < path.m_GridReference.Rows(); ++rowIdx )
    {
        for ( int columnIdx{}; columnIdx < path.m_GridReference.Columns(); ++columnIdx )
        {
            switch ( path.m_PathElements[ path.FindCoordinateIndex( rowIdx, columnIdx ) ].m_Direction )
            {
                case Direction::Up:
                {
                    outputStream << "⇈";
                    break;
                }
                case Direction::Down:
                {
                    outputStream << "⇊";
                    break;
                }
                case Direction::Right:
                {
                    outputStream << "⇉";

    // for ( const Coordinate& coordinate : path.m_PathElements )
    // {
    //     outputStream << coordinate << "\t";
    // }
                    break;
                }
                case Direction::Left:
                {
                    outputStream << "⇇";
                    break;
                }
                case Direction::Undefined:
                {
                    outputStream << "x";
                    break;
                }
            }
        }
        outputStream << "\n";
    }
    
    return outputStream;
}
