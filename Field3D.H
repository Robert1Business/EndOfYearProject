//Template Klasse für 3dimensionale Matrix
//Feld beginnt bei 0!
#ifndef __FIELD3D_H
#define __FIELD3D_H

#include <iostream>
#include "pgm.h"
#include <cstring>
#include "Point.H"
#include "Field.H"
#include "rng.h"
#include "ppm.h"
#include "Problem.H"
#include "Vector3.h"


template <class T>
class Field3D{

private:
       //Field3D (const Field3d<T>&); //CopyKonstruktor
       unsigned x_;
       unsigned y_;
       unsigned z_;

       int numObjects_;
       
       T defaultValue; //standardwert
       
       T *elems;
        
       bool iterend_;
       
       unsigned iterx_;
       unsigned itery_;
       unsigned iterz_;
       
       T depositedParticles_; //Maximalwert, der geschrieben wurde Grauwertcolorierung

protected:       
inline       int makeIndex(unsigned x, unsigned y, unsigned z );

public:
       void StartIteration(); //rücksetzen der Iterationsoperation
       Field3D<T>& operator++(int); //++Operator überladen für Schleifen
       bool EndIteration();
       
       //Field3D();
       Field3D(unsigned x, unsigned y, unsigned z, T defval, int numObjects); 
       //numObjects = Anzahl der Splats plus Stufe für die Grauwertcodierung der Schnitte
       //  This constructor reads a Field3D from a file with given name
       Field3D(unsigned x, unsigned y, unsigned z, std::string filename);
  
       virtual ~Field3D();
       
       //Dimensionen zurückgeben
       int getXDim();
       int getYDim();
       int getZDim();
       
       T& at(unsigned x, unsigned y, unsigned z); //Wertrückgabe mit Angabe der Position
       T& at(void);//ohne Position (Position hier der interne Iterator)
       bool set(unsigned x, unsigned y, unsigned z, T value); //Wert setzen
       void set(T value);//ohne Position (Position hier der interne Iterator)
       
       bool indexOk(unsigned x, unsigned y, unsigned z );
       void Reset(); //alle Werte auf defaultValue
       
       void Print();
       T& Max(); //liefert Maximalwert des Arrays
       
       //Schnitte durch 3D-Array in PGM umwandeln
       pgm XYLayer(unsigned z);
       pgm XZLayer(unsigned y);
       pgm YZLayer(unsigned x);
    
       double getCoatingHeight(int x, int y, double scaleZ);
       Vector3 getNormalVector(int x, int y, double scale);
 
       void writeMatlabfile();
       void writeTecplotfile();
       void writeTecplotfile(string filename);
       bool findColor(int x, int y, int colorCode, int &zLow, int &zHigh);
       void writePovrayFile(string filename, Problem & theProblem);
       
       void Deposit(Point p,Field<real> &field, double dz);
       void DepositNew(Field<real> &field, Field<real> &Ground, double dz);
       Field3D<T>& slice(string coord, int xmin, int xmax);
       Field3D<T>& sliceZSurface();
};

void modulo(Field3D<int> & f, int mod);

#endif
       

