#include "NeighbourIterator.H"


NeighbourIterator::NeighbourIterator(const Point & inPoint, NUMBEROFNEIGHBOURS numNeighIn)                    //constructor
{
    p = inPoint;
    dir = 0;
    numNeigh=numNeighIn;
//    WRITE(p.x);
//    WRITE(p.y);
}


NeighbourIterator& NeighbourIterator::operator++(int)  //operator++ is postfix with (int) -> means "a++"
{                                                      //erst zurückgeben und dann erhöhen
    dir++;    
    return(*this);
}


Point NeighbourIterator::operator*()
{
    Point resultPoint(*(p.theLattice));  
    //p.theLattice -> Zeiger auf Lattice -> mit * den Wert holen ??

    switch(direction())
    {
	case North :                                           //dir=0
	    resultPoint = p + Point(0,-1,*(p.theLattice));
	    //std::cout <<"case North" << std::endl;
	    break;
	case East :                                            //dir=1
	    resultPoint = p + Point(1,0,*(p.theLattice));
	    //std::cout <<"case East" << std::endl;
	    break;
	case South :                                           //dir=2
	    resultPoint = p + Point(0,1,*(p.theLattice));
	    //std::cout <<"case South" << std::endl;
	    break;
	case West :                                            //dir=3
	    resultPoint = p + Point(-1,0,*(p.theLattice));
	    //std::cout <<"case West" << std::endl;
	    break;

	case NorthWest :                                       //dir=4
	    resultPoint = p + Point(-1,-1,*(p.theLattice));
	    break;
	case NorthEast :                                       //dir=5
	    resultPoint = p + Point(1,-1,*(p.theLattice));
	    break;
	case SouthWest :                                       //dir=6
	    resultPoint = p + Point(-1,1,*(p.theLattice));
	    break;
	case SouthEast :                                       //dir=7
	    resultPoint = p + Point(1,1,*(p.theLattice));
	    break;

	case NorthNorth :                                      //dir=8
	    resultPoint = p + Point(0,-2,*(p.theLattice));
	    break;
	case EastEast :                                        //dir=9
	    resultPoint = p + Point(2,0,*(p.theLattice));
	    break;
	case SouthSouth :                                      //dir=10
	    resultPoint = p + Point(0,2,*(p.theLattice));
	    break;
	case WestWest :                                        //dir=11
	    resultPoint = p + Point(-2,0,*(p.theLattice));
	    break;

	case End :                                             //dir=12
	    std::cout <<"ERROR: case End" << std::endl;
	    break;

	default : std::cout <<"This can't happen" << std::endl;
    }

    return resultPoint;
}



DIRECTION NeighbourIterator::direction()
{
    return DIRECTION(dir);
}


void NeighbourIterator::begin()
{
    dir = 0;
}


bool NeighbourIterator::end()
{
    if ((numNeigh == FOUR) && (dir>3))
    {
	return true;
    }
    else if ((numNeigh == EIGHT) && (dir>7))
    {
	return true;
    }
    else if ((numNeigh == TWELVE) && (dir>11))
    {
	return true;
    }
    return false;
}




