// This program copies and moves result files around to a new
// directory 
#include "definitions.H"
#include "auxilliary.H"
#include "Problem.H"

#include <iostream>



int main(int argc, char * argv[])
{
  std::cout.precision(30);
  Problem p;
  std::string dirNameBase="result";
  
  // 1. argument: file name for the problem definition 
  // 2. argument: first iteration to actually be performed
  //              i.e., give 1 to skip iteration 0 etc.
  if (argc>1)
    p.read(argv[1]);
  if (argc>2)
    {
      dirNameBase=(argv[2]);
    }
  int dirCounter=0;
  do
    dirCounter++;
  while ( systemCommand("test -d %s%s", dirNameBase.c_str(), makeString(dirCounter).c_str()) ==0    );
  std::string dirName=dirNameBase+makeString(dirCounter);
  std::cout << "Storing files in directory " << dirName << std::endl;
  
  
  systemCommand("mkdir %s", dirName.c_str());
  systemCommand("mv nohup.out %s", dirName.c_str());
  // Note: Use the old versions because calling store creates new log-files
  systemCommand("cp *.C %s",  dirName.c_str());
  systemCommand("cp *.cpp %s",  dirName.c_str());
  systemCommand("cp *.h %s",  dirName.c_str());
  systemCommand("cp *.H %s",  dirName.c_str());
  systemCommand("cp Testin.ini %s",  dirName.c_str());
  systemCommand("mv coating*.pov %s",  dirName.c_str());
  systemCommand("mv coating*.png %s",  dirName.c_str());
  systemCommand("mv tecplotmacro.mcr %s",  dirName.c_str());
  systemCommand("mv runmatlab_plot.m %s",  dirName.c_str());
  
  
  return 0;
}
