//Main program for three-dimensional MC simultion of a tbc coating process

#include "Splat.H"
#include "Field.H"
#include "Lattice.H"
#include "definitions.H"
#include "Problem.H"
#include "Field.H"
#include "Field3D.H"
#include "Coating.H"
#include "Distribution.H"
#include "rng.h"
#include "pgm.h"
#include "ppm.h"

// The deposition routine gets the impact point, the current splat
// number and its height as a field, the Ground height and the Coating.
// It adds the current splat onto ground and pushes the
// CoatingElements onto theCoating
void depositSplat(Point p, int splatNumber, Field<real> & field,
		  Field<real> & Ground, Coating & theCoating)
{
  for(int x = 0; x < field.getX(); x++)
    for (int y = 0; y < field.getY(); y++)
      {
	// MB: Changed /2 to /2. to make sure that doubles are used
	int xdirection = (x + p.x - (int)round(double(field.getX())/2.)
			  + Ground.getX()) % Ground.getX();
	int ydirection = (y + p.y - (int)round(double(field.getY())/2.)
			  + Ground.getY()) % Ground.getY();
	Ground[xdirection][ydirection] = Ground[xdirection][ydirection] + field(x,y);
	// Now push back this onto theCoating, but only if
	// significantly different from zero
	if (field(x,y) > heightTolerance)
	  {
	    theCoating[xdirection][ydirection].push_back(splatNumber, Ground[xdirection][ydirection]);
	  }
	
      }

  if (splatNumber%10==0)
    
    for(int x = 1; x < theCoating.getX()-1; x++)
      for (int y = 1; y < theCoating.getY()-1; y++)
	{
	  // Occasionally, remove unneded elements from theCoating to
	  // save memory
	  // Check the height of the four next neighbours
	  double hN=0, hS=0, hE=0, hW=0;
	  if (   theCoating[x][y+1].size()>0)
	    hN = theCoating[x][y+1].back().sZ();
	  
	  if (   theCoating[x][y-1].size()>0)
	    hS = theCoating[x][y-1].back().sZ();
	  
	  if (   theCoating[x-1][y].size()>0)
	    hW = theCoating[x-1][y].back().sZ();
	  
	  if (   theCoating[x+1][y].size()>0)
	    hE = theCoating[x+1][y].back().sZ();
// 	  if (x==80 && y==213)
// 	    std::cout << "At 80,213" << OUT(hN) << OUT(hS) << OUT(hE) << OUT(hW)
// 		      << OUT(theCoating[x][y].front().sZ());
	  
	  while(theCoating[x][y].size()>2 &&
		theCoating[x][y].front().sZ() < hN &&
		theCoating[x][y].front().sZ() < hS &&
		theCoating[x][y].front().sZ() < hE &&
		theCoating[x][y].front().sZ() < hW )
	    {
	      theCoating[x][y].pop_front();
// 	      std::cout << "Pop at " << x<< " " << y << std::endl;
	      
	    }
	  
	}
};

void depositGround(int splatNumber, 
		  Field<real> & Ground, Coating & theCoating)
{
  for(int x = 0; x < Ground.getX(); x++)
    for (int y = 0; y < Ground.getY(); y++)
      {
	// MB: Changed /2 to /2. to make sure that doubles are used
	theCoating[x][y].push_back(splatNumber, Ground[x][y]);
	
      }
}


int main (int argc, char *argv[])
{    
  //check parameter list
  if (argc != 2) 
    {
      cout<<"Ungueltige Parameterliste!"<<endl;
      cout<<"Programm bitte ausschliesslich mit dem Dateinamen der Input-Datei starten!"<<endl;
      exit(1);
    }
    
  string filename = argv[1];
    
  if(!renamer::FileExists(filename))
    {
      cout<<"Datei "<<filename<<" nicht gefunden!"<<endl;
      exit(1);
    }
    
    
  Splat** Splats; //splat pointer
    
  Problem theProblem(filename); //read problem file
  // Remove old images only if no new ones are created
  if (theProblem.epsOutput !=0)
    {
      system("rm -f all*.eps"); //delete old *.eps files
      system("rm -f all*.gif"); //delete old *.gif files
    }
    
  WRITE(theProblem.impactAngle);
  WRITE(theProblem.distributionFilename);
    
  RNG Rand(theProblem.seed);    //random numbers
  Lattice mainLattice(theProblem.dimXMain, theProblem.dimYMain, theProblem.dx, theProblem.dy);
  Lattice localLattice (theProblem.numLxPoints, theProblem.numLyPoints, theProblem.dx, theProblem.dy);
  Field<real> Ground(mainLattice);
  Field<real> currentHeight;
  Coating theCoating(mainLattice);
    
    
    
  Field3D<int> *Volume;
  if (theProblem.write3D)
    Volume = new Field3D<int>(theProblem.dimXMain,theProblem.dimYMain,theProblem.dimZMain,0, theProblem.numParticles+1); 
  //3D filed to create matlab file
  //due to color coding of cuts, numParticles+1 (=splats per stage) is
  //handed over here

  std::cout<<"Starting 3D TBC simulation"<<std::endl;

  Splats = (Splat**)calloc(theProblem.numParticles, sizeof(Splat*));
  // 1D-Feld mit Zeigern ins nichts, spaeter auf Objekt "Splat"     
       
  // Profilierte Substrate am Ende der Datei

  Point center(theProblem.dimXMain/2, theProblem.dimYMain/2, mainLattice);
  // circular hole 
  /*
    for (int i = 0; i < 300; i++)       //X-Richtung
    for (int j = 0; j < 300; j++)     //Y-Richtung
      {
	Point t(299-i,299-j, mainLattice);
	if ( (i*i+j*j) > 100)
	  Ground(t) =  5.e-6; 
	  }
  */
  /*    for (int i = 0; i < 300; i++)       //X-Richtung
    for (int j = 0; j < 300; j++)     //Y-Richtung
      {
	Point t(i,j, mainLattice);
	if ( (j<100) || (j>103))
	  Ground(t) =  5.e-6; 
	  }*/
  if (theProblem.write3D)
    Volume->Deposit(center,Ground,theProblem.dz); //Anzeige von Hindernissen (Matlab)
  // center = Punkt zur Uebergabe von Hindernissen von Ground(2DFeld) nach Volume(3DFeld)
  // center ist eine Verschiebung, Funktion Deposit in Field3D.C 
  // z.B. Eckpunkt Stufe(0,0) + center (p.x,p.y) - 1/2 Gitterlaenge
  // wenn center = 1/2 dimXMain bzw. dimYMain, werden die Gitter von Ground und Volume identisch
	
//    depositGround(0, Ground, theCoating);

  for (int i = 0; i < theProblem.numParticles; i++) //auskommentieren, um nur Stufe plotten zu koennen
    {
      Point *point;
      point = new Point ((int)Rand.uniform(0,theProblem.dimXMain),(int)Rand.uniform(0,theProblem.dimYMain), mainLattice);
	
//          	point = new Point(70,70, mainLattice);
      // 	WRITE(Ground(*point));
	
      // 	if (i == 0) //Auftreffpunkt festgelegt für 2 bis 4 Teilchen
      // 	    point = new Point(100,85, mainLattice);
      //         else if (i==1)
      // 	point = new Point(100,120, mainLattice);
      // 	else if (i==2)    
      // 	point = new Point(140,120, mainLattice);
      // 	else if (i==3)    
      // 	point = new Point(110,60, mainLattice);

//       std::cout<<"ACHTUNG: IMPACT POINT PER HAND GESETZT !!!"<<std::endl;

      std::cout<<"   "<<std::endl;
      std::cout<<"   "<<std::endl;
      std::cout<<"SPREADING OF PARTICLE NO."<< i+1 <<std::endl;
      std::cout<<"   "<<std::endl;
      std::cout<<"Impact point coordinate:  "<<point->x<<"  "<<point->y<<std::endl;

      currentHeight = Ground.Cut(*point, localLattice);
      Splats[i] = new Splat(theProblem, currentHeight); 
      // erzeugt fuer jedes i ein Objekt, i=0 bis numParticles
      // Konstruktor Splat::Splat, Reset() -> Ausgangssituation

      currentHeight = Splats[i]->Melt();
      // Ausbreitung: marker->SetVolume und marker->CalcNewPosition

      Field<real> globalCurrentHeight(mainLattice);
      globalCurrentHeight.MakeGlobal(*point,currentHeight);
      if (theProblem.write3D)
	Volume->DepositNew(globalCurrentHeight, Ground, theProblem.dz); //und nochmal ablagern in 3D
      // Replace this by depositSplat because deposition has to be
      // done on Ground and on theCoating
      depositSplat(*point, i+1, currentHeight,  Ground,   theCoating);
	
      // Old version:
      //        Ground.Deposit(*point, currentHeight); //currentHeight auf Ground ablagern

      std::cout<<std::endl<<std::endl;
      std::cout<<"END of spreading of particle No. "<< i+1 <<std::endl;
      std::cout<<std::endl<<std::endl;
      cout << "Roughness-Particle " << i+1 << OUT(roughnessRa(Ground)) << " "
	   << OUT(roughnessMinMax(Ground))<< " " << OUT(mean(Ground)) << endl;
      if (( theProblem.writePovray) &&
	  (   (((i<200) || (i>1900)) && (i%10==0)  ) || (i%100==0)))
	{
	  std::string povname="coating"+makeString(i,4)+".pov";
	  theCoating.writePovrayFile(povname.c_str(), theProblem);
	  systemCommand("povray %s -W2048 -H1536 -D", povname.c_str());
	}
      // not clean but saves memory...
      delete (Splats[i]);
    }
  ofstream fcoat ("coating.dat", ios::out|ios::binary);
  fcoat << theCoating;
  fcoat.close();

  if ( theProblem.writePovray)
    {
      theCoating.writePovrayFile("coating.pov", theProblem);
      systemCommand("povray coating.pov -W2048 -H1536 -D");
    }
  
  //----------------------------------------------------------------------
  // output
  // calculate scale factor in y and z direction 
  double scale=theProblem.dx/theProblem.dz;
  int xScale = 1;
  int yScale = 1;
  int zScale = 1;
    
  while ( fabs(scale*zScale- int(scale*zScale)) > 1.e-10)
    zScale++;
    
  yScale = int(scale*zScale);
  xScale = int(scale*zScale);
    
  //Ausgabe Faktoren
  /*
    cout<<OUT(scale)<<endl; 
    cout << "scaling factor in x-direction: " << xScale
    << " y-direction: " << yScale <<
    "z-direction: " << zScale << endl;
    cout << "length and width of a pixel: " << theProblem.dz/zScale
    << endl;    
    if (xScale > 100 || yScale > 100 || zScale > 100)
    {
    cout << "Abbruch- Skalierungsfaktor außerhalb des zulässigen Bereichs" << endl;
    exit(1);
    }   
    
    /*int slices = 15; //Anzahl der auszugebenden Schliffe
    int distanceSlice = theProblem.dimYMain/slices;
    pgm neu;    
    for (int i=0; i<theProblem.dimYMain; i+=distanceSlice)
    {	    
    neu = Volume->YZLayer(i);
	
    //Skalierung in X-Richtung 
    neu.scaleNearNeigh(yScale,1.);
    //Skalierung in Y-Richtung
    neu.scaleNearNeigh(1.,zScale);
	
    string sname="SchnittYZ"+itoa(i)+".pgm";
    cout << OUT(i) << OUT(sname) << endl;
    neu.saveAs(sname);
    ppm::pgm2ppm(sname,HEAT);
    }
  */
    
  //----------------------------------------------------------------------

  //cout trough volume
  if (theProblem.write3D)
    {
      pgm neu2;
      neu2 = Volume->XYLayer(1);
      neu2.scaleNearNeigh(xScale,1.);
      neu2.scaleNearNeigh(1.,yScale);

      neu2.saveAs("SchnittXY.pgm");
      ppm::pgm2ppm("SchnittXY.pgm",HEAT);

    
      neu2 = Volume->XZLayer(60);
      neu2.scaleNearNeigh(xScale,1.);
      neu2.scaleNearNeigh(1.,zScale);
      neu2.saveAs("SchnittXZ.pgm");
      ppm::pgm2ppm("SchnittXZ.pgm",HEAT);

      //Matlab-Datei erstellen
      if (theProblem.writeTecplot != 0)
	Volume->writeTecplotfile();
      // Write Povray-picture
      Volume->writePovrayFile("polygon.pov", theProblem);
      // Do a single tecplot output directly to a file
      if (theProblem.writeTecplot == 2)
	{
	  systemCommand("sed -e 's/--imaxvalue--/%i/g' -e 's/--jmaxvalue--/%i/g' -e 's/--kmaxvalue--/%i/g' -e 's/--blankcutoff--/%i/g' testsplash.mcr > tecplotmacro.mcr", 
			theProblem.dimZMain, theProblem.dimXMain, theProblem.dimYMain,
			theProblem.numParticles+2);
	  systemCommand("tec360 -b -p tecplotmacro.mcr");
	
	}
    
      if (theProblem.writeMatlab != 0)
	Volume->writeMatlabfile();
    }
    
  // 3D output with gnuplot
  if (theProblem.writeGnuplot != 0)
    Ground.writetofile(); 

  std::cout<<std::endl;
  std::cout<<"Program finished"<<std::endl;
}

// Profilierte Substrate
    //KANAL
/*    for (int i = 0; i < 85; i++)
        for (int j = 0; j < 75; j++)
        {
            Point t(i,j, mainLattice);
            Ground(t) = 5.0e-6; 
        }
    for (int i = 115; i < 200; i++)
        for (int j = 0; j < 75; j++)
        {
            Point t(i,j, mainLattice);
            Ground(t) = 5.0e-6; 
        }
*/

    //STUFEN 

    // FALL A: 
/*    for (int i = 0; i < 200; i++)       //X-Richtung 600
        for (int j = 0; j < 75; j++)     //Y-Richtung 150
        {
            Point t(i,j, mainLattice);
            Ground(t) = 2.0e-6; 
        }
*/

    // FALL B: 
/*    for (int i = 0; i < 200; i++)       //X-Richtung
        for (int j = 0; j < 105; j++)     //Y-Richtung
        {
            Point t(i,j, mainLattice);
            Ground(t) = 0.9e-6; 
        }
*/

/*// Double-inclined slope
    for (int i = 0; i < 200; i++)       //X-Richtung
        for (int j = 0; j < 200; j++)     //Y-Richtung
        {
            Point t(i,j, mainLattice);
            Ground(t) = abs(50-(j%100)) * 4.e-7; 
        }
*/

// simple slope
/*    for (int i = 0; i < 200; i++)       //X-Richtung
        for (int j = 0; j < 200; j++)     //Y-Richtung
        {
            Point t(i,j, mainLattice);
            Ground(t) = (200-j) * 6.e-7; 
        }
    WRITE(roughnessRa(Ground));
    WRITE(roughnessMinMax(Ground));
    exit(1);
*/    
      
// 3D Gaussian
/*    for (int i = 0; i < 200; i++)       //X-Richtung
        for (int j = 0; j < 200; j++)     //Y-Richtung
        {
            Point t(i,j, mainLattice);
            Ground(t) = 3.e-5 *
	      exp(-   double( ((i-100)*(i-100)) + ((j-100)*(j-100)))/2000); 
        }

*/
/*
    for (int i = 0; i < 200; i++)       //X-Richtung 
        for (int j = 0; j < 200; j++)     //Y-Richtung 150
        {
            Point t(i,j, mainLattice);
            Ground(t) = 10.*theProblem.dz; 
        }
*/
//     Ground.readfromfile();
    
