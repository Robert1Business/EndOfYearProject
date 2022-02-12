#ifndef _PROBLEM_H
#define _PROBLEM_H
#include "definitions.H"
#include "Control.H"

class Problem
{
public:
    int numParticles;         
    int numLxPoints;
    int numLyPoints;
    real dx;
    real dy;
    real partMeanRadius;
    //real partMeanRadiusMIN;
    //real partMeanRadiusMAX;
    real markerRadius;
    real partMeanVel;;
    real partViscosity; 
    real partDensity;
    real impactAngle;
    real directionAngle;
    real slowDownFactor;
    real slowDownNorm;
    int epsOutput;
    int dimXMain;
    int dimYMain;
    int dimZMain;
    real dz;
    int seed;
  real moltenFraction;
  real splashValue, medDelta, smallDelta, sniffAheadDistance;
  real markerWidth;
  int numberOfRays, markerPerCell;
  int numberOfFingers;
  real fingerLength;
  int radiusDistribution;
  std::string distributionFilename;
  
  
  int writeTecplot, writeMatlab, writeGnuplot, write3D, writePovray;
  

    std::string filebase;
    std::string filename;
    std::string makeFilename(std::string filebase);

    Control control;
    Problem();
    Problem(const std::string & fileIn);

    void read(const std::string & filename);
    void pushDefaults();
    void assignAll();
    void write();
};

extern Problem theProblem;

#endif
