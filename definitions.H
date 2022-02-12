#ifndef _DEFINITIONS
#define _DEFINITIONS

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <map>
#include <string>
#include <stdexcept>
#include <ctime>
#include <stdarg.h>
#include <algorithm>

using namespace std;

//einfache Ausgabe von Variablen
#define OUT(variable) "" << ( #variable ) << ": " << ( variable )
#define WRITE(variable) std::cout << OUT(variable) << std::endl

//unrealistische Parameter
#define NIRWANA 10000000. // A real coordinate that marks invalid points. 
#define UNREAL_VELOCITY -1e10 //unrealistische Geschwindigkeit zur Initalisierung
const double heightTolerance=1.e-15;


//Berechnung MIN/MAX zweier Werte
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define MAX_ANGLE 80 //maximaler Aufprallwikel eines Teilchens

#define VOLUME_RESIDUAL 1e-28 //benoetigter Grenzwert fuer Mat.-Verteilung (entspricht Rest)
                              //ab diesem Wert wird das abgeschnittene Volumen nicht mehr iterativ verteilt 

//Werte zur Loesung des Integrals beim Schraegaufprall
#define MC_POINTS 5000000 //Anzahl der Integrationspunkte der MC-Integration (je mehr, desto besser, aber langsam)
#define DELTA_ANGLE 10    //Ebenen immer um diesen Winkel versetzen (in Deg)

//Startposition der Marker  -- now in Problem.H/Problem.C
// #define NUMBER_OF_RAYS 1440 //Anzahl der Strahlen bei radialer
// 			    //Anordnung, jedes 1/8 Grad ; default-Wert 2880
// #define MARKER_PER_CELL 1   //Anzahl der Marker^2 pro Zelle (4 -> 16 Marker)
//                             //bei radialer Verteilung: Anzahl der Marker pro Strahl (4 Stck)
// #define MARKER_WIDTH 1. //Breite des Rings mit Markern; 1
// 			//entspr. einer Zelle (Markeranzahl =
// 			//MARKER_PER_CELL)  Default value is 1.

//Werte fuer mark-Feld
#define DEAD 0  //Zelle, die von anderen Fluidzellen umgeben wird (keine Partikel noetig -> werden geloescht)
#define ALIVE 1 //lebende Zelle mit Partikeln (Oberflaechenzellen und deren Nachbarn)
#define BLANK 2 //kein Fluid (Hindernis oder leer)

//weitere Werte fuer ApplyRules - now in Problem.C
// #define SPLASH_VALUE 4.e-6 //obere Grenze fuer Stufe auf- und abwaerts, Hindernis unueberwindbar, Marker tot
// #define MEDDELTA 1.5e-6    //mittlere Grenze fuer Stufe aufwaerts 2.5e-6
// #define SMALLDELTA 1.e-6   //untere Grenze fuer Stufe abwaerts, Ausbreitung normal, Ueberfliessen

//Element der Partikel-Liste (fuer Marker-Klasse)
struct MARKER_ELEM
{
    double x; //x-Koordinate des Partikels
    double y; //y-Koordinate des Partikels
    double u_vel; //x-Geschwindigkeit
    double v_vel; //y-Geschwindigkeit
    bool dead; //ist Partikel tot (dann keine Bewegung mehr moeglich)
    double volume; //noch zu verteilendes Volumen
    double vol_percentage; //prozentualer Anteil des Markers am Gesamtvolumen des Tropfens
  double oldStepHeight; // height of the previous step to allow moving
			// along steep slopes
  bool reflected; // Keep track whether particle was reflected at a wall
};

typedef double real; //real Definition (der selbe interne Datentyp wie double)

double degToRad(double angle); //Umwandlung von Deg in Rad
double radToDeg(double angle); //Umwandlung von Rad in Deg

#endif
