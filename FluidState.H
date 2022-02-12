#ifndef _FLUIDSTATE_H
#define _FLUIDSTATE_H

#include "definitions.H"
#include "auxilliary.H"

enum FLUIDSTATE {E, EN, FS, F};


class FluidState
{
public:

    FLUIDSTATE state;

    FluidState();
    FluidState& operator+(const FluidState & inFluidState);
    FluidState& operator+=(const FluidState & inFluidState);
    FluidState& operator-(const FluidState & inFluidState);
    FluidState& operator=(const FluidState & inFluidState);
    FluidState& operator=(const FLUIDSTATE & inFluidState);


};

std::ostream& operator<<(std::ostream& s, const FluidState& state);
std::istream& operator>>(std::istream& s, const FluidState& inState);



#endif



