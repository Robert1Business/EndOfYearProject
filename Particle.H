#ifndef _PARTICLE
#define _PARTICLE
#include "Point.H"
#include "definitions.H"
#include "Problem.H"

class Particle
{
private:
    real radius;
    real velocity;
    real viscosity;
    real density;
  real theVolume, hMadejski;
    int xPosition;
    int yPosition;

public:

    Particle(real inRadius, real inVelocity, real inViscosity, real inDensity,
	     real inImpactAngle, real inDirectionAngle, bool inMolten, int inxPosition, int inyPosition,
         real inSlowDownFactor, real inSlowDownNorm);
    Particle(real inRadius, real inVelocity, real inViscosity, real inDensity,
		   real inImpactAngle, real inDirectionAngle, bool inMolten, Point inPosition,
           real inSlowDownFactor, real inSlowDownNorm);
  real theRadius();
  
    real Reynolds();
    real volume();
    void calcVolume();
    real heightMadejski();
    void calcMadejski();
    real impactAngle;
    real directionAngle;
    real slowDownFactor;
    real slowDownNorm;
    
    bool molten;
};


#endif
