#include "NeighbourState.H"


NeighbourState::NeighbourState()
{
    stateNeigh = N;
}


NeighbourState& NeighbourState::operator+(const NeighbourState& inNeigh)
{
    error("Don´t add NeighbourState objects");
    return (*this);
}

NeighbourState& NeighbourState::operator-(const NeighbourState& inNeigh)
{
    error("Don´t subtract NeighbourState objects");
    return (*this);
}


NeighbourState& NeighbourState::operator=(const NeighbourState& inNeigh)
{
    stateNeigh = inNeigh.stateNeigh;
    return (*this);
}


NeighbourState& NeighbourState::operator=(const NEIGHBOURSTATE& inNeigh)
{
    stateNeigh = inNeigh;
    return (*this);
}


std::ostream& operator<<(std::ostream& s, const NeighbourState& inState)
{
    return s <<" "<< int(inState.stateNeigh);
}

std::istream& operator>>(std::istream& s, const NeighbourState& inState)
{
  error("Don't read in neighbour states!");

    return s;
}

