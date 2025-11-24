#ifndef COORDINATE
#define COORDINATE

////////////////////////////////////////////////////////////////////////////////
/// Includes
////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "Direction.hpp"

namespace gw
{
    ////////////////////////////////////////////////////////////////////////////////
    /// Coordinates
    ////////////////////////////////////////////////////////////////////////////////
    ///
    struct Coordinate
    {
        int m_RowIndex{};
        int m_ColumnIndex{};
        
        void MoveCoordinateByOne( const Direction& direction );
        
        bool operator==( const Coordinate& other ) const;
        
        friend std::ostream& operator<<( std::ostream& outputStream, const Coordinate& coordinate );
    };
    
} //gw

#endif //COORDINATE