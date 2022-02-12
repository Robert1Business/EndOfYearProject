#ifndef _NEIGHBOURITERATOR_H
#define _NEIGHBOURITERATOR_H

#include "definitions.H"
#include "Point.H"

enum DIRECTION {North, East, South, West, NorthWest, NorthEast, SouthWest, SouthEast, NorthNorth, EastEast, SouthSouth, WestWest, End};

enum NUMBEROFNEIGHBOURS {FOUR, EIGHT, TWELVE};

class NeighbourIterator  
{

private:
    int dir;
    Point p;
    NUMBEROFNEIGHBOURS numNeigh;
public:
    NeighbourIterator(const Point& inPoint, NUMBEROFNEIGHBOURS numNeighIn = TWELVE);     //constructor for the neighbour iterator

    NeighbourIterator& operator++(int);
    Point operator*();

    DIRECTION direction();

    void begin();
    bool end();


};

#endif
