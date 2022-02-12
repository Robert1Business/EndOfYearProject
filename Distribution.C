#include "Distribution.H"
tableEntry::tableEntry(real keyIn, real valIn)
{
  key=keyIn;
  val=valIn;
}


bool tableEntry::operator<(const tableEntry& that) const
{
  return key < that.key;
}



Distribution::Distribution()
{
  table.push_back(tableEntry(0.,0.));
}

Distribution::Distribution(const std::string & filename)
{
  std::cout << "READING distribution" << std::endl;
  
  std::ifstream f(filename.c_str());
  real ki,vi;
  while (f.good())
    {
      f >> ki >> vi;
      if (f.eof()) break;
      table.push_back(tableEntry(ki,vi));
    }
  sort(table.begin(), table.end());
  if ( fabs(table.back().key- 1.) > 1.e-6)
    warning("table does not end with 1. Read from file", filename.c_str());
  if ( fabs(table.front().key) > 1.e-6)
    table.push_back(tableEntry(0.,0.));
  sort(table.begin(), table.end());
  write();
  
}
void Distribution::read(const std::string & filename)
{
  std::ifstream f(filename.c_str());
  real ki,vi;
  while (f.good())
    {
      f >> ki >> vi;
      if (f.eof()) break;
      table.push_back(tableEntry(ki,vi));
    }
  sort(table.begin(), table.end());
  if ( fabs(table.back().key- 1.) > 1.e-6)
    warning("table does not end with 1. Read from file", filename.c_str());
  if ( fabs(table.front().key) > 1.e-6)
    table.push_back(tableEntry(0.,0.));
  sort(table.begin(), table.end());
  write();
  
}



int Distribution::findMatching(tableEntry ent)
{
  for (int i=0;i<table.size(); i++)
    if (table[i].key == ent.key)
      return i;
  return -1;
}

void Distribution::push_back(real keyIn, real valIn)
{
  table.push_back(tableEntry(keyIn, valIn));
}

void Distribution::push_back(const tableEntry & ent)
{
  if (findMatching(ent) != -1)
    error("Trying to push_back table value %12.8g which already exists", ent.key);
  table.push_back(ent);
  sort(table.begin(), table.end());
}

void Distribution::write()
{
  for (int i=0;i<table.size(); i++)
    std::cout << table[i].key << " " << table[i].val << std::endl;
}

real Distribution::interpolateValue(real key)
{
  if (key <table.front().key)
    {
      warning("Key %12.8g to interpolate smaller than lowest key", key);
      return table.front().val;
    }
  if (key >table.back().key)
    {
      warning("Key %12.8g to interpolate larger than largest key", key);
      return table.back().val;
    }
    for (int i=0;i<table.size()-1; i++)
      if ( (key > table[i].key) &&  (key <= table[i+1].key))
	{
	  // Interpolate linearly
	  real result = (key - table[i].key)/(table[i+1].key- table[i].key) *
	    (table[i+1].val- table[i].val) + table[i].val;
	  return result;
	}
    error("Key value %12.8g not found. This cannot happen!", key);
    return -1.e-10;
    
}
