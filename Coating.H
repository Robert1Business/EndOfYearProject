#ifndef _COATING
#define _COATING
#include "definitions.H"
#include "Point.H"
#include "CoatingQueue.H"
#include "Problem.H" 
#include "pgm.h"
#include "rng.h"
#include "ppm.h"

// A coating stores at each point a list of splat numbers together
// with the end height of the splat. Thus, there is no discretisation
// in vertical direction. This is very similar to the old 2D
// simulation model.

class Coating: public Field<CoatingQueue>
{
public:
  Coating(Lattice &inLattice);
  void writePovrayFile(string filename, Problem & theProblem);
  
  
};

std::ostream& operator<<(std::ostream& s, const Coating& coat);

#endif
