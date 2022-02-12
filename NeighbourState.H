#ifndef _NEIGHBOURSTATE_H
#define _NEIGHBOURSTATE_H

#include "definitions.H"
#include "auxilliary.H"

enum NEIGHBOURSTATE {noNSWO, N, O, S, W, NW, NO, SW, SO, NSW, NSO, NWO, SWO,
			 NSWO, NS, WO};

class NeighbourState
{
public:
    NEIGHBOURSTATE stateNeigh;

    NeighbourState();
    NeighbourState& operator+(const NeighbourState & inNeigh);
    NeighbourState& operator-(const NeighbourState & inNeigh);
    NeighbourState& operator=(const NeighbourState & inNeigh);
    NeighbourState& operator=(const NEIGHBOURSTATE & inNeigh);
    
    

};

std::ostream& operator<<(std::ostream& s, const NeighbourState& stateNeigh);
std::istream& operator>>(std::istream& s, const NeighbourState& inState);


#endif
