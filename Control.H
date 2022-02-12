#ifndef _CONTROL_H
#define _CONTROL_H
#include "definitions.H"
#include <iostream>
#include <string>

enum TYPE {DOUBLE, FLOAT, INT, STRING};

class ControlElement
{
public:
  std::string controlString;
  std::string shorthand;
  std::string defaultValue;  // The default
  std::string trueValue;     // and the true value the variable has
  TYPE type;
  void * ptr;
  int defaultChanged;
  
  ControlElement();
  ControlElement(const std::string & cs,const  std::string & dv,
			       double * pt );
  ControlElement(const std::string & cs,const  std::string & dv,
			       int * pt );
  ControlElement(const std::string & cs,const  std::string &dv,
				 float * pt );
  ControlElement(const std::string & cs,const  std::string &dv,
				 std::string * pt );
  void write();
  friend class Problem;
  friend class Control;
  friend class Psfile;
};

class Control
{
public:
  std::vector<ControlElement> contvec;
  Control();
  int readFile(const std::string & filename);
  void assign();
  void push(const ControlElement & in);
  void push(const std::string & cs,const  std::string &dv,  
		     int * pt );
  void push(const std::string & cs,const  std::string &dv,  
		     double * pt );
  void push(const std::string & cs,const  std::string &dv,  
		     float * pt );
  void push(const std::string & cs,const  std::string &dv,  
		     std::string * pt );
  std::string makeFilename(const std::string & filebase);
  void write();
};


#endif
