// pgm.h created by Philipp Seiler
// generated: 15:02 09.04.2008 
// Version: 2.6
//
// simple PGM-class
//

//Momentan werden nur pgm's mit 256 Bit Graustufen verarbeitet!
#ifndef __PGM_H
#define __PGM_H

#include <iostream>
#include <string>
#include <fstream>
#include <math.h> 
#include <iomanip>


//Matrix-Klasse -> stellt alle nötigen Matrixoperationen zur Verfügung, die
//PGM- Klasse benötigt
class matrixC
{
    friend std::ostream& operator<<(std::ostream&, const matrixC&);
    friend matrixC operator*(const matrixC&, const matrixC&);
    // Transponieren der Matrix
    friend matrixC transpose(const matrixC &m);
    // Multiplikation aller Matrixelemente mit einem konstanten Faktor
    // Implementierung für die Fälle: matrix*int und int*matrix
    friend matrixC operator*(const matrixC&, int);
    friend matrixC operator*(int, const matrixC&);
    
    protected:
        int numrows, numcolumns;
        int *all_elements;

    public:
        // Konstruktoren
        matrixC(int n, int m);
         
        // Destruktor
        ~matrixC(void);
            
        // Überladene Operatoren (als Memberfunktion)
        // Vergleichsoperator
        bool operator==(const matrixC&) const;
    
        // Zuweisungsoperator
        matrixC& operator=(const matrixC&);
    
        // Funktionsaufrufoperator
        int& operator()(int row, int column) const;
        
        // Sonstige Methoden
        void set_value(int i, int j, int wert);
        int get_value(int i, int j);
        int get_numrows() const;        // Rückgabe der Zeilenanzahl
        int get_numcolumns() const;     // Rückgabe der Spaltenanzahl      
};



//eigentliche Klasse zur PGM-Verarbeitung
class pgm
{
 friend pgm fillOther(pgm &m, int x, int y, int fill);
 //wie boundaryFill, nur dass anderes PGM- Objekt (wird zurückgegeben) mit gegebener Farbe gefüllt wird
 
	
 private:
        //Bildeigenschaften
        int width;
        int height;
        int greyscale; //Graustufenwert 
        
        std::string filename;
        matrixC *picture; //Zeiger auf Matrix mit Bildangaben
        
 protected:
       //Methoden, um Bildgröße zu ändern
       void setWidth(int w);
       void setHeigth (int h);
       int** copyObjektToArray (void); //gibt Pointer auf Array zurück (Array enthält Graustufenwerte)
       void copyArrayToObjekt (int ** mat, int h, int w); //überschreibt Bildwerte des Objektes mit einem Array
        
 public:
        
        pgm(void); // legt leeres Objekt an (interne Daten vorhanden z.B. mit getWidth abrufbar)
        pgm(int w, int h, int greys, std::string file); 
        //legt neues Bild mit Angaben über Höhe, Breite, Grauwert
        //(momentan nur 255) und Dateinamen an
        pgm(std::string file); //vorhandene Datei öffnen
        
        virtual ~pgm(void); //Destruktor
        
        int getGrey(void); //Übergibt Graustufenwert
        int getWidth(void); //Übergibt Bildbreite
        int getHeigth(void); //Übergibt Bildhöhe
        std::string getFilename(void); //Übergibt Dateinamen
        
        int getValue(int w,int h); //Rückgabe eines Pixels; Anfang bei Position 1,1
        void setValue (int w,int h, int value); //Manipulation der Bild-Matrix; Zeichnet Punkt
        
        void saveAs (std::string file); //speichern des aktuell geladenen Bildes in neue Datei
        void Save (void); //speichern des aktuell geladenen Bildes in ursprüngliche Datei
        
        void Clear(void); //setzt alle Bildpunkte auf 0
        void Invert(void); //Invertiert Bilddaten
        void p_to_pLine (int x0, int y0, int x1, int y1, int greyscale); 
        //Zeichnet Linie von Punkt zu Punkt
        void Line (int x0, int y0, int length, double angle, int greyscale);
        //Zeichnet Line von Startpunkt mit geg. Laenge und Winkel
        void angleLine (int x0, int y0, double angle, int greyscale);
        //Zeichnet Line von Startpunkt mit geg. Winkel bis Bildende
        void Cross (int x0, int y0, int length, int greyscale); //Zeichnet Kreuz
        void fCircle (int x0, int y0, int radius, int greyscale); //Zeichnet ausgefüllten Kreis
        
        //Methoden zur Spiegelung
        void mirrorX(void); //spiegel in X-Richtung (Spiegelachse in der Mitte)

        //die folgenden Methoden geben jeweils in X- und Y- Richtung die Koordinaten
        //des ersten Auftretens eines Grauwertes aus- so kann man die Projektionen einer
        //zusammenhängenden Fläche, auf die Koordinatenachsen bezogen, berechnen;
        //wenn 0 zurückgegeben wird, ist Grauwert nicht vorhanden
        int minColorAreaX (int greyscale);
        int maxColorAreaX (int greyscale);
        int minColorAreaY (int greyscale);
        int maxColorAreaY (int greyscale);
        
        int nextColorPoint(int &x0, int &y0, double angle, int greyscale);
        //x0, y0 -> Call-by-Referenz
        //diese Koordinaten enthalten nach Methodenaufruf die neuen Koordinaten
        //des Punktes mit dem gegebenen Graustufenwert entlang einer Linie mit angegebenen Winkel
        // für x0 & y0 = 0 -> keinen entspr. Punkt gefunden;
        //gibt Abstand zwischen den beiden Punkten zurück
    	int nextPoint(int &x0, int &y0, double angle);
    	//wie nextColorPoint, nur dass Algorithmus nächsten Punkt wiedergibt, der nicht
    	//Farbe des Anfangspunktes entspricht
        
        pgm& operator=(const pgm&); //Zuweisung bzw. kopieren
        
        int maxColor (void); //gibt höchsten Farbwert zurück
        int colorValue (int greyscale); //gibt Gesamtzahl der Pixel einer Farbe zurück
        void boundaryFill(int x, int y, int fill, int greyscale);
        //Füllalgorithmus, der zusammenhängende Fläche einer gegebenen Farbe füllt
        // fill -> Füllfarbe; greyscale -> zu füllende Farbe

	    int* frequencyDistribution(void); //gibt Häufigkeitsverteilung einzelner Farben zurück als Array von int (von 0 bis höchste vorhandene Farbe)
	    void Quantize (int levels); //einfacher Quantisierungsalgorithmus; erwartet Anzahl der Farben
	    
	    void scaleNearNeigh(double scaleFactorX, double scaleFactorY); //skalieren mit Nearest Neighbor Interpolation
};

#endif
