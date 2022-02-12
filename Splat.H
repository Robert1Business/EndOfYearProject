//Hauptklasse der MC-Simulation

#ifndef _SPLAT_H
#define _SPLAT_H

#include "definitions.H"
#include "Point.H"
#include "Field.H"
#include "Lattice.H"
#include "LatticeIterator.H"
#include "NeighbourIterator.H"
#include "FluidState.H"
#include "NeighbourState.H"
#include "FieldProperty.H"
#include "Particle.H"
#include "Problem.H"
#include "Marker.H"
#include "renamer.h"
#include "rng.h"
#include "Distribution.H"

class Splat
{
private:
    
    Lattice theLattice;
    Problem *theProblem;
    Particle *droplet;
    Point *impactPoint;
    Marker *marker;
    
    int circulationCount;
    
    Field<real> height;        //aktuelle Substrathoehe (z.B. Hsub + H von zig Splats)
    Field<real> depositHeight; //aktuelle Splathoehe (Dicke des abzulegenden Splats)
    Field<FluidState> Fluid;   //Feld fuer Fluidstate (E, EN, FS, F)
    Field<NeighbourState> Neighbours; //Feld fuer Neighbourstate (N, S, O, W, NW, NO ...) 
    
    bool molten; //schmilzt Partikel

  // Local angles for impact on inclined substrate 
  double localImpactAngle, localDirectionAngle;
  
    
    void OutputParameters();  //Ausgabe der wichtigsten Parameter
    double depositedVolume(); //bereits abgelagertes Volumen
    
    real maxAbsField(Field<real> &field); //gibt groessten Betrag eines real-Feldes zurueck
    
    double CutFluid(Field<real> &heightToDeposit); //kuerzt das Hoehenfeld auf die Madejski-Hoehe und gibt das ueberschuessige Volumen zurueck
    
    void WriteVolumeData(double deltaAngle, string fileName); //zur Berechnung des tatsaechl. abgelag. Volumens
    //Plausibilitaetskontrolle durch Vergleich mit Int_a_b.int (gnuplot-Plots erstellen)

public:
    Splat(Problem &prob, Field<real> &currentHeight);
    virtual ~Splat();
    
    void Output(string filename);

    void Reset(); //Ausgangssituation erzeugen (Startkreis...)
    Field<real> Melt(void); //Ausbreitung des Splats
    void Smoothing( );
  void writeSplat2File(int splatNumber, Point &impactPoint);
};

#endif
