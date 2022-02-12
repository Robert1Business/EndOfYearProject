#include "Particle.H"



Particle::Particle(real inRadius, real inVelocity, real inViscosity, real inDensity,
		   real inImpactAngle, real inDirectionAngle, bool inMolten, int inxPosition, int inyPosition, real inSlowDownFactor, real inSlowDownNorm)
{
    radius = inRadius;
    velocity = inVelocity;
    viscosity = inViscosity;
    density = inDensity;
    impactAngle = inImpactAngle;
    directionAngle = inDirectionAngle;
    molten = inMolten;
    xPosition = inxPosition;
    yPosition = inyPosition;
    slowDownFactor = inSlowDownFactor;
    slowDownNorm = inSlowDownNorm;
    calcVolume();
    calcMadejski();
    
    return;
}

Particle::Particle(real inRadius, real inVelocity, real inViscosity, real inDensity,
		   real inImpactAngle, real inDirectionAngle, bool inMolten, Point inPosition, real inSlowDownFactor, real inSlowDownNorm)
{
    radius = inRadius;
    velocity = inVelocity;
    viscosity = inViscosity;
    density = inDensity;
    impactAngle = inImpactAngle;
    directionAngle = inDirectionAngle;
    molten = inMolten;
    xPosition = inPosition.x;
    yPosition = inPosition.y;
    slowDownFactor = inSlowDownFactor;
    slowDownNorm = inSlowDownNorm;
    calcVolume();
    calcMadejski();
    return;
}



real Particle::Reynolds()
{
    real Re = (density * velocity * 2. * radius / viscosity);
    return Re;
}


void Particle::calcVolume()
{
    theVolume= M_PI * pow(radius,3) * 4 / 3;
    return;
}

real Particle::volume()
{
    return theVolume;
}

real Particle::theRadius()
{
    return radius;
}


real Particle::heightMadejski()
{
  return hMadejski;
}

void Particle::calcMadejski()
{
    real xi = 1.2941 * pow(Reynolds(),0.2);

    real splatDiam = xi * 2 * radius;
    real splatSurface = M_PI * pow(splatDiam,2) / 4; 

    hMadejski = volume() / splatSurface;

    return ;
}

