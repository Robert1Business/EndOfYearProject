#ifndef _MARKER_H
#define _MARKER_H

#include "definitions.H"
#include "Lattice.H"
#include "LatticeIterator.H"
#include "Field.H"
#include "FluidState.H"
#include "NeighbourIterator.H"
#include "NeighbourState.H"
#include "FieldProperty.H"
#include "Point.H"
#include "Particle.H"
#include "Problem.H"
#include "renamer.h"
#include "rng.h"
#include "Solver.h"
#include <fstream>
#include <list>
#include <vector>

enum MarkerState {Flows, SplashesDown, Dies, Undefined};



class Marker
{
private:
  list<MARKER_ELEM> elems; //Doppelt verkettete Liste mit Elementen
  list<MARKER_ELEM>::iterator elemsIter; // Iterator definieren
  list<MARKER_ELEM>::iterator fastestMarker; //Zeiger auf schnellsten Marker

  //Vektoren fuer eingelesenes Integral
  double wholeVolRate;	
  double d_angle; //Winkelschritte des Integrals	
  // 22/08/08: Use a map for the volume Integral
  map<int, double> volIntegral;

  // 01/09/08: The phase is for fingering only
  double phase;
  
  Problem* theProblem;
  Particle* droplet;
  
  Lattice theLattice;
  //setzt neue Marker auf Feld (strahlenfoermig)        
  void SetMarkerRadiant(Field<FluidState> &Fluid, double impactAngle, double directionAngle); 
  //Regeln werden angewendet und neue Partikeleigenschaften berechnet,
  //fuer aktuelles elemsIter-Element
  void ApplyRules(Field<real> &depositHeight, Field<real> &Ground); 
  double CalcNorm2D(double u, double v); //Rechnet Betrag aus Geschwindigkeitskomponenten 
        
  double r_; //Aussenradius
  Point impactPoint_;
  int number; //Anzahl der Marker
  double volume_to_deposit; //abzulagerndes Volumen
  double volume_every_step; //Hoehe, die nach jedem Schritt und pro Marker das Hoehenfeld erhoeht
  double hMadejski; 
  
  //Loest Integral des Schraegaufpralls und liest Werte ein
  void ReadVolPercentage(double impactAngle, double directionAngle); 
  //Gibt Volumenanteil eines Markers zurueck
  double GetVolPercentage(double angle, double impactAngle, double directionAngle);
        
public:
  // 28/08/08: this is to detect splashing of the droplet
  bool splashed;
  //Neue Position aus Geschwindigkeit errechnen
  void CalcNewPosition(Lattice &theLattice, Field<FluidState> &Fluid, Field<real> &depositHeight,
		       Field<real> &Ground, Field<real> &heightToDeposit); 
  void SetVolume(double volume, double heightMadejski); //setzt Markervolumen
  bool VolumeZero(); //wenn Marker ein Volumen kleiner als volume_every_step hat
  void SlowDownMarker(double slowDownFactor, double slowDownNorm);//verlangsamt alle Marker
       
  double GetDeadVolume(void); //gibt Volumen zurueck, das nicht mehr ausgebreitet wird, da weggespritzt
  void SniffAhead(Field<real> &Ground, double & minH, double& maxH);
  
       
  Marker(Lattice localLattice, Field<FluidState> &Fluid,  double r, Point impactPoint,double impactAngle, double directionAngle, Problem * probIn, Particle* dropletIn); //Konstruktor
  ~Marker();
  //neues Partikel erstellen (anhaengen), angle -> Winkel vom Zentrum des Feldes (fuer Geschwindigkeitsberechnung)       
  void New(double x, double y, double angle, double impactAngle, double directionAngle); 
  void DelAll(void); //Liste leeren
  void DelDead(void); //loesche alle nicht noetigen Partikel (in "toten" Element)
       
  void Output(); //Ausgabe aller Partikelkoordinaten
       
  int GetNumber(); //Gibt Anzahl der gespeicherten Elemente zurueck
  void distributeVolume(Lattice &theLattice);
  void write();
};

#endif
