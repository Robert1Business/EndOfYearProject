#include "Problem.H"

void Problem::pushDefaults()
{
  control.push("numParticles#Np","100",    &numParticles);
  control.push("numLxPoints # Nlx ","3000",    &numLxPoints);
  control.push("numLyPoints # Nly ","3000",    &numLyPoints);
  control.push("dx","1.25e-6",    &dx);
  control.push("dy","1.25e-6",    &dy);
  // These data are from Bertagnolli etal., 1995, for Zirconia
  // Adapted to make the splats flatter 
  control.push("partMeanRadius#MRad","150.e-6",    &partMeanRadius);
  control.push("partMeanVel#MVel","150.",    &partMeanVel);
  // The radii may be read from a file - set radiusDistribution to 1
  control.push("radiusDistribution","0",    &radiusDistribution);
  control.push("distributionFilename","__NO_FILE__",    &distributionFilename);
  // Note: Error on the next line (factor e-2 missing) corrected on 22/2/05
  control.push("partViscosity#Visc","4.2e-2",    &partViscosity);
  control.push("partDensity#Dens","5.55e3",    &partDensity);
  control.push("filebase","coat",    &filebase);
  control.push("seed","1",    &seed);
  control.push("epsOutput #Out","0",    &epsOutput);
  control.push("impactAngle","0",    &impactAngle);
  control.push("directionAngle","0",    &directionAngle);

  // 19/08/08: Made these Problem variables
  // 01/09/08: markerRadius is now dependend on dx
  control.push("markerRadius#MarkerRad","2.",    &markerRadius);
  control.push("numberOfRays","1440",    &numberOfRays);
  control.push("markerPerCell","1",    &markerPerCell);
  control.push("markerWidth","1.",    &markerWidth);

  control.push("numberOfFingers","0",    &numberOfFingers);
  control.push("fingerLength","1.",    &fingerLength);
  
  //Hauptfeld
  control.push("dimXMain","200",    &dimXMain);
  control.push("dimYMain","200",    &dimYMain);
  control.push("dimZMain","200",    &dimZMain);
  control.push("dz","2.e-7",    &dz);
  control.push("moltenFraction","0.",    &moltenFraction);
  
  //Parameter zum Abbremsen
  control.push("slowDownFactor","1.",    &slowDownFactor);
  control.push("slowDownNorm","0.",    &slowDownNorm);

  // Stufen-Parameter
  // Old values for radius 10mu
  //   control.push("splashValue","4.e-6",    &splashValue);
  //   control.push("medDelta","1.5e-6",    &medDelta);
  //   control.push("smallDelta","1.e-6",    &smallDelta);
  // splashValue et al. are now relative to the radius!
  control.push("splashValue","0.4",    &splashValue);
  control.push("medDelta","0.15",    &medDelta);
  control.push("smallDelta","0.1",    &smallDelta);
  control.push("sniffAheadDistance","5.",    &sniffAheadDistance);

  // writeTecplot: 0 no writing, 1 only resulttec-file, 2 call tec360
  // and do a single plot
  control.push("writeTecplot","0",    &writeTecplot);
  control.push("writeMatlab","0",    &writeMatlab);
  control.push("writeGnuplot","0",    &writeGnuplot);
  control.push("writePovray","0",    &writePovray);
  // If write3D is zero, no 3D info will be written - also no matlab or
  // tecplot 
  control.push("write3D","1",    &write3D);
  return;
}

// There are two constructors, one without any parameters, which
// constructs the problem with default parameters, and one to read
// from a file.  

Problem::Problem()
{
  pushDefaults();
  assignAll();
  return;
};


Problem::Problem(const std::string & fileIn)
{
  pushDefaults();
  read(fileIn);
  return;
  
};

//@
void Problem::read(const std::string & fileIn)
{
  
  // If wanted, read data from file
  control.readFile(fileIn);
  // The next statement is necessary to ensure that the default data
  // are really written
  assignAll();
  return;
}

//@
void Problem::assignAll()
{
  control.assign();
  filename = control.makeFilename(filebase);
  srand(seed);
  return;
}

void Problem::write()
{
  control.write();
  return;
}
