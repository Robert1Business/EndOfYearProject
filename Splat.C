#include "Splat.H"

real Splat::maxAbsField(Field<real> &field)
{
  LatticeIterator LatIter(theLattice); //LatticeIterator erzeugen
  real value = 0;
    
  for(LatIter.begin(); !LatIter.end(); LatIter++)  
    {
      if (value < abs(field(*LatIter)) && abs(field(*LatIter))!= abs(UNREAL_VELOCITY))
	value = abs(field(*LatIter));
    }
  return value;
}

void Splat::Reset()
{
  //Partikel als geschmolzen setzen
  static RNG Rand(theProblem->seed);
  static bool firstParticle=true;
  
  if (Rand.uniform()<theProblem->moltenFraction)
    molten = false;
  else
    molten = true;

  LatticeIterator LatIter(theLattice); //LatticeIterator erzeugen
  impactPoint = new Point ((int)round(theLattice.Lx()/2.),(int)round(theLattice.Ly()/2.),theLattice); //Auftreffpunkt
  
  Point latP = * LatIter; //Nullpunkt
  localDirectionAngle = 0;
  localImpactAngle = 0;

  // 21/08/08 Take the actual substrate shape into account to form
  // elliptical splats if the splat impacts on a substrate which is
  // inclined.
  // For this, we need to write the surroundings of the impact point
  // to a file and then fit a plane through this
  // As fourth argument, we write the distance plus a constant to weight the
  // data points (i.e., those near the center are weighted more strongly)
  systemCommand("rm -f 3dplot.params");
  string filename = "3dplot.data";
  ofstream f (filename.c_str(), ios::out|ios::binary);
  int averagingDistance = int(3* (theProblem->markerRadius));
  for (int x=-averagingDistance; x<=averagingDistance;x++)
    {
      for (int y=-averagingDistance; y<=averagingDistance;y++)
	{
	  // Ugly safety catch: If the data are fitted exactly by a
	  // plane (inclined, as may happen with initially inclined
	  // substrate), then gnuplot's fit may not converge. To avoid
	  // this, we multiply one of the data points with a tiny
	  // error
	  double tinyFact=1.;
	  if ((x==-averagingDistance) && (y==-averagingDistance))
	     tinyFact=1.00001;
	  f << x * theProblem->dx
	    << " "<< y* theProblem->dy << " " <<" " << 
	    height(*impactPoint + Point(x,y, theLattice)) * tinyFact<< " "
	    << (sqrt(x*x+y*y)+averagingDistance/3.)* theProblem->dx << std::endl;
	}      
      f << std::endl;  
  }
  f.close();
  systemCommand("gnuplot 3dplot.gpl > gnuplot.log 2>&1 ");
  filename = "3dplot.params";
  ifstream fi (filename.c_str(), ios::in|ios::binary);
  double a,b,z0;
  fi >> a >> b >> z0;
  systemCommand("more 3dplot.params");
  a *=-1;
  b *=-1;
  //  cout << OUT(a) << OUT(b) << OUT(z0) << endl;
  // round the impact angle

  localImpactAngle = int(radToDeg(M_PI_2 - atan(1./sqrt(a*a+b*b)))+0.5);
  if (localImpactAngle > 0.5)
    {
      localDirectionAngle = radToDeg(atan(b/a));
      if (a<0) localDirectionAngle+=180;
      if ((a>0) && (b<0))  localDirectionAngle+=360;
    }

  WRITE(localDirectionAngle);
  WRITE(localImpactAngle);
  WRITE(theProblem->impactAngle);
  static  Distribution test;
  static bool distributionRead=false;
  if ((theProblem->radiusDistribution==1) && !distributionRead)
    {
      distributionRead=true;
      test.read(theProblem->distributionFilename);
    }
  real radius;
  if (theProblem->radiusDistribution==1)
    {
      // draw a random number and look for the corresponding radius
      do
	{
	  radius=test.interpolateValue(Rand.uniform(0.,1.));
	}
      while (radius<theProblem->markerRadius * theProblem->dx);
      
    }
    else
      radius=theProblem->partMeanRadius;
  std::cout << "particleRadius " << radius << std::endl;
  
  //Teilchen mit festem Radius
  droplet = new Particle(radius, theProblem->partMeanVel, theProblem->partViscosity,
			 theProblem->partDensity,
			 theProblem->impactAngle + localImpactAngle,
			 theProblem->directionAngle + localDirectionAngle, 
			 molten, *impactPoint,
			 theProblem->slowDownFactor, theProblem->slowDownNorm);
  
  
  // kleiner Startkreis mit Radius = markerRadius
  if (molten)
    {
      for(LatIter.begin(); !LatIter.end(); LatIter++)  
	{
	  double DX = theLattice.dx()*(impactPoint->x - (*LatIter).x);
	  double DY = theLattice.dy()*(impactPoint->y - (*LatIter).y);
	  double distance = sqrt(pow(DX,2) + pow(DY,2));
      
	  if(distance < theProblem->markerRadius * theProblem->dx)
	    {
	      depositHeight(*LatIter) = droplet->heightMadejski();
	    }
	  else
	    {
	      depositHeight(*LatIter) = 0; 
	    }
	}
  
      //Zuweisung Fluidstate
      FieldProperty::loopENtoFS(theLattice, LatIter, depositHeight, Fluid);
  
      //Zuweisung Neighbourstate an alle FS
      FieldProperty::loopNSWO(theLattice, LatIter, Fluid, Neighbours);
  
      //Marker-Partikel setzen
      marker = new Marker(theLattice,Fluid, theProblem->markerRadius,
			  *impactPoint, 
			  theProblem->impactAngle + localImpactAngle,
			  theProblem->directionAngle + localDirectionAngle, 
			  theProblem, droplet);
  
      std::cout<<std::endl;
      if (firstParticle)
	{
	  std::cout<<"Startkonfiguration:"<<std::endl;
	  OutputParameters();
	  firstParticle=false;
	}
    }
  else
    {
      double unmoltRad=pow(2., 0.333) * droplet->theRadius();
      
      for(LatIter.begin(); !LatIter.end(); LatIter++)  
	{
	  double DX = theLattice.dx()*(impactPoint->x - (*LatIter).x);
	  double DY = theLattice.dy()*(impactPoint->y - (*LatIter).y);
	  double distance = sqrt(pow(DX,2) + pow(DY,2));
	  
	  if(distance < unmoltRad)
	    {
	      // Use Pythagorean theorem for half-sphere height 
	      depositHeight(*LatIter) = sqrt(unmoltRad*unmoltRad - distance*distance );
	      
	    }
	  else
	    {
	      depositHeight(*LatIter) = 0; 
	    }
	}
      marker = NULL;
    }
  
}

void Splat::OutputParameters()
{
//   WRITE(theLattice.Lx());
//   WRITE(theLattice.Ly());
//   WRITE(theLattice.dx());
//   WRITE(theLattice.dy());
  WRITE(theProblem->numParticles);
  WRITE(theProblem->partMeanRadius);
  WRITE(theProblem->partMeanVel);
  WRITE(theProblem->partViscosity);
  WRITE(theProblem->partDensity);
  WRITE(droplet->impactAngle);
  WRITE(droplet->directionAngle);
  WRITE(droplet->Reynolds());
  WRITE(droplet->volume());
  WRITE(droplet->heightMadejski());
}

Splat::Splat(Problem &prob, Field<real> &currentHeight)
{
  theProblem = &prob;
  //Creating a lattice with theProblem (pushDefault or Testin)
  theLattice = Lattice(theProblem->numLxPoints,
		       theProblem->numLyPoints, theProblem->dx, theProblem->dy);
    
  //Felder definieren
  height = currentHeight;        //aktuelle Substrathöhe (z.B. Hsub + H von zig Splats)  
  depositHeight = Field<real> (theLattice); //aktuelle Splathöhe (Dicke des abzulegenden Splats)
  Fluid = Field<FluidState> (theLattice);  //Feld fuer Fluidstate (E, EN, FS, F)
  Neighbours = Field<NeighbourState> (theLattice); //Feld fuer Neighbourstate (N, S, O, W, NW, NO ...)
  //F, FS etc. nur noch fuer Darstellung in all*.eps interessant
    
  Reset();
}

Splat::~Splat()
{
  //Speicher freigeben
  if (impactPoint != NULL) delete (impactPoint);
  if (droplet != NULL) delete (droplet);     
  if (marker != NULL) delete (marker);  
}

double Splat::depositedVolume(void)
{   
  //Aufsummieren des abgelagerten Volumens 
  real SumDepositVol = 0;    
  LatticeIterator LatIter(theLattice);     

  for(LatIter.begin(); !LatIter.end(); LatIter++)
    SumDepositVol += (theLattice.dx()*theLattice.dy()*depositHeight(*LatIter));

  return SumDepositVol;
}

double Splat::CutFluid(Field<real> &heightToDeposit)
{
  double redundantVolume = 0; //ueberschuessiges Volumen, noch zu verteilen
    
  //jedes Feldelement durchgehen
  LatticeIterator LatIter(theLattice); //LatticeIterator erzeugen    

  for(LatIter.begin(); !LatIter.end(); LatIter++)
    {
      //wenn die Feldhoehe die errechnete Madejski-Hoehe ueberschreitet
      if(depositHeight((*LatIter)) > heightToDeposit(*LatIter))
        {
	  //ueberschuessiges Volumen berechnen
	  redundantVolume += (theLattice.dx()*theLattice.dy()) *
			      (depositHeight(*LatIter) - heightToDeposit(*LatIter));
	  depositHeight(*LatIter) = heightToDeposit(*LatIter); // auf Madejski-Hoehe zurueckschneiden
        }
    }
    
  return redundantVolume;
}

Field<real> Splat::Melt(void)
{
  //height.writetofile(); //Ausgabe des Untergrundes (Stufe) fuer gnuplot

  LatticeIterator LatIter(theLattice);

  std::cout<<std::endl;
  std::cout<<"Starting the spreading of a molten particle."<<std::endl;
  // This field contains the change in the local height of the
  // splat: The current substrate height is averaged out and the
  // Madejski-height is then added to this local height, not the
  // current substrate Height (which is in height)
  // Second parameter is the smoothing range
  Field<real> smoothedHeight=averageField(height,5); 
  Field<real> madField(theLattice, droplet->heightMadejski());
  Field<real> heightToDeposit(theLattice);
  heightToDeposit=(madField + smoothedHeight) - height;
  static int counter=0;
  counter++;
  LatticeIterator p(theLattice); //LatticeIterator erzeugen
  // Avoid too much smoothing, negative values etc.
  for(p.begin(); !p.end(); p++)  
    {
      heightToDeposit(*p)= MAX(heightToDeposit(*p) , 0.5*droplet->heightMadejski());
      heightToDeposit(*p)= MIN(heightToDeposit(*p) , 5.*droplet->heightMadejski());
      // Safety catch: Don't make it higher than the splashing value
      heightToDeposit(*p)= MIN(heightToDeposit(*p) ,
			       0.7*droplet->theRadius()* theProblem->splashValue);
    }
  
  
  if(molten)
    {
      real splatVolStart = M_PI * pow(theProblem->markerRadius* theProblem->dx,2)
	* droplet->heightMadejski();
      WRITE(splatVolStart);  //Volumen des Startkreises
    
      real VolumeToDeposit = droplet->volume() - splatVolStart;
      WRITE(VolumeToDeposit); //noch zu verteilendes Volumen
      std::cout<<std::endl;
        
      circulationCount = 0;
        	
      Output("all"); //Ausgabe der Startkonfiguration
      //Variable ueberschuessiges Volumen = noch zu verteilendes Volumen
      double redundantVolume = VolumeToDeposit; 

//       do //Ausbreiten, solange VolumeToDeposit und redundantVolume beide positiv
        {
	  //Markervolumen setzen
	  // NEW! (28/02/08): The marker class
	  // takes care of subtracting the volume of the start circle
	  marker->SetVolume(redundantVolume, droplet->heightMadejski());
            
	  do //Ausbreitungsalgorithmus
	    {
	      circulationCount ++;
	      //Marker bewegen & Material ablagern                
	      marker->CalcNewPosition(theLattice, Fluid, depositHeight, height, heightToDeposit); 
	      //Markerbewegung verlangsamen (findet keine Anwendung, Faktor = 1, Norm = 0)
	      marker->SlowDownMarker(droplet->slowDownFactor, droplet->slowDownNorm); 
	      marker->distributeVolume(theLattice);
	      // Debug-output
	      if (circulationCount > 200)
		{
		  marker->write();
		}
	      
	      Output("all"); //Ausgabe aller Zwischenschritte
            } while (!marker->VolumeZero());
	  
	  //Höhenfeld beschneiden u. überschüssiges Volumen berechnen
// 	  redundantVolume = CutFluid(heightToDeposit); 
	  VolumeToDeposit = droplet->volume() - depositedVolume();

	  //cout << "abgeschnittenes Volumen (redundantVolume): "<< redundantVolume << endl;
	  //cout << "bisher abgelagertes Volumen (depositedVolume): "<< depositedVolume() << endl;
	  //cout << "noch abzulagerndes Volumen (VolumeToDeposit): "<< VolumeToDeposit  << endl;
	  //cout << "redundantVolume + abgelagertes Volumen: " << redundantVolume + depositedVolume() << endl;
                
        }
      // Redundant volume not needed for new algorithm
      //      while (VolumeToDeposit >= 0 && redundantVolume > 0);
        
    
      WRITE(depositedVolume());
      WRITE(marker->GetDeadVolume());
      WRITE(depositedVolume()+marker->GetDeadVolume());
      WRITE(droplet->volume());
      if (marker->splashed)
	cout << "Splashing occured in particle " << counter << endl;
      Output("all"); //Ausgabe
	
      //Datei schreiben mit abgelagertem Volumen in Abhaengigkeit des Winkels
//        WriteVolumeData(0.1*M_PI/180, "VolumeData.dat");
	
    } //END if(droplet->molten)
  else
    {
      std::cout<<"Particle " << counter << " is not molten!"<<std::endl;
    }
  //Schleife bzw. Funktion zur Glättung des abgelagerten Splats
  // Instead of this, the new function averageField could be used,
  // but the Smoothing routine is slightly better cause it takes
  // care of trouble with downward steps

    Smoothing();
    

  return depositHeight;
}

void Splat::Smoothing( )
{
  for (int repeat=0; repeat<1; repeat++)
    {
      
      Field<real> smoothHeight(theLattice);
      LatticeIterator LatIter(theLattice);

      for(LatIter.begin(); !LatIter.end(); LatIter++)
	{
	  double heightSum = 0;
	  double heightSum0to3 = 0;
	  double heightSum4to11 = 0;

	  /*
	  //  VERSION MIT ACHT NACHBARN

	  NeighbourIterator NeighIter(*LatIter, EIGHT);

	  for(NeighIter.begin(); !NeighIter.end(); NeighIter++)
	  {
	  heightSum += depositHeight(*NeighIter) + height(*NeighIter);
	  }

	  // Mittelung ueber 8 Nachbarn und den aktuellen Wert (*LatIter)	
	  smoothHeight(*LatIter) = (heightSum/8 + (depositHeight(*LatIter) + height(*LatIter)) )/2 - height(*LatIter);	
	  */

	  //  VERSION MIT ZWOELF NACHBARN

	  // wenn wir nicht auf einem aktuellen Punkt des Splats sind:
	  //if (depositHeight(*LatIter)==0.) //MIST
	  if ((depositHeight(*LatIter)==0.) && (Fluid(*LatIter).state != EN)) 
	    {   //keine Höhe und keine Zelle mit vollem Nachbarn -> also E-Zelle

	      smoothHeight(*LatIter) = 0.;
	      continue;
	    }

	  //wenn wir auf einem aktuellen Punkt des Splats sind:
	  //Definition Nachbar-Iterator fuer den aktuellen Punkt (*LatIter)
	  NeighbourIterator NeighIter(*LatIter, TWELVE);

	  for(NeighIter.begin(); !NeighIter.end(); NeighIter++)
	    {
	      if(NeighIter.direction() == 0 || NeighIter.direction() == 1 || NeighIter.direction() == 2 || NeighIter.direction() == 3)
		{
		  heightSum0to3 += 2*(depositHeight(*NeighIter) + height(*NeighIter));
		}
	      else
		{
		  heightSum4to11 += depositHeight(*NeighIter) + height(*NeighIter);
		}

	    }

	  //WRITE(heightSum0to3);
	  //WRITE(heightSum4to11);

	  //Mittelung ueber 12 Nachbarn und den aktuellen Wert (*LatIter)
	  //Wertung je nach Faktor, deshalb die äußeren 16wertig

	
	  smoothHeight(*LatIter) = ((heightSum0to3 + heightSum4to11)/16 +
				    (depositHeight(*LatIter) + height(*LatIter)))/2 - height(*LatIter);
	  // 	double smoothFactor=0.8;
	  // 	smoothHeight(*LatIter) = (  smoothFactor*( (heightSum0to3 + heightSum4to11)/16 + (depositHeight(*LatIter))
	  // 				  + (1-smoothFactor) * height(*LatIter))) - height(*LatIter);
	}


      //geglaettete Hoehen ins alte Feld uebertragen
      for(LatIter.begin(); !LatIter.end(); LatIter++)
	{
	  bool highStep = false;
	  if (Fluid(*LatIter).state == EN)
	    {
	      NeighbourIterator NeighIter(*LatIter);
	      for(NeighIter.begin(); !NeighIter.end(); NeighIter++)
		{
		  if (height(*NeighIter) - height(*LatIter) >
		      3.*theProblem->smallDelta* droplet->theRadius())
		    highStep = true;
		} //an hohen Stufen abwärts soll die EN-Zelle (erst Zelle unterhalb der Stufe) keinen Wert bekommen
	    }
	  if (!highStep)
	    depositHeight(*LatIter) = smoothHeight(*LatIter);
	
	}
    }
  
  return;
}


void Splat::Output(string filename)
{
  if (theProblem->epsOutput == 1)
    {
      LatticeIterator LatIter(theLattice); 

      //Schreiben der FS-Zellen
      string f1 = filename + itoa(1) +  ".dat";
      ofstream f_1 (f1.c_str(), ios::out|ios::binary);

      for(LatIter.begin(); !LatIter.end(); LatIter++)  
        {
	  if (Fluid(*LatIter).state == FS)
	    f_1<<LatIter.getX()+0.5<<" "<<-(LatIter.getY()+0.5)<<" "<<Fluid(*LatIter)<<std::endl;
        } 
      f_1<<std::endl<<std::endl;

      //Schreiben der F-Zellen
      for(LatIter.begin(); !LatIter.end(); LatIter++)  //Field depositVol aufsummieren
        {
	  if (Fluid(*LatIter).state == F)
	    f_1<<LatIter.getX()+0.5<<" "<<-(LatIter.getY()+0.5)<<Fluid(*LatIter)<<std::endl;
        }
      f_1.close();
        
      marker->Output();
        
      //Gnuplot ausfuehren
      system("rm -f gnuplot_all.gpl"); //alte Datei gnuplot_all.gpl wird gelöscht
      ofstream gnu("gnuplot_all.gpl"); //neue Datei gnuplot_all.gpl wird erzeugt

      //schreiben der Datei gnuplot_all.gpl; 
      //x- und yrange in Abh. der lokalen Gitterparameter setzen !!
      gnu << "set term post portrait color 'Times-Roman' 14" << std::endl; 
      gnu << "set output 'all.eps'" << std::endl; 
      gnu << "set size square" << std::endl; 
      gnu << "set xrange [0:" << theProblem->numLxPoints <<  "]" << std::endl; 
      gnu << "set yrange [" << -theProblem->numLxPoints  << ":0]" << std::endl; 
      gnu << "set style line 1 lt 7 lw 1" << std::endl; 
      gnu << "set style line 2 lt 0 lw 1" << std::endl; 
      gnu << "set xtics 20 " << std::endl; 
      gnu << "set ytics 20" << std::endl; 
      gnu << "set mxtics 20" << std::endl; 
      gnu << "set mytics 20" << std::endl; 
      gnu << " " << std::endl; 
      gnu << "set grid  xtics ytics mxtics mytics ls 1, ls 2" << std::endl; 
      gnu << "plot 'all1.dat' index 0 using 1:2 notitle with points lt 5 pointtype 5 pointsize 0.4,"; 
      gnu << "     'all1.dat' index 1 using 1:2 notitle with points lt 3 pointtype 5 pointsize 0.4,"; 
      gnu << "     'velocity.dat' using 1:2:3:4 notitle with vectors lt 1 lw 0.2,"; 
      gnu << "     'marker.dat' using 1:(-$2) notitle with points lt 15 pointtype 7 pointsize 0.4" << std::endl; 

      system("gnuplot gnuplot_all.gpl");
      string extraString="";
      if (circulationCount<10) extraString="00";
      else if (circulationCount<100) extraString="0";
      string file_new = "all" + extraString + itoa(circulationCount)+".eps";
      string file_newgif = "all" + extraString + itoa(circulationCount)+".gif";
      system("convert all.eps all.gif");  //Konvertierung von eps zu gif fuer ~/gifsicle (Animation erstellen)
      renamer::RenameFile("all.eps", file_new);  
      renamer::RenameFile("all.gif", file_newgif);  
    }
}


void Splat::WriteVolumeData(double deltaAngle, string fileName)
{
  //Laufvariable fuer Winkel
  double angle = 0;
    
  //Laufvariable Mittelpunkt bis Ende Splat (dort, wo keine Hoehe mehr abgelagert wurde)
  int l = 0;

  int center_x = (int) floor(theLattice.Lx()/2.);
  int center_y = (int) floor(theLattice.Ly()/2.);

  //Koordinaten des aktuell betrachteten Elements
  int cell_x;
  int cell_y;

  //Koordinaten des vorherigen Elements
  int cell_x_old;
  int cell_y_old;

  //Wert des Volumens ueber Winkel
  double volumeValue;

  //Objekt zur Dateiausgabe
  ofstream file (fileName.c_str(), ios::out);
    
  while (angle < 2*M_PI) //solang bis 360 Grad durchlaufen
    {
      cell_x_old = center_x;
      cell_y_old = center_y;

      cell_x = center_x;
      cell_y = center_y;
      if ( (cell_x <0) && (cell_y <0) &&
	   (cell_x >=theLattice.Lx()) && (cell_y >=theLattice.Ly()) )
	continue;
      
      volumeValue = 0;

      // !! THIS IS NOT TOTALLY FAILSAFE!!! cell_x or cell_y might run
      // out of bounds
      while (depositHeight[cell_x][cell_y] > 0)
	{
	  cell_x = center_x + (int) floor(l* cos(angle)); 
	  cell_y = center_y + (int) floor(l* sin(angle));
	     

	  if (( (cell_x != cell_x_old || cell_y != cell_y_old)) &&
	       (cell_x >=0) && (cell_y >=0) &&
	       (cell_x <theLattice.Lx()) && (cell_y <theLattice.Ly()) )
	    volumeValue += depositHeight[cell_x][cell_y] * theLattice.dx() * theLattice.dy();
	     
	  cell_x_old = cell_x;
	  cell_y_old = cell_y;
	  l++;
	}
      file << angle*180/M_PI << " " << volumeValue << endl; 
	
	
      l = 0; //Laufvariable der Laenge auf Ursprungswert zuruecksetzen
      angle += deltaAngle; //Laufvariable des Winkels um einen Winkelschritt erhoehen
    } 
    
  file.close();
}
 
void Splat::writeSplat2File(int splatNumber, Point &impactPoint)
{
    //führende Nullen realisieren    
    std::string numberZeros; //Nullen, die eingefügt werden müssen (z.B. Splat0001.dat)
    int numSplats = theProblem->numParticles; //Gesamtzahl der Splats    
    int decimal = 1;    
    while (numSplats/(splatNumber+1)/decimal > 0)
    {           
        numberZeros += "0";
        decimal *=10;    
    }

    //Dateinamen des aktuellen Splats zusammenbauen (fortlaufende Nummerierung mit führenden Nullen auffüllen)
    std::string splatFileName = "Splat" + numberZeros + itoa(splatNumber,10)+".dat";
    cout << "Write splat " << splatNumber << " to file " << splatFileName << endl;
    
    //Objekt zur Dateiausgabe
    ofstream splatFile (splatFileName.c_str(), ios::out);
    
    //x_min, y_min, y_max, y_max, h_min und h_max des Splats finden
    LatticeIterator LatIter(theLattice);   
    int x_min = theLattice.Lx();
    int y_min = theLattice.Ly();
    int x_max = 0;
    int y_max = 0;
    double h_min = NIRWANA;
    for(LatIter.begin(); !LatIter.end(); LatIter++)
    {  
        if (depositHeight(*LatIter) > 1e-28)
        {        
            if (LatIter.getX() < x_min) x_min = LatIter.getX();
            if (LatIter.getY() < y_min) y_min = LatIter.getY();
            if (LatIter.getX() > x_max) x_max = LatIter.getX();
            if (LatIter.getY() > y_max) y_max = LatIter.getY();
            
            if (height(*LatIter) < h_min) h_min = height(*LatIter);     
        }
    }

    //Ausgabe der Daten
    splatFile << "#" << theProblem-> filename<< std::endl;
    splatFile << "# global impact point" << std::endl;
    splatFile << impactPoint.x << "  " << impactPoint.y << std::endl << "#" <<std::endl;
    splatFile << "# global field dimensions" << std::endl;
    splatFile << theProblem->dimXMain << "  " << theProblem->dimYMain << std::endl << "#" <<std::endl;
    splatFile << "# local field dimensions" << std::endl;
    splatFile << theProblem->numLxPoints << "  " << theProblem->numLyPoints << std::endl << "#" <<std::endl;
    splatFile << "# dx,dy of fields" << std::endl;
    splatFile << theProblem->dx << "  " << theProblem->dy << std::endl << "#" <<std::endl;
    splatFile << "#x  y  Ground  currentHeight" << std::endl; 
    for(LatIter.begin(); !LatIter.end(); LatIter++)  
        if (LatIter.getX() >= x_min && LatIter.getX() <= x_max && LatIter.getY() >= y_min && LatIter.getY() <= y_max)
        {
            int x = LatIter.getX();
            int y = LatIter.getY();
            //aus lokalen Koordinaten globale berechnen            
            //x = (x + impactPoint.x - (int)round(double(height.getX())/2.) + theProblem->dimXMain) % theProblem->dimXMain;
            //y = (y + impactPoint.y - (int)round(double(height.getY())/2.) + theProblem->dimYMain) % theProblem->dimYMain;
            //speichern in Datei            
            splatFile << x << "  " << y << "  " << height(*LatIter)-h_min << "  "
                      << depositHeight(*LatIter) << std::endl;
        }
    splatFile.close();
}




