// This file contains auxiliary functions that do not belong anywhere
// else. They are all self-explanatory.


#include "definitions.H"
#include "auxilliary.H"

double degToRad(double angle)
{
  return (M_PI*angle)/180.;
}; 

double radToDeg(double angle)
{
  return (angle/M_PI*180.);
}; 

void error(char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  
  char errortext[200];
  vsprintf(errortext,fmt,args);
  std::cerr << "ERROR" << errortext << std::endl;
  va_end(args);
  exit(1);
  return ;
};

void warning(char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  
  char errortext[200];
  vsprintf(errortext,fmt,args);
  std::cerr << "WARNING" << errortext << std::endl;
  va_end(args);
  return ;
};

int systemCommand(char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  
  char command[200];
  vsprintf(command,fmt,args);
  int return_code = system(command);
  va_end(args);
  return return_code;
};


// This rounds on 7 digits.
real roundRoughly(real x)
{
  int lexp = int(log10(x));
  real xShift = x / pow(10.,lexp);
  float xnew = float(int(xShift*1000000));
  //@  std::cout << OUT(x) << OUT(lexp) << OUT(xShift) << OUT(xnew) ;@
  
  xnew /= 1000000;
  
  return xnew*float(pow(10.,lexp));
  
}

// Stolen from  http://www.jb.man.ac.uk/~slowe/cpp/itoa.html
std::string itoa(int value, int base) 
{
  enum { kMaxDigits = 35 };
  std::string buf;
  buf.reserve( kMaxDigits ); // Pre-allocate enough space.
  // check that the base if valid
  if (base < 2 || base > 16) return buf;
  int quotient = value;
  // Translating number to string with base:
  do
    {
      buf += "0123456789abcdef"[ std::abs( quotient % base ) ];
      quotient /= base;
    }
  while ( quotient );
  // Append the negative sign for base 10
  if ( value < 0 && base == 10) buf += '-';
  std::reverse( buf.begin(), buf.end() );
  return buf;
}

// Calculate a rank vector, containing a list of the sorted indices
// The rank is descending, i.e. the first element is the largest
// This should really be a standard vector function!!
void rankVector(std::vector<double> & h, std::vector<int> & rank)
{
  int size = h.size();
  // Start the rank vector from scratch
  rank.clear();
  for (int i=0;i<size;i++)
    rank.push_back(i);
  // Outer loop for sorting
  for (int count=size;count>1;count--)
    {
      double minVal = 1.e10;
      int minRank=-1;
      // Inner loop
      for (int i=0;i<count;i++)
	{
	  if (h[rank[i]] < minVal)
	    {
	      minVal = h[rank[i]];
	      minRank = i;
	    }
	}
      // The minimal rank is found, now switch this with the last
      // active element
      int aux= rank[count-1];
      rank[count-1] = rank[minRank];
      rank[minRank] = aux;
    }
  return;
}

// This makes an n-digit string from an integer, i.e., converts 12
// to 012 etc.
std::string makeString(int i, int n)
{
  WRITE(n);
  
  std::string result="";
  if (i==0)
    {
      for (int ii=0;ii<n;ii++)
	{
	  result+="0";
	  WRITE(ii);
	}
      return result;
    }
  
  int numDigits = int(log10(double(i)))+1;
  for (int ii=numDigits;ii<n;ii++)
    {
      result+="0";
    }
  result +=itoa(i);
  return result;
  
}



