CC=g++ -Wall
OPTIONS=-ggdb
INCLUDE=
LIBS=

all:  mc3d store #plotSlices


store: store.o Problem.o Control.o auxilliary.o 
	$(CC) -o store store.o Problem.o Control.o auxilliary.o  $(LDFLAGS)

store.o:  Problem.H store.C 
	${CC} ${OPTIONS} -c -o store.o store.C ${INCLUDE}

rng.o: rng.h rng.C
	${CC} ${OPTIONS} -c -o rng.o rng.C ${INCLUDE}

renamer.o: renamer.h renamer.cpp auxilliary.H
	${CC} ${OPTIONS} -c -o renamer.o renamer.cpp ${INCLUDE}

Vector3.o: Vector3.h Vector3.cpp
	${CC} ${OPTIONS} -c -o Vector3.o Vector3.cpp ${INCLUDE}

Plain3.o: Plain3.h Plain3.cpp Vector3.h
	${CC} ${OPTIONS} -c -o Plain3.o Plain3.cpp ${INCLUDE}

Solver.o: Solver.h Solver.cpp Vector3.h Plain3.h rng.h
	${CC} ${OPTIONS} -c -o Solver.o Solver.cpp ${INCLUDE}

pgm.o: pgm.h pgm.cpp
	${CC} ${OPTIONS} -c -o pgm.o pgm.cpp ${INCLUDE}

ppm.o: ppm.h ppm.cpp pgm.h
	${CC} ${OPTIONS} -c -o ppm.o ppm.cpp ${INCLUDE}

Point.o: Point.H Point.C definitions.H Lattice.H auxilliary.H
	${CC} ${OPTIONS} -c -o Point.o Point.C ${INCLUDE}

CoatingQueue.o: CoatingQueue.H CoatingQueue.C definitions.H Point.H Field.H
	${CC} ${OPTIONS} -c -o CoatingQueue.o CoatingQueue.C ${INCLUDE}

Coating.o: Coating.H Coating.C definitions.H Point.H Field.H
	${CC} ${OPTIONS} -c -o Coating.o Coating.C ${INCLUDE}

Field.o: Field.H Field.C definitions.H Point.H FluidState.H NeighbourState.H
	${CC} ${OPTIONS} -c -o Field.o Field.C ${INCLUDE}

Field3D.o: Field3D.H Field3D.C pgm.h Point.H Field.H rng.h Vector3.h
	${CC} ${OPTIONS} -c -o Field3D.o Field3D.C ${INCLUDE}

Lattice.o: Lattice.H Lattice.C definitions.H
	${CC} ${OPTIONS} -c -o Lattice.o Lattice.C ${INCLUDE}

Distribution.o: Distribution.H Distribution.C definitions.H
	${CC} ${OPTIONS} -c -o Distribution.o Distribution.C ${INCLUDE}

LatticeIterator.o: LatticeIterator.H LatticeIterator.C Lattice.H Point.H definitions.H
	${CC} ${OPTIONS} -c -o LatticeIterator.o LatticeIterator.C ${INCLUDE}

NeighbourIterator.o: NeighbourIterator.H NeighbourIterator.C Point.H definitions.H
	${CC} ${OPTIONS} -c -o NeighbourIterator.o NeighbourIterator.C ${INCLUDE}

FluidState.o: FluidState.H FluidState.C definitions.H auxilliary.H
	${CC} ${OPTIONS} -c -o FluidState.o FluidState.C ${INCLUDE}

Problem.o: Problem.H Problem.C definitions.H Control.H 
	${CC} ${OPTIONS} -c -o Problem.o Problem.C ${INCLUDE}

Particle.o: Particle.H Particle.C Point.H definitions.H Problem.H
	${CC} ${OPTIONS} -c -o Particle.o Particle.C ${INCLUDE}

Control.o: Control.H Control.C definitions.H
	${CC} ${OPTIONS} -c -o Control.o Control.C ${INCLUDE}

auxilliary.o: auxilliary.H auxilliary.C definitions.H
	${CC} ${OPTIONS} -c -o auxilliary.o auxilliary.C ${INCLUDE}

FieldProperty.o: FieldProperty.H FieldProperty.C Lattice.H LatticeIterator.H Field.H FluidState.H NeighbourState.H NeighbourIterator.H Point.H
	${CC} ${OPTIONS} -c -o FieldProperty.o FieldProperty.C ${INCLUDE}

Marker.o: Marker.H Marker.C definitions.H Lattice.H LatticeIterator.H Field.H FluidState.H NeighbourIterator.H NeighbourState.H FieldProperty.H Point.H renamer.h Solver.h Problem.H
	${CC} ${OPTIONS} -c -o Marker.o Marker.C ${INCLUDE}

NeighbourState.o: NeighbourState.H NeighbourState.C definitions.H auxilliary.H
	${CC} ${OPTIONS} -c -o NeighbourState.o NeighbourState.C ${INCLUDE}

Splat.o: Splat.H Splat.C definitions.H Point.H Field.H Lattice.H LatticeIterator.H NeighbourIterator.H FluidState.H NeighbourState.H FieldProperty.H Particle.H Problem.H Marker.H renamer.h 
	${CC} ${OPTIONS} -c -o Splat.o Splat.C ${INCLUDE}

main.o: main.C Splat.H Field.H Lattice.H definitions.H Problem.H Field.H Field3D.H rng.h pgm.h ppm.h
	${CC} ${OPTIONS} -c -o main.o main.C ${INCLUDE}


mc3d: main.o Point.o Field.o Lattice.o LatticeIterator.o NeighbourIterator.o FluidState.o Problem.o Particle.o Control.o auxilliary.o FieldProperty.o NeighbourState.o Splat.o rng.o pgm.o Field3D.o Marker.o renamer.o Solver.o Vector3.o Plain3.o ppm.o CoatingQueue.o  Coating.o Distribution.o
	${CC} ${OPTIONS} -o mc3d main.o Point.o Field.o Lattice.o LatticeIterator.o NeighbourIterator.o FluidState.o Problem.o Particle.o Control.o auxilliary.o FieldProperty.o NeighbourState.o Splat.o rng.o pgm.o Field3D.o Marker.o renamer.o Solver.o Vector3.o Plain3.o ppm.o CoatingQueue.o  Coating.o Distribution.o ${INCLUDE} ${LIBS}

#plotSlices.o: plotSlices.C Splat.H Field.H Lattice.H definitions.H Problem.H Field.H Field3D.H rng.h pgm.h ppm.h
#	${CC} ${OPTIONS} -c -o plotSlices.o plotSlices.C ${INCLUDE}


#plotSlices: plotSlices.o Point.o Field.o Lattice.o LatticeIterator.o NeighbourIterator.o FluidState.o Problem.o Particle.o Control.o auxilliary.o FieldProperty.o NeighbourState.o Splat.o rng.o pgm.o Field3D.o Marker.o renamer.o Solver.o Vector3.o Plain3.o ppm.o
#	${CC} ${OPTIONS} -o plotSlices plotSlices.o Point.o Field.o Lattice.o LatticeIterator.o NeighbourIterator.o FluidState.o Problem.o Particle.o Control.o auxilliary.o FieldProperty.o NeighbourState.o Splat.o rng.o pgm.o Field3D.o Marker.o renamer.o Solver.o Vector3.o Plain3.o ppm.o ${INCLUDE} ${LIBS}



