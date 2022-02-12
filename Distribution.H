#ifndef _DISTRIBUTION
#define _DISTRIBUTION
#include "definitions.H"
#include <algorithm>
#include "auxilliary.H"

// First entry is the cumulative frequency, second is the value at the
// end of the interval
// All this could (should?) be a template
class Distribution;

class tableEntry
{
  real key;
  real val;
public:
  tableEntry(real keyIn, real valIn);
  bool operator<(const tableEntry& that) const;
  friend class Distribution;
  
};



// Class for statistical distribution of cumulative data
// allows to find a - linearly interpolated - value of the dependent
// variable when a number between 0 and 1 is given
// It is simply a table with two entries
class Distribution
{
private:
  std::vector<tableEntry> table;
public:
  Distribution();
  Distribution(const std::string & filename);
  void read(const std::string & filename);
  
  void push_back(real keyIn, real valIn);
  void push_back(const tableEntry & ent);
  int findMatching(tableEntry ent);
  void write();
  real interpolateValue(real key)  ;
  
};



#endif
