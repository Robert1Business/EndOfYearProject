#include "Point.H"


Point::Point()  //default-constructor (was required for the constructor NeighbourIterator)
{               //warum auch immer ...
    x=y=0;
    theLattice=NULL;
}


Point::Point(Lattice & inLattice)      //linking point and lattice                                 
{                                      //every point knows to which lattice it belongs
    x = 0;
    y = 0;
    theLattice = &inLattice;
}


Point::Point(int a, int b, Lattice & inLattice)     //ditto
{
      x = a;
      y = b;
      theLattice = &inLattice;

}
/*      if(a<0 || a>theLattice->Lx() || b<0 || b>theLattice->Ly())
	  error(" (Point::Point): Point (%i,%i) outside the lattice with size %i,%i",
		a,b, theLattice->Lx(), theLattice->Ly());
*/



bool Point::operator==(const Point& comparePoint)      //to compare "Point" and "comparePoint"
{
  
    if(x==comparePoint.x && y==comparePoint.y)
      {	
        return true;
      }
    else
      {
        return false;
      }
}


Point Point::operator+(const Point& addPoint)             //to add "addPoint" to the value of "Point"
{ //const, weil sonst Addition in NeighbourIterator::operator* nicht möglich
    Point resultPoint(*theLattice) ;

    if((*this).theLattice == addPoint.theLattice)   //are the points on the same lattice ?
    {
	resultPoint.x = (x + addPoint.x);         
	resultPoint.y = (y + addPoint.y);          

        //periodic boundary conditions: is resultPoint within the sim volume between 0 and Lx (or 0 and Ly)?
	while(resultPoint.x<0)                   
	    resultPoint.x += theLattice->Lx();        //if it is "left" outside: add the number of lattice points
 
	while(resultPoint.y<0)                   
	    resultPoint.y += theLattice->Ly();   
    
	resultPoint.x = resultPoint.x % theLattice->Lx();       //if it is "right" outside: modulo function
	resultPoint.y = resultPoint.y % theLattice->Ly();       //to get the remainder
        
	return resultPoint;                        
    }
    else
    {
	error("(Point::operator+): different lattice for point (%i,%i) and (%i,%i)",
	      (*this).x, (*this).y, addPoint.x,  addPoint.y);       //function void error from auxilliary.C file
    }

}


Point Point::west()
{
    Point resultPoint(*theLattice);

    resultPoint.x = x - 1;
    if (resultPoint.x == -1) resultPoint.x = theLattice->Lx()- 1;
    resultPoint.y = y;

    return resultPoint;
}


Point Point::east()
{
    Point resultPoint(*theLattice);

    resultPoint.x = x + 1;
    if (resultPoint.x == theLattice->Lx()) resultPoint.x = 0;
    resultPoint.y = y;

    return resultPoint;
}


Point Point::north()
{
    Point resultPoint(*theLattice);

    resultPoint.x = x;
    resultPoint.y = y - 1;
    if (resultPoint.y == -1) resultPoint.y = theLattice->Ly()- 1;

    return resultPoint;
}


Point Point::south()
{
    Point resultPoint(*theLattice);

    resultPoint.x = x;
    resultPoint.y = y + 1;
    if (resultPoint.y == theLattice->Ly()) resultPoint.y = 0;

    return resultPoint;
}


std::ostream & operator<<(std::ostream & s, const Point & p)
{
    s << "(" << p.x << "," << p.y << ")";
    return s;
}

