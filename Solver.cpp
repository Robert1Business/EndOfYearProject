#include "Solver.h"

//numerische Integration des Kreisvolumens mit Angabe der Elementlänge
double Solver::SphereIntegration(Plain3 &p1, Plain3 &p2, Vector3 &p)
{
	double volume = 0;
	double dx = dl;
	double dy = dl;
	double dz = dl;
	double radius = r;

	//Volumen in einzelne Teile hacken
	//Kugel: 1<x^2+y^2+z^2
	//Kugelmittelpunkt in 0,0,0

	for (double x = -radius; x < radius; x+=dx)
	{
		for (double y = -radius; y < radius; y+=dy)
		{
			for (double z = -radius; z < radius; z+=dz)
			{
				Vector3 tmp (x,y,z);
				if (PointInVolume(p, p1, p2, tmp))			
				{
					volume += dx * dy * dz;
				}
			}
		}
	}
	return volume;
}

//Monte-Carlo Integration des Kugelvolumens; benötigt Anzahl der Punkte
double Solver::MCSphereIntegration(Plain3 &p1, Plain3 &p2, Vector3 &p)
{
	double volume = 0;
	int pointsInVolume = 0;
	RNG RngValue; //Zufallszahlen        
    
	for (int i = 0; i < numPoints; i++)
	{
		Vector3 tmp (RngValue.uniform(-r,r), RngValue.uniform(-r,r), RngValue.uniform(-r,r));
                              
		if (PointInVolume(p, p1, p2, tmp)) 
			pointsInVolume ++;
	}

	volume = (double)pointsInVolume / (double)numPoints*8;	return volume;
}



bool Solver::PointInSphere(Vector3 &p)
{
	bool value=false;
	
	//Pruefung, ob Ursprungsvektor in Kugel
	if (p.X*p.X + p.Y*p.Y + p.Z*p.Z < 1)
		value = true;
	
	return value;
}

bool Solver::PointInVolume(Vector3 &p, Plain3 &p1, Plain3 &p2, Vector3 &recentPoint)
{
	bool value = false;
         
	if (p1.GetSign(p) == p1.GetSign(recentPoint) && p2.GetSign(p) == p2.GetSign(recentPoint) && PointInSphere(recentPoint))		value = true;

	return value;
}

Solver::Solver(double impactAngle_, double directionAngle_, double dAngle_, int numPoints_, std::string resultfile_)
{
	//Attribute schreiben
	impactAngle = impactAngle_;
	directionAngle = directionAngle_;
	resultfile = resultfile_;
	dAngle = dAngle_;
	numPoints = numPoints_;
	r = 1; //Radius der zu integrierenden Kugel
}

void Solver::solveIntegral(void)
{
	std::cout<<"Integration mit"<<std::endl;
	std::cout<<"    impactAngle="<<impactAngle<<std::endl;
	std::cout<<"    directionAngle="<<directionAngle<<std::endl;
	std::cout<<"    Schrittweite="<<dAngle<<std::endl;
	std::cout<<"    Anzahl Punkte fuer MC-Algorithmus:"<<numPoints<<std::endl;	
	std::cout<<"    Ausgabedatein:"<<resultfile<<std::endl<<std::endl;
	
	//wenn resultfile existiert, dann löschen, um neu zu erstellen
	remove(resultfile.c_str());
	std::ofstream resf (resultfile.c_str(), std::ios::app); //Datei öffnen	

	//Winkel in Radiant umrechnen	
	double impactAngleRad = 2 * M_PI / 360 * impactAngle;
	double directionAngleRad = 2 * M_PI / 360 * directionAngle;
	double dAngleRad = 2 * M_PI / 360 * dAngle;

	double angle = dAngle; //Laufvariable
	
	//Auftreffpunkt
	Vector3 impactPoint(0, 0, -r); 
	
	//normierter Geschwindigkeitsvektor aus impactAngle und directionAngle berechnen (Polarkoordinaten)
	Vector3 v;
	v.X = sin(-impactAngleRad) * cos(directionAngleRad);
	v.Y = sin(-impactAngleRad) * sin(directionAngleRad);
	v.Z = cos(-impactAngleRad);
	//v.Negate();
	v.Normalize();

	//Vektor vom Ursprung bis Ende v
	Vector3 vPoint = impactPoint + v;

	//Vektor, der erste Ebene aufspannt
	Vector3 v1 = Vector3::CrossProduct(impactPoint,v);
	v1.Normalize();
	//Vektor drehen, so dass er Richtung x-Achse zeigt (entspricht nach Definition 0 Grad)	
	v1.Turn(v,2*M_PI/360*(-90-directionAngle));
	v1.Y = 0; //kleine Ungenauigkeit beim Drehen ausgleichen (Vektor zeigt genau in X-Richtung)

	
	//Vektor, der zweite Ebene aufspannt
	Vector3 v2 = v1;
	v2.Turn(v,dAngleRad);
	
	//Vektor, der zwischen v1 und v2 liegt
	Vector3 h = 0.5*v1; //Hilfsvektor, der mitdreht
	h.Turn(v,dAngleRad/2);
	Vector3 p = impactPoint+v+h; //Punkt,der zwischen Ebenen liegt (Ursprungsvektor)

	//Ausgabe der Vektoren in Datei
	std::string vFile = "vectors.dat";
	remove(vFile.c_str());//alte Datei löschen	
	impactPoint.appendToFile(origin, vFile);
	v.appendToFile(impactPoint, vFile);
	
	//Integration über alle Winkel, indem die beiden Ebenen weiter gedreht werden
	for (angle; angle <= 360; angle+=dAngle)
	{
		//Ausgabe		
		v1.appendToFile(vPoint, vFile);
		v2.appendToFile(vPoint, vFile);
		p.appendToFile(origin, vFile);
		
		//Ebenen aufspannen
		Plain3 p1 (impactPoint,v,v1,false);
		Plain3 p2 (impactPoint,v,v2,false);
		
		//Kugelausschnitt integrieren		
		//double volume = SphereIntegration(p1, p2, p);
		double volume = MCSphereIntegration(p1, p2, p); //Monte-Carlo-Integration

		if (angle-dAngle > 0) resf<<std::endl;		
		std::cout<<"Volumen von "<<angle-dAngle<<" bis "<<angle<<" Grad:"<<volume<<std::endl;
		resf<<angle-dAngle<<" "<<angle<<" "<<volume;	

		v1.Turn(v,dAngleRad);//v1 weiter drehen
		v2.Turn(v,dAngleRad);//v2 weiter drehen
		h.Turn(v,dAngleRad);//h weiter drehen
		p = impactPoint+v+h;
	}

	resf.close();
		
	std::cout<<"Integration beendet"<<std::endl;
	
}
