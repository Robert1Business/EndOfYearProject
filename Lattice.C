#include "Lattice.H"


Lattice::Lattice()            //constructor standard
{
    privLx = 0;
    privLy = 0;
    privdx = 0;
    privdy = 0;
}


Lattice::Lattice(int Lxstart, int Lystart, real dxstart, real dystart)   //constructor
{
    privLx = Lxstart;
    privLy = Lystart;
    privdx = dxstart;
    privdy = dystart;
    return;
}


int Lattice::Lx()
{
    return privLx;
}


int Lattice::Ly()
{
    return privLy;
}


real Lattice::dx()
{
    return privdx;
}


real Lattice::dy()
{
    return privdy;
}

Lattice& Lattice::operator=(const Lattice& that)
{
    if (this != &that)
    {
        (*this).privLx = that.privLx;
        (*this).privLy = that.privLy;
        (*this).privdx = that.privdx;
        (*this).privdy = that.privdy;
    }

    return (*this);
}



