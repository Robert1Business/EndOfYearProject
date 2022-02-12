#ifndef _COATINGQUEUE
#define _COATINGQUEUE
#include "definitions.H"
#include "Point.H"
#include "Field.H"
#include <deque>

// A coating stores at each point a list of splat numbers together
// with the end height of the splat. Thus, there is no discretisation
// in vertical direction. This is very similar to the old 2D
// simulation model.
// The list that is stored at each point is a double-ended queue. This
// has the advantage that splats buried within can be eliminated from
// below. (They may be written to a file to be recoverable...)

// The coating element contains a splat number and the info where it
// ends. 
// Thus, if a point in the coating contains entries of
// (12, 0.1), (17, 0.3)
// this means that splat 12 starts at 0 and ends at 0.1, and splat 17
// starts at 0.1 and ends at 0.3 etc.
class CoatingElement
{
private:
  int splatNumber;
  double splatZ;

public:
  CoatingElement(int numi=0, double z=0.);
  friend class CoatingQueue;
  friend class Coating;
  friend std::ostream& operator<<(std::ostream& s, const CoatingElement& el);
  int num();
  double sZ();
  

  
};

std::ostream& operator<<(std::ostream& s, const CoatingElement& el);


class CoatingQueue: public std::deque<CoatingElement>
{
public:
  void push_back(int num, double z);
  CoatingQueue& operator+(const CoatingQueue & in);
  CoatingQueue& operator-(const CoatingQueue & in);

};

std::ostream& operator<<(std::ostream& s, const CoatingQueue& in);

std::istream& operator>>(std::istream& s, const CoatingQueue& in);



#endif
