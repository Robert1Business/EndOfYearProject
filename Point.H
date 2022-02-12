#ifndef _POINT
#define _POINT
#include "definitions.H"
#include "auxilliary.H"
#include "Lattice.H"

class Point
{


public:
    Lattice * theLattice;  //every point has to know about it´s lattice, so here is a pointer for the lattice

    int x, y;         

    Point();
    Point(Lattice & inLattice);
    Point(int a, int b, Lattice & inLattice);

    bool operator==(const Point& comparePoint);
    Point operator+(const Point& addPoint);  

    Point west();
    Point east();
    Point north();
    Point south();

    friend class Lattice;

};

//Ausgabe-Operator
std::ostream & operator<<(std::ostream & s, const Point & p);

#endif
