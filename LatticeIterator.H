#ifndef _LATTICEITERATOR
#define _LATTICEITERATOR
#include "definitions.H"
#include "Lattice.H"
#include "Point.H"

class LatticeIterator  
{

private:
    Lattice * theLattice; //the iterator has to know to which lattice it belongs 
    int Itx,Ity;          // -> pointer (analogue Point, Field)
    
    //hinzuaddierte Werte bei ++-Operator
    int dx_;
    int dy_;
    
    //Grosse des abzudeckenden Iterationsfeldes
    int Intx_;
    int Inty_;

public:
    int getX(void);
    int getY(void);
    
    LatticeIterator(Lattice & inLattice);     //constructor for the lattice iterator
    LatticeIterator(Lattice & inLattice, int dx, int dy); //Konstruktor mit zusaetslicher Schrittweiten dx/ dy

    LatticeIterator& operator++(int);
    LatticeIterator& operator=(const LatticeIterator& that); //Ueberschreiben von Iteratoren
    Point operator *();

    void begin();
    bool end();

};

#endif
