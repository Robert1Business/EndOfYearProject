//Klasse zum Lösen des Integrals
#ifndef __SOLVER_H
#define __SOLVER_H

#include <iostream>
#include <string>
#include <fstream>
#include <math.h>
#include <list>
#include "Vector3.h"
#include "Plain3.h"
#include "rng.h"

class Solver{

private:
    std::string resultfile;
    double impactAngle;
    double directionAngle;
    double dl;
    double dAngle;
    int numPoints; //Anzahl Punkte für MC-Integration
    double r; //radius

    bool PointInSphere(Vector3 &p); //prüft, ob Punkt innerhalb der Kugel mit radius r

    //p1,p2 -> Ebenen durch Kugel
    //recentPoint-> gesuchter Punkt
    //p->Punkt der Volumen zwischen Ebenen definiert, das "true" zurückgibt    
    bool PointInVolume(Vector3 &p, Plain3 &p1, Plain3 &p2, Vector3 &recentPoint);

    //Integrationsalgorithmus; p-> Punkt liegt in zu integrierendem Volumenteil, das von den Ebenen eingeschlossen wird
    double SphereIntegration(Plain3 &p1, Plain3 &p2, Vector3 &p);
    
    //MC-Integration;  p-> Punkt liegt in zu integrierendem Volumenteil, das von den Ebenen eingeschlossen wird
    double MCSphereIntegration(Plain3 &p1, Plain3 &p2, Vector3 &p);

public:
    //directionAngle-> Hauptausbreitungsrichtung in Deg
    //impactAngle-> Aufprallwinkel in Deg
    //dAngle-> Schrittweite des Winkels in Deg
    //numPoints-> Anzahl Punkte für MC-Integration
    Solver(double impactAngle_, double directionAngle_, double dAngle_, int numPoints_, std::string resultfile_);

    void solveIntegral(void); //löst Integral und schreibt Lösung in Datei

};

#endif
