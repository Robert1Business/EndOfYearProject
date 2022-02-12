#include "Control.H"

void removeBlanks(std::string & token)
{
  // Remove blanks
  int pos = token.find(" ");
  while ( (pos <100 ) &&  (pos>=0))
    {
      token.replace(pos,1,"");
      pos = token.find(" ");
    }
  return ;
}

// The next routine splits the controlString in two parts at the
// #-sign, if present
std::string splitShorthand(std::string & controlString)
{
  std::string shorthand;
  int pos = controlString.find("#");
  if ( (pos <100 ) &&  (pos>=0))
    {
    shorthand = controlString.substr(pos+1);
    controlString = controlString.substr(0,pos);
    }
  else
    shorthand = controlString;
  removeBlanks(shorthand);
  removeBlanks(controlString);
  return shorthand;
}

ControlElement::ControlElement()
{
  return;
}

ControlElement::ControlElement(const std::string & cs,const  std::string &dv,  
			       double * pt ):
  controlString(cs),defaultValue(dv), ptr((void*)pt)
{
  type = DOUBLE;
  defaultChanged = false;
  trueValue = defaultValue;
  shorthand = splitShorthand(controlString);
  return;
}

ControlElement::ControlElement(const std::string & cs,const  std::string &dv,  
			       int * pt ):
  controlString(cs),defaultValue(dv), ptr((void*)pt)
{
  type = INT;
  defaultChanged = false;
  trueValue = defaultValue;
  shorthand = splitShorthand(controlString);
  return;
}

ControlElement::ControlElement(const std::string & cs,const  std::string &dv,  
			       float * pt ):
  controlString(cs),defaultValue(dv), ptr((void*)pt)
{
  type = FLOAT;
  defaultChanged = false;
  trueValue = defaultValue;
  shorthand = splitShorthand(controlString);
  return;
}

ControlElement::ControlElement(const std::string & cs,const  std::string &dv,  
			       std::string * pt ):
  controlString(cs),defaultValue(dv), ptr((void*)pt)
{
  type = STRING;
  defaultChanged = false;
  trueValue = defaultValue;
  shorthand = splitShorthand(controlString);
  return;
}

Control::Control()
{
  contvec.reserve(10);
}

int Control::readFile(const std::string & filename)
{
  contvec.reserve(10);
  std::ifstream instr((filename).c_str());
  char line[100];
  // Read a whole line
  while(instr.getline(line,100,'\n'))
  {
    std::string theline = line;

    // Ignore commentlines, i.e. everything from a "#" onwards
    int pos=theline.find("#");
    if ((pos <100) && (pos>=0)) 
      {
	theline = theline.substr(0,pos);
	if (theline.size()==0) continue;
      }
    // Break after a line starting with "!!"
    pos=theline.find("!!");
    if ((pos <100) && (pos>=0)) 
      {
	theline = theline.substr(0,pos);
	if (theline.size()==0) break;
      }
    // Ignore empty lines
    removeBlanks(theline);
    if (theline.size()<1) continue;
    // Now search for the "="-sign
    pos=theline.find("=");
    if ((pos >=100 ) ||  (pos<0))
      {
	std::cerr << "Error(Control::readFile): Non-comment-line" <<
	  " without an =-sign!\n This line will be ignored!" << std::endl;
	continue;
	
      }
    // So the =-sign is at position pos
    // Now split into the token (variable name) and its value
    std::string token, value;
    token = theline.substr(0,pos);
    value = theline.substr(pos+1);
    removeBlanks(token);
    removeBlanks(value);
    
    // Search for the token string in the variable name list
    int found = false;
    for (int i=0;i<contvec.size();i++)
      {
	pos = contvec[i].controlString.find(token);
	if ( (pos <100 ) &&  (pos>=0))
	  {
	    found =true;
	    if (contvec[i].defaultValue != value)
	      {
		contvec[i].trueValue = value;
		contvec[i].defaultChanged = true;
	      }
	    else
	      contvec[i].defaultChanged = false;
	    break;
	  }
      }
    if (found == false)
      {
	std::cerr << "Error(Control::readFile): Variable name " << token <<
	  " unknown!\n This line will be ignored!" << std::endl;
      }
    
  }
  
  return 0;
}

// Assign all values in the control structure
void Control::assign()
{
  // Here follows a loop that assgins the variable values correctly. 
  for (int i=0;i<contvec.size();i++)
    {
      switch(contvec[i].type)
	{
	case DOUBLE:
	  *(static_cast<double*>(contvec[i].ptr)) =
	    atof(contvec[i].trueValue.c_str());
	  break;
	case FLOAT: 
	  *(static_cast<float*>(contvec[i].ptr)) =
	    atof(contvec[i].trueValue.c_str());
	  break;
	case INT:
	  *(static_cast<int*>(contvec[i].ptr)) =
	    atoi(contvec[i].trueValue.c_str());
	  break;
	case STRING:
	  *(static_cast<std::string*>(contvec[i].ptr)) = contvec[i].trueValue;
	}
      
    }
  return;
  
}



void Control::push(const ControlElement & in)
{
  contvec.push_back(in);
  return;
}

void Control::push(const std::string & cs,const  std::string &dv,  
		   int * pt )
{
  contvec.push_back(ControlElement(cs,dv,pt));
  return;
}

void Control::push(const std::string & cs,const  std::string &dv,  
		   double * pt )
{
  contvec.push_back(ControlElement(cs,dv,pt));
  return;
}

void Control::push(const std::string & cs,const  std::string &dv,  
		   float * pt )
{
  contvec.push_back(ControlElement(cs,dv,pt));
  return;
}

void Control::push(const std::string & cs,const  std::string &dv,  
		   std::string * pt )
{
  contvec.push_back(ControlElement(cs,dv,pt));
  return;
}


std::string Control::makeFilename(const std::string & filebase)
{
  time_t mTime;
  time(&mTime);
  std::string filename = filebase;
  for (int i=0;i<contvec.size();i++)
    if ((contvec[i].defaultChanged == true) &&
	(contvec[i].type != STRING)) 
      filename = filename+contvec[i].shorthand
	+contvec[i].trueValue;
  char datestring[13];
  sprintf(datestring,"-%i-%i-%i",localtime(&mTime)->tm_mday, 
		     localtime(&mTime)->tm_mon+1,
		     localtime(&mTime)->tm_year-100);
  filename = filename + datestring;
  removeBlanks(filename);
  
  WRITE(filename);
  
  return filename;
}


void ControlElement::write()
{
  std::cout << controlString << "(" << shorthand << ") = " << trueValue << " (" <<
    defaultValue << ")" << std::endl;
  return;
}

void Control::write()
{
  for (unsigned int i=0; i<contvec.size(); i++)
    contvec[i].write();
  return;
}
