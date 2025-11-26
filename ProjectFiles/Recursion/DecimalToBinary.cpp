#include "DecimalToBinary.hpp"
#include <iostream>
#include <cmath>
#include <cassert>

std::string rec::DecToBin::operator()( int decimal )
{
    if ( decimal == 0 )
    {
        return m_BinaryCharacters;
    }
    
    if ( decimal % 2 == 0 )
    {
        m_BinaryCharacters += '0';
    }
    else
    {
        m_BinaryCharacters += '1';
    }
 
    this->operator()( decimal / 2 );

    return m_BinaryCharacters;
}

int rec::BinToDec::operator()( std::string binary )
{
    if ( binary.size() == 0 )
    {
        return m_DecimalNr;
    }

    assert( binary.back() == '0' or binary.back() == '1' );

    if ( binary.back() == '1' )
    {
        m_DecimalNr += std::pow(2, binary.size() - 1 );
    }
    
    this->operator()( binary.substr( 0, binary.size() - 1 ) );

    return m_DecimalNr;
}
