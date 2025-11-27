////////////////////////////////////////////////////////////////////////////////
/// Includes
////////////////////////////////////////////////////////////////////////////////

#include "Coordinate.hpp"

#include "Direction.hpp"
////////////////////////////////////////////////////////////////////////////////
/// Coordinate
////////////////////////////////////////////////////////////////////////////////

void gw::Coordinate::MoveCoordinateByOne( const Direction& direction )
{
    switch( direction )
    {
        case Direction::Up:
        {
            m_RowIndex -= 1;
            break;
        }
        case Direction::Down:
        {
            m_RowIndex += 1;
            break;
        }
        case Direction::Left:
        {
            m_ColumnIndex -= 1;
            break;
        }
        case Direction::Right:
        {
            m_ColumnIndex += 1;
            break;
        }
        case Direction::Undefined:
        {
            break;
        }
    }
}

bool gw::Coordinate::operator==( const Coordinate& other ) const
{
    return m_RowIndex == other.m_RowIndex && m_ColumnIndex == other.m_ColumnIndex;
}

namespace gw 
{
    std::ostream &operator<<( std::ostream& outputStream, const Coordinate& coordinates )
    {
        outputStream << "[" << coordinates.m_RowIndex << ", " << coordinates.m_ColumnIndex << "]";
        return outputStream;
    }
} //gw