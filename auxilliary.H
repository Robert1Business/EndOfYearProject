#ifndef _auxilliary
#define _auxilliary
#include "definitions.H"

void error(char *fmt, ...);

void warning(char *fmt, ...);

int systemCommand(char *fmt, ...);


real roundRoughly(real x);

std::string itoa(int value, int base=10);

void rankVector(std::vector<double> & h, std::vector<int> & rank);

std::string makeString(int i, int n=3);



#endif
