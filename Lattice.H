#ifndef _LATTICE
#define _LATTICE
#include "definitions.H"



class Lattice
{
 
private:
    int privLx,privLy;         //number of lattice points in x und y
    real privdx, privdy;       //lattice constant (size of one cell) 

public:

    Lattice();
    Lattice(int Lxstart, int Lystart, real dxstart, real dystart);    //constructor for a lattice

    int Lx();        //functions to return a private value
    int Ly();        //int and not void, because there is a returned value for calculation
    real dx();
    real dy();


    Lattice& operator=(const Lattice& that);
};



#endif
