//legt Eigenschaften des Feldes an
#ifndef _FIELDPROPERTY_H
#define _FIELDPROPERTY_H

#include "Lattice.H"
#include "LatticeIterator.H"
#include "Field.H"
#include "FluidState.H"
#include "NeighbourState.H"
#include "NeighbourIterator.H"
#include "Point.H"


class FieldProperty
{
public:
      static void loopNSWO(Lattice& theLattice, LatticeIterator& LatIter, Field<FluidState>& Fluid, Field<NeighbourState>& Neighbours);
      static void loopENtoFS(Lattice& theLattice,LatticeIterator& LatIter,Field<real>& depositHeight,Field<FluidState>& Fluid);
      static void loopOnlyFS(Lattice& theLattice,LatticeIterator& LatIter,Field<FluidState>& Fluid);
      static real numberNSWO(Lattice& theLattice,LatticeIterator& LatIter, Field<NeighbourState>& Neighbours, Point inPoint);
};

#endif
