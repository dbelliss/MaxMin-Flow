#ifndef _VECTOR_CPP_
#define _VECTOR_CPP_

#include "vector.h"

template <class CityInfo>
const vector<CityInfo> & vector<CityInfo>::operator=( const vector<CityInfo> & rhs )
{
    if( this != &rhs )
    {
        delete [ ] objects;
        currentSize = rhs.size( );
        objects = new CityInfo[ currentSize ];
        for( int k = 0; k < currentSize; k++ )
            objects[ k ] = rhs.objects[ k ];
    }
    return *this;
}

template <class CityInfo>
void vector<CityInfo>::resize( int newSize )
{
    CityInfo *oldArray = objects;
    int numToCopy = newSize < currentSize ? newSize : currentSize;
    
    objects = new CityInfo[ newSize ];
    currentSize = newSize;
    
    for( int k = 0; k < numToCopy; k++ )
        objects[ k ] = oldArray[ k ];
    
    delete [ ] oldArray;
}

#endif
