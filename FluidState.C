#include "FluidState.H"


FluidState::FluidState()
{
    state = E;
}


FluidState& FluidState::operator+(const FluidState& inFluidState)
{
    error("Don´t add FluidState objects");
    return (*this);
}

FluidState& FluidState::operator+=(const FluidState& inFluidState)
{
    error("Don´t add FluidState objects");
    return (*this);
}

FluidState& FluidState::operator-(const FluidState& inFluidState)
{
    error("Don´t subtract FluidState objects");
    return (*this);
}


FluidState& FluidState::operator=(const FluidState& inFluidState)
{
    state = inFluidState.state;
    return (*this);
}


FluidState& FluidState::operator=(const FLUIDSTATE& inFluidState)
{
    state = inFluidState;
    return (*this);
}


std::ostream& operator<<(std::ostream& s, const FluidState& inState)
{
    return s <<" "<< int(inState.state);
}

std::istream& operator>>(std::istream& s, const FluidState& inState)
{
  error("Don't read in fluid states!");
    return s;
}

