#include "LatticeIterator.H"


LatticeIterator::LatticeIterator(Lattice & inLattice)                    //constructor
{
    Itx = 0;
    Ity = 0;
    dx_ = 1;
    dy_ = 1;
    theLattice = &inLattice;
    Intx_ = theLattice->Lx();
    Inty_ = theLattice->Ly();
}

LatticeIterator::LatticeIterator(Lattice & inLattice, int dx, int dy)
{
    Itx = 0;
    Ity = 0;
    dx_ = dx;
    dy_ = dy;
    theLattice = &inLattice;
    Intx_ = theLattice->Lx();
    Inty_ = theLattice->Ly();
}



LatticeIterator& LatticeIterator::operator++(int)     //operator++ is postfix with (int) -> means "a++"
{                                                     //erst zurückgeben und dann erhöhen
    Itx += dx_;
    if(Itx>=theLattice->Lx())
	{
	    Itx = 0;
	    Ity += dy_;
	}
	if(Ity>theLattice->Ly())
	    Ity = theLattice->Ly();
         
    return (*this);
}


Point LatticeIterator::operator*()
{
    return Point(Itx,Ity,*theLattice);
}


void LatticeIterator::begin()
{
    Itx = 0;
    Ity = 0;
}


bool LatticeIterator::end()
{

    if(Ity>=theLattice->Ly())
    {
	    return true;
    }
    else 
    {
	    return false;
    }   
}

int LatticeIterator::getX()
{
    return Itx;
}

int LatticeIterator::getY()
{
    return Ity;
}

LatticeIterator& LatticeIterator::operator=(const LatticeIterator& that)
{
    (*this).Itx = that.Itx;
    (*this).Ity = that.Ity;
    (*this).dx_ = that.dx_;
    (*this).dy_ = that.dy_;
    (*this).Intx_ = that.Intx_;
    (*this).Inty_ = that.Inty_;
    (*this).theLattice = that.theLattice;
}
