//  Note that a Marker is the vector of all markers, individual markers 
//  are called MARKER_ELEM

#include "Marker.H"

void Marker::SetMarkerRadiant(Field<FluidState> &Fluid, double impactAngle, double directionAngle)
{
  //Variablen deklarieren
  double angle = 0; //1. Laufvariable: Winkel
  WRITE(theProblem->numberOfRays);
  
  double delta_angle = degToRad(360. / theProblem->numberOfRays);
  double r_innen = r_ - theProblem->markerWidth; //Innenradius
  // 01/09/08: WARNING: This implies that the markers actually start
  // within the initial circle.
  double l = r_innen; //2. Laufvariable: Laenge des Strahls, auf dem Marker sitzen
  double deltal = (double)theProblem->markerWidth/(double)theProblem->markerPerCell; //Abstand der Marker zueinander

  //Mittelpunt bestimmen (in Elementlaengen)
  double center_x = theLattice.Lx()/2.+0.5;
  double center_y = theLattice.Ly()/2.+0.5;

  while (angle < 2*M_PI) //solang bis 360 durchlaufen
    {
      while (l < r_) //von Innen- bis Aussenradius laufen
	{
	  New(center_x + l* cos(angle), center_y + l * sin(angle), angle, impactAngle, directionAngle); //setze Marker
	  l += deltal; //Abstand des naechsten Markers vom Mittelpunkt berechnen
	}
      l = r_innen; //Laufvariable der Laenge auf Ursprungswert zuruecksetzen
      angle += delta_angle; //Laufvariable des Winkels um einen Winkelschritt erhoehen
    }
     
  // Korrekturfaktor wird eingefuehrt, mit dem alle Anteile normiert werden (Addition muss 1 ergeben)
  // 20/08/08: This is repeated now because the fingering may change
  // volumes
 std::cout << "Writing correction \n";
  
  wholeVolRate=0;
  elemsIter = elems.begin();
  while (elemsIter!= elems.end())
    {
      wholeVolRate+=elemsIter->vol_percentage ;
      elemsIter++;
    }

  double correctionFactor = 1./wholeVolRate;
  WRITE(correctionFactor);
  elemsIter = elems.begin();
  while (elemsIter!= elems.end())
    {
      elemsIter->vol_percentage *= correctionFactor;
      elemsIter++;
    }
}

Marker::Marker(Lattice localLattice, Field<FluidState> &Fluid, double r, Point impactPoint,
	       double impactAngle, double directionAngle, Problem * probIn, Particle* dropletIn)
{
  //Uebergebene Werte als Attribute setzen

  theProblem = probIn;
  number = 0;
  wholeVolRate = 0;
  volume_every_step = 0;
  hMadejski=0;
  r_ = r;
  impactPoint_ = impactPoint;
  droplet=dropletIn;
  volume_to_deposit = 0;
  splashed=false;
  theLattice = localLattice;

  // If fingering occurs, use arbitrary starting phase to avoid all fingers pointing in
  // the same direction
  static RNG Rand(theProblem->seed);
  phase=Rand.uniform(0,M_PI);
  WRITE(phase);
  
    
  //Volumenanteil einlesen bzw. Integral loesen
  ReadVolPercentage(impactAngle, directionAngle); 

  //Marker setzen
  SetMarkerRadiant(Fluid, impactAngle, directionAngle);
}

Marker::~Marker()
{
  //Speicher freigeben
  DelAll();
}

double Marker::CalcNorm2D(double u, double v)
{
  return(sqrt(pow(u,2) + pow(v,2)));
}

void Marker::New(double x, double y, double angle, double impactAngle, double directionAngle)
{
  number++; //Anzahl der Marker erhoehen
  MARKER_ELEM new_elem; //neues Marker-Struct erstellen, das spaeter an Liste angehaengt wird

  //Koordinaten setzen
  new_elem.x = x; //double-Werte, d.h. mit Komma
  new_elem.y = y;

  //Geschwindigkeiten berechnen (Bedingung: sqrt(u_vel^2 + v_vel^2) = 1)
  new_elem.u_vel = cos(angle);
  new_elem.v_vel = sin(angle);

  //restliche Werte setzen
  new_elem.dead = false;
  new_elem.reflected = false;
  new_elem.volume = 0;
  new_elem.vol_percentage = GetVolPercentage(angle, impactAngle, directionAngle);
  // Add some fingering. this will not affect the total volume because
  // the vol_percentage is normalised later on
  if (theProblem->numberOfFingers > 0)
    {
      // Divide numberOfFingers by 2 for sin**2
      new_elem.vol_percentage *= (1 + theProblem->fingerLength *
				  pow((sin(angle* theProblem->numberOfFingers/2 + phase)),2) );
    }
  
  
  new_elem.oldStepHeight=NIRWANA;
  elems.push_back (new_elem); //Marker an Liste anfuegen
}

void Marker::DelAll(void)
{
  elems.clear();
  number = 0;
}

void Marker::DelDead(void)
{
  if (elems.size()==0) return;
  for(elemsIter=elems.begin(); elemsIter!=elems.end(); elemsIter++)
    {
      int x = (int)elemsIter->x;
      int y = (int)elemsIter->y;
      if (x < 0 || x > theLattice.Lx() || y < 0 || y > theLattice.Ly())
        {
	  bool atStart=true;
	  // Changed == to != in next line
	  if (elemsIter!=elems.begin())
	    atStart=false;
	  elems.erase(elemsIter);
	  if (elems.size()==0) break;
	  if (atStart)
	    elemsIter=elems.begin();
	  else
	    elemsIter--;
	  number--;
        }
    }
}
       
void Marker::Output()
{
  ofstream f ("marker.dat", ios::out|ios::binary); //schreiben der Markerkoordinaten
  ofstream f_2 ("velocity.dat", ios::out|ios::binary); //schreiben der Geschwindigkeiten
  f.precision(16);
  f_2.precision(16);
  
  for(elemsIter=elems.begin(); elemsIter!=elems.end(); ++elemsIter)
    {
      if (!elemsIter->dead) //nur lebende Marker ausgeben
        {
	  f<<elemsIter->x<<", "<<elemsIter->y<<endl;
	  f_2<<elemsIter->x<<" "<<-(elemsIter->y)<<" "<<elemsIter->u_vel*elemsIter->vol_percentage*1000<<" "<<-(elemsIter->v_vel)*elemsIter->vol_percentage*1000<<std::endl;
        }
    }
  f.close();
  f_2.close();
}

int Marker::GetNumber()
{
  return number;
}

// SniffAhead looks in the direction the marker goes to and looks for
// steps upwards and downwards. It returns the height of the highest
// and lowest point in the marker direction

void Marker::SniffAhead(Field<real> &Ground, double & minH, double& maxH)
{
  minH=1000.;
  maxH=-1000.;
  //fuer das gerade aktuelle elemsIter- Element
  int x_old = (int)elemsIter->x;
  int y_old = (int)elemsIter->y;
  int x_new = (int)floor(elemsIter->x + elemsIter->u_vel);
  int y_new = (int)floor(elemsIter->y + elemsIter->v_vel);
  if (x_new < 0 || x_new >= theLattice.Lx() || y_new < 0 || y_new >= theLattice.Ly())
    {
      minH=maxH=0.;
      warning("SniffAhead leaves the lattice");
      return;
    }
  //normDeltaH is DeltaH normalised to the distance travelled
  double normDeltaH;
  if (elemsIter->oldStepHeight == NIRWANA)
    normDeltaH=0;
  else
    normDeltaH=elemsIter->oldStepHeight / sqrt(elemsIter->u_vel*elemsIter->u_vel
					       + elemsIter->v_vel*elemsIter->v_vel);
  
  double xCurrent, yCurrent, distance;
  xCurrent = x_new;
  yCurrent = y_new;
  do 
    {
      xCurrent += elemsIter->u_vel;
      yCurrent += elemsIter->v_vel;
      if (int(xCurrent) < 0 || int(xCurrent) >= theLattice.Lx() ||
	  int(yCurrent) < 0 || int(yCurrent) >= theLattice.Ly())
	{
	  if (minH==1000.) minH=0.;
	  if (maxH==-1000.) maxH=0.;
	  warning("SniffAhead leaves the lattice");
	  return;
	}
      distance = sqrt((x_old-xCurrent)*(x_old-xCurrent)
		      + (y_old-yCurrent)*(y_old-yCurrent));
      //distance = Kaestchenanzahl
      int x_new = (int)floor(xCurrent);
      int y_new = (int)floor(yCurrent);
      if (int(x_new) < 0 || int(x_new) >= theLattice.Lx() ||
	  int(y_new) < 0 || int(y_new) >= theLattice.Ly())
	{
	  if (minH==1000.) minH=0.;
	  if (maxH==-1000.) maxH=0.;
	  warning("SniffAhead leaves the lattice");
	  return;
	}
      if ((x_new!=x_old) || (y_new != y_old))
	{
	  // Calculate the difference from current point to old ground,
	  // and subtract the expected difference
	  double deltaH = (Ground[x_new][y_new] - Ground[x_old][y_old])
	    - distance*normDeltaH;
	  if (deltaH < minH) minH=deltaH;
	  if (deltaH > maxH) maxH=deltaH;
	}
      
    }
  while( distance < theProblem->sniffAheadDistance);
}


void Marker::ApplyRules(Field<real> &depositHeight, Field<real> &Ground)
{
  //fuer das gerade aktuelle elemsIter- Element
//   cout << "Applyrules" << endl;
  
  int x_old = (int)elemsIter->x;
  int y_old = (int)elemsIter->y;
  //  floor avoids unspecified round-off trouble
  int x_new = (int)floor(elemsIter->x + elemsIter->u_vel);
  int y_new = (int)floor(elemsIter->y + elemsIter->v_vel);
  if (x_new < 0 || x_new >= theLattice.Lx() || y_new < 0 || y_new >= theLattice.Ly())
    {   //Sicherheitsabfrage: x_new und y_new auf Gitter ??

      elemsIter->dead = true;
      cout<<"Marker verlaesst lokales Gitter. Marker tot !!"<< x_new <<"  "<< y_new <<endl;
      return;
    }
  //x_new und y_new liegen auf dem Gitter
  double deltaH = Ground[x_new][y_new] - Ground[x_old][y_old];
  double deltaHDiff = 0;

  // If there was no previous height difference, the marker moves
  // in any case.
  MarkerState theState=Undefined;
  // Now compare not with deltaH, but with the actual change in
  // deltaH.
  // 07/01/09 ... but only if you do enter a new cell, otherwise
  // use deltaH (which is zero in this case)
  // Also if oldStepHeight is yet undefined
  if (((x_new!=x_old) || (y_new != y_old)) && (elemsIter->oldStepHeight!=NIRWANA) )
    deltaHDiff = deltaH -elemsIter->oldStepHeight;
  else
    deltaHDiff = deltaH;
      
  // Sniff ahead to look at the structure on a larger length scale (of
  // sniffAheadDistance). Important for filling small gaps and for
  // running up to large steps
  double sniffMax, sniffMin;
  SniffAhead(Ground, sniffMin, sniffMax);
  
  // 07/01/09: We first determine what happens to the marker and then
  // actually do it. This is simpler than the old way of doing
  // everything together because it avoids doubling of statements
  
  if(deltaHDiff < 0)  // ABWAERTS -> deltaH kleiner Null
    {
//       cout << OUT(sniffMin) << OUT(sniffMax) << OUT(deltaHDiff) << endl;
      // If the step is downwards, but an upwards step comes within
      // sniffAheadDistance, fill up anyway
      if(sniffMax-deltaHDiff > theProblem->medDelta * droplet->theRadius()) //4.e-6
	{
// 	  cout << "SniffAhead with" << OUT(sniffMax) << OUT(elemsIter->volume) << endl;
	  
	  theState=Flows;
	}
      else if(abs(deltaHDiff) > theProblem->splashValue * droplet->theRadius()) //4.e-6
	theState=Dies;
      else if(abs(deltaHDiff) > theProblem->smallDelta* droplet->theRadius()) 
	theState=SplashesDown;
      else //abs(deltaH) <= SMALLDELTA 1.e-6
	theState=Flows;
    }
  else if(deltaHDiff > 0)  // AUFWAERTS -> deltaH groesser Null
    {
      if(abs(deltaHDiff) > theProblem->medDelta* droplet->theRadius())
	theState=Dies;
      else //abs(deltaH) <= MEDDELTA
	theState=Flows;
    }
  else //deltaH == 0
    theState=Flows;
  double h0 , xCurrent, yCurrent, deltaH1, distance;
  double travelled_distance;
  switch(theState)
    {
    case Flows:
      elemsIter->x = elemsIter->x + elemsIter->u_vel;
      elemsIter->y = elemsIter->y + elemsIter->v_vel;
      break;
    case SplashesDown:
      //2.Fall: mittlere Stufen abwaerts 
      // -> Marker ueber Winkel hinter der Stufe neu aufprallen lassen 
      h0 = Ground[x_old][y_old];
      xCurrent = x_new;
      yCurrent = y_new;
      deltaH1 = tan(degToRad(15.)) * sqrt(pow(theLattice.dx(),2) + pow(theLattice.dy(),2)); 
      // Hoehenuntersch. pro Kaestchen in Laufrichtung,
      // deshalb in do-while mit distance multipliziert
      travelled_distance=0;
    RESTART_SUBDROPLET:
      do 
	{
	  xCurrent += elemsIter->u_vel;
	  yCurrent += elemsIter->v_vel;
	  if (int(xCurrent) < 0 || int(xCurrent) >= theLattice.Lx() ||
	      int(yCurrent) < 0 || int(yCurrent) >= theLattice.Ly())
	    {
	      elemsIter->dead = true;
	      cout<<"Marker verlaesst Volumen. Marker tot !!"<<endl;
	      break;
	    }
	  travelled_distance+=sqrt(elemsIter->u_vel*elemsIter->u_vel+
				   elemsIter->v_vel*elemsIter->v_vel);
	  
	  distance = sqrt((x_old-xCurrent)*(x_old-xCurrent)
			  + (y_old-yCurrent)*(y_old-yCurrent));
	  //distance = Kaestchenanzahl
	      
	}
      while(h0 - (deltaH1 * travelled_distance) > Ground[int(xCurrent)][int(yCurrent)]);
//       cout << "Hit ground at" << OUT(xCurrent) << OUT(yCurrent) << endl;
//       cout << OUT(h0) << OUT(h0 - (deltaH1 * distance)) <<
// 	OUT(Ground[int(xCurrent)][int(yCurrent)]) << endl;
      if (Ground[int(xCurrent)][int(yCurrent)]
	  < h0 - (deltaH1 * travelled_distance) +theProblem->medDelta * droplet->theRadius())
	{
	  
	  splashed=true;
	  elemsIter->x = xCurrent;
	  elemsIter->y = yCurrent;
	  elemsIter->oldStepHeight=NIRWANA;
	}
      else // Don't hit the substrate upwards! Marker is back-reflected
	{
	  if (elemsIter->reflected)
	    {
	      elemsIter->dead = true;
	      break;
	    }
	  
// 	  cout << "Marker reflected" << endl;
	  elemsIter->u_vel*=-1;
	  elemsIter->v_vel*=-1;
	  elemsIter->reflected=true;
	  goto RESTART_SUBDROPLET;
	  
	}
      if (int(xCurrent) < 0 || int(xCurrent) >= theLattice.Lx() ||
	  int(yCurrent) < 0 || int(yCurrent) >= theLattice.Ly())
	elemsIter->dead = true;
      
      //cout<<"Stufe abwaerts. Fall 2. Marker verschoben !!"<<endl;
      break;
    case Dies:
      elemsIter->dead = true;
      break;
    case Undefined:
      warning("Marker State undefined!");
    default:
      error("Defaulting on MarkerState switch. This cannot happen!");
    }
  // Adapt deltaH if the marker is in a new cell
  if ((x_new!=x_old) || (y_new != y_old) || (elemsIter->oldStepHeight=NIRWANA))
    {
      elemsIter->oldStepHeight=deltaH;
    }
      
  
}


void Marker::write()
{
  int count=0;
  for(elemsIter=elems.begin(); elemsIter!=elems.end(); ++elemsIter)
    {
      count++;
      //Marker nur bewegen, wenn nicht tot und noch Volumen vorhanden
      cout << "Marker " << count <<  " " << OUT(elemsIter->dead)
	   << OUT(elemsIter->volume) << " at " << elemsIter->x << ","
	   <<elemsIter->y << " v " << elemsIter->u_vel << ","
	   <<elemsIter->v_vel << endl;

    }
  
}



void Marker::CalcNewPosition(Lattice &theLattice, Field<FluidState> &Fluid, Field<real> &depositHeight,
			     Field<real> &Ground, Field<real> &heightToDeposit)
{
  //ueber alle Partikel
  int x_new;
  int y_new;
  real area= theLattice.dx() * theLattice.dy();
  for(elemsIter=elems.begin(); elemsIter!=elems.end(); ++elemsIter)
    {
      //Marker nur bewegen, wenn nicht tot und noch Volumen vorhanden
      if (!elemsIter->dead && elemsIter->volume > 0)
        {
	  //neue Positionen berechnen
	  ApplyRules(depositHeight, Ground); //Regeln werden angewendet und neue Partikeleigenschaften berechnet
	  x_new = (int)elemsIter->x;
	  y_new = (int)elemsIter->y;
            
	  if (x_new > 0 && x_new < theLattice.Lx() && y_new > 0 && y_new < theLattice.Ly())
            {
	      //neue Fluideigenschaften setzen
	      if (Fluid[x_new][y_new].state == E || Fluid[x_new][y_new].state == EN)
		Fluid[x_new][y_new].state = F;

	      //neues Hoehenfeld setzen
	      //nur ablagern, wenn nicht tot
	      // 27/02/08: Neu: Nur ablagern, wenn Zelle noch nicht voll
	      real missingHeight= heightToDeposit[x_new][y_new] - depositHeight[x_new][y_new];
	      if ((missingHeight > VOLUME_RESIDUAL) && (!elemsIter->dead)) 
                {
		  //Check if the volume suffices to fill the cell
		  real missingVolume = missingHeight*area;
		  if (elemsIter->volume >= missingVolume)
		    {
		      depositHeight[x_new][y_new] = heightToDeposit[x_new][y_new];
		      elemsIter->volume -= missingVolume;
		    }
		  else
		    {
		      depositHeight[x_new][y_new] += elemsIter->volume /area;
		      elemsIter->volume = 0;
		    }
                }
            }
        }
    }
    
  LatticeIterator LatIter (theLattice);
  FieldProperty::loopENtoFS(theLattice, LatIter, depositHeight, Fluid); 
  DelDead();
}

void Marker::SlowDownMarker(double slowDownFactor, double slowDownNorm)
{
  //ueber alle Marker
  elemsIter = elems.begin();
    
  double Norm; //Betrag vor Verringerung der Geschwindigkeit
    
  //nur ausfuehren, wenn Werte Einfluss auf Geschwindigkeit haben
  if (slowDownFactor != 1. && slowDownNorm!= 0.)
    {
      while (elemsIter!= elems.end())
        {
	  //Geschwindigkeit verringern mit Faktor
	  elemsIter->u_vel = elemsIter->u_vel * slowDownFactor;
	  elemsIter->v_vel = elemsIter->v_vel * slowDownFactor;
            
	  //wenn neue Norm > 0, dann Rechnung durchfuehren
	  Norm = CalcNorm2D(elemsIter->u_vel, elemsIter->v_vel);
	  if (Norm - slowDownNorm > 0)
            {
	      //Geschwindigkeit mit additiver Verkleinerung des Betrags verringern
	      elemsIter->u_vel = elemsIter->u_vel * (Norm - slowDownNorm)/Norm;
	      elemsIter->v_vel = elemsIter->v_vel * (Norm - slowDownNorm)/Norm;
            }
	  else
            {
	      //ansonsten beide Geschwindigkeitskomponenten = 0
	      elemsIter->u_vel = 0;
	      elemsIter->v_vel = 0;
            }
            
	  //Iterator erhoehen (naechster Marker wird aktuell)
	  elemsIter++;
        }
    }
}

double Marker::GetVolPercentage(double angle, double impactAngle, double directionAngle)
{
  double value;

  if (impactAngle == 0.)
    {
      //wenn Auftreffwinkel = 0, dann gerader Aufprall:
      //jeder Marker erhaelt gleichen Volumenanteil
      value = 1./(theProblem->markerPerCell*theProblem->numberOfRays);
      wholeVolRate += value;//jede Prozentualler Anteil wird aufaddiert
    }
  else
    {      
      // Add 360 to avoid negative values
      
      double alpha = radToDeg(angle)-directionAngle;
      if (alpha <0)
	alpha += 360;
      //Stellen werden abgeschnitten, um auf Array mit Volumenanteilen zuzugreifen
      int intAlpha = ((((int)(alpha))/ int(d_angle)) * int(d_angle))%360 ;
		
      //interpolieren
      int intAlpha2 = ((((int)(alpha))/ int(d_angle)+1) * int(d_angle)) %360 ;
		
      double a = (double)(volIntegral[intAlpha2]-volIntegral[intAlpha])/(double)d_angle;
      double b = volIntegral[intAlpha] - a*intAlpha;
      value = a*alpha + b;
	
//        std::cout<<OUT(intAlpha)<<" "<<(alpha)<<" "<<(intAlpha2)<<" "<<(volIntegral[intAlpha])<<" "<<(value)<<" "<<(volIntegral[intAlpha2])<<std::endl;
      wholeVolRate += value;//jeder prozentualler Anteil wird aufaddiert	
    }

  return value;
}

void Marker::ReadVolPercentage(double impactAngle, double directionAngle)
{	
  if (impactAngle != 0)
    {	
      //Dateiname der abgepeicherteten Integralwerte erzeugen
      string filename = "Int_" + itoa((int)impactAngle,10)+ ".int";

      //weitere Variablen
      double value; //eingelesene double-Zahl
      double wholeVolRate=0; //Volumenanteil ueber alle aufsummiert (sollte 1 sein)
      double correctionFactor; //wenn wholeVolRate != 1, dann wird dieser Faktor anmultipliziert
      if (!renamer::FileExists(filename))
	{
	  //wenn Datei nicht existiert, dann Volumenintegral fuer diese Winkel loesen
	  cout<<"Loese Integral..."<<endl;
	  // The directionAngle is always 0. now!
	  Solver mySolver(impactAngle, 0.,DELTA_ANGLE,MC_POINTS,filename);
	  mySolver.solveIntegral();
	}
		
      std::cout<<"Volumenintegral gefunden- Datei " << filename << " wird eingelesen"<<std::endl;
      ifstream intFile(filename.c_str()); //Dateiobjekt
      //Integral einlesen
      double ang, ang2;
      intFile >> ang >> ang2;
      d_angle = abs(ang-ang2);
      intFile >> value;
      volIntegral[int(ang+0.5)]=value;
	    
      while (!intFile.eof())
	{
	  intFile >> ang >> ang2;
	  intFile >> value;
	  volIntegral[int(ang+0.5)]=value;
// 	  std::cout << OUT(ang) << OUT(value) << std::endl;
	}
      intFile.close();
    }  
}

void Marker::SetVolume(double volume, double heightMadejski)
{
  hMadejski=heightMadejski;
  volume_every_step = 0.01* hMadejski * theLattice.dx() * theLattice.dy();
  double v = volume;

  // 28/02/08: We need a correction for oblique impact: The initial
  // circle should be subtracted from the volume of each of the marker
  // elements. 
  // This is done for the percentages because of averaging of markers
  // in the same cell
  // In fact, this correction is very minor as long as the initial
  // circle is small
  real splatVolStart = M_PI * pow(theProblem->markerRadius*theProblem->dx,2) * hMadejski;
  real totalVolume=volume+splatVolStart;
  real percentagePerMarker = (splatVolStart/totalVolume)
    / (theProblem->numberOfRays * theProblem->markerPerCell) ;
  WRITE(percentagePerMarker);

//   for(elemsIter=elems.begin(); elemsIter!=elems.end(); ++elemsIter)
//     WRITE(elemsIter->vol_percentage);

  // Subtract this from vol_percentage of each marker
  real totalPercentage=0;
  for(elemsIter=elems.begin(); elemsIter!=elems.end(); ++elemsIter)
    {
      elemsIter->vol_percentage-= percentagePerMarker;
      if (elemsIter->vol_percentage <0)
	{
	  elemsIter->dead = true;
	  elemsIter->vol_percentage=0;
	  elemsIter->volume=0;
	}
      totalPercentage +=elemsIter->vol_percentage;
    }
  // This has destroyed the careful normalisation, so, normalise again
  for(elemsIter=elems.begin(); elemsIter!=elems.end(); ++elemsIter)
    {
      elemsIter->vol_percentage*= (1/totalPercentage);
    }

  totalPercentage =0;
  for(elemsIter=elems.begin(); elemsIter!=elems.end(); ++elemsIter)
    {
      elemsIter->vol_percentage= elemsIter->vol_percentage*elemsIter->vol_percentage;
      totalPercentage +=elemsIter->vol_percentage;
    }
  for(elemsIter=elems.begin(); elemsIter!=elems.end(); ++elemsIter)
    {
      elemsIter->vol_percentage*= (1/totalPercentage);
    }

//   std::cout << "###\n\n\n";
//   for(elemsIter=elems.begin(); elemsIter!=elems.end(); ++elemsIter)
//     WRITE(elemsIter->vol_percentage);

  while (volume > VOLUME_RESIDUAL) //1e-28
    {
      v = volume;
      elemsIter = elems.begin();

      while (elemsIter!= elems.end())
        {
	  if (!elemsIter->dead)
            {
	      elemsIter->volume += v * elemsIter->vol_percentage;
	      volume = volume - v * elemsIter->vol_percentage;
            }
	  elemsIter++;
        }
    }
//    std::cout << "###\n\n\n";
//   for(elemsIter=elems.begin(); elemsIter!=elems.end(); ++elemsIter)
//     std::cout << (elemsIter->vol_percentage) << " " << (elemsIter->volume) << std::endl;
//   exit(1);
}

double Marker::GetDeadVolume(void)
{
  //Volumen aller toten Marker addieren
  double deadVolume = 0;
  if (elems.size()==0) return 0.;
  elemsIter = elems.begin();
  while (elemsIter!= elems.end())
    {
      //totes Volumen, wenn Marker tot
      if (elemsIter->dead) deadVolume += elemsIter->volume;
      elemsIter++;
    }
    
  return deadVolume;
} 

bool Marker::VolumeZero()
{
  bool value = true;
  if (elems.size()==0) return true;
  elemsIter=elems.begin();
  while (elemsIter!= elems.end())
    {
      //Rueckgabewert FALSE, wenn ein lebender Marker noch Volumen besitzt, das groesser als mind. Volume ist
      if (elemsIter->volume > volume_every_step && !elemsIter->dead)
	{
	  value = false;
	  break;
	}

      elemsIter++;
    }
    
  return value;
}


// This next routine averages out the volume between all Markers
// inside a lattice cell.
void Marker::distributeVolume(Lattice &theLattice)
{
  Field<real> totalCellVolume(theLattice, 0.);
  Field<real> totalCellPercentage(theLattice, 0.);
  Field<int> cellMarkerCount(theLattice, 0);
  for(elemsIter=elems.begin(); elemsIter!=elems.end(); ++elemsIter)
    if (!elemsIter->dead) //nur lebende Marker ausgeben
    {
      int xx = (int)elemsIter->x;
      int yy = (int)elemsIter->y;
      totalCellVolume(xx,yy) += elemsIter->volume;
      cellMarkerCount(xx,yy)++;
      totalCellPercentage(xx,yy) += elemsIter->vol_percentage;
    }
  for(elemsIter=elems.begin(); elemsIter!=elems.end(); ++elemsIter)
    if (!elemsIter->dead) //nur lebende Marker ausgeben
    {
      int xx = (int)elemsIter->x;
      int yy = (int)elemsIter->y;
      // Take the volume percentage of the different markers into
      // account here
      elemsIter->volume=
	totalCellVolume(xx,yy) 
	*  (elemsIter->vol_percentage / totalCellPercentage(xx,yy));
    }
}

