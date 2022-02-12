//3D- Ebenenklasse, die Vektorklasse Vector3 benutzt
#ifndef __PLAIN3_H
#define __PLAIN3_H

#include "Vector3.h"

class Plain3{
private:
	//Punkte, die Ebene aufspannen (Ursprungsvektoren)
        Vector3 p1;
        Vector3 p2;
        Vector3 p3;

        //aus den Ursprungsvektoren p1-p3 berechnete Vektoren, die Ebene aufspannen
        Vector3 a;
        Vector3 b;

public:
	//true-> p bfindet sich auf der positiven Seite der Ebene	
	bool GetSign(Vector3 &p);

	//allPoints = true-> alle Vektoren vom Ursprung
        //ansonsten wird Ebene direkt von p2_ und p3_ aufgespannt
	Plain3 (Vector3 &p1_, Vector3 &p2_, Vector3 &p3_, bool allPoints);
};

#endif
