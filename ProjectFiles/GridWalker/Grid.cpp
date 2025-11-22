////////////////////////////////////////////////////////////////////////////////
/// Includes
////////////////////////////////////////////////////////////////////////////////

#include "Grid.hpp"

#include <iostream>

////////////////////////////////////////////////////////////////////////////////
/// Grid
////////////////////////////////////////////////////////////////////////////////

Grid::Grid( int rows, int columns )
    : m_Rows{ rows }
    , m_Columns{ columns }
    , m_GridCells{ static_cast< size_t >( rows * columns ) }
{
    for (std::size_t idx{}; idx < m_GridCells.size(); ++idx )
    {
        Coordinate& currentCell = m_GridCells[idx];
        currentCell.m_RowIndex = idx / rows;
        currentCell.m_ColumnIndex  = idx % rows;
    }

    std::cout << rows << " x " << columns << " grid created";
    std::cout << *this;
}

Grid::~Grid()
{
    std::cout << m_GridCells.size() << " cells destroyed\n";
}

bool Grid::IsWithinGridBounds( const Coordinate& coordinate ) const
{
    return coordinate.m_RowIndex >= 0 
        && coordinate.m_RowIndex < m_Rows
        && coordinate.m_ColumnIndex >= 0
        && coordinate.m_ColumnIndex < m_Columns;
}

size_t Grid::Size() const
{
    return m_GridCells.size();
}

const int& Grid::Rows() const
{
    return m_Rows;
}

const int& Grid::Columns() const
{
    return m_Columns;
}

std::ostream &operator<<( std::ostream &outputStream, const Grid &grid )
{
    int currentRowIndex{ 0 };
    
    outputStream << "\n\n";
    for ( const Coordinate& coordinate : grid.m_GridCells )
    {
        if ( currentRowIndex != coordinate.m_RowIndex )
        {
            outputStream << "\n";
        }
        currentRowIndex = coordinate.m_RowIndex;
        outputStream << coordinate << "\t";
    }
    outputStream << "\n\n";

    return outputStream;
}