//Klasse zur Farbgenerierung und Umrechnung
//alle Farbwerte sind von 0- 255 definiert
#ifndef __COLOR_H
#define __COLOR_H

#include <iostream>
#include <list>
#include <cmath>


enum COLORSYSTEM {RGB};
enum COLORCODE {HEAT, NO}; //Codierung von Grauwerten in Farbwerte

//3-Komponentenfarbe
struct K3Color
{
    int k1;
    int k2;
    int k3;
    int gray;
};

class Color
{
    private:
        int gray_;
        K3Color color_; //gespeicherte Farbwerte
        COLORSYSTEM color_system_; //gesetztes Farbsystem, bis jetzt nur RGB
        COLORCODE color_code_; //Farbkodierung (Berechnung von Farbwerten aus Graustufe)
        std::list<K3Color> defined_color_code_; //gespeicherte Farben zum Interpolieren von Benutzerdefinierten color_code
        std::list<K3Color>::iterator cIter; // Iterator definieren
        
        bool TrueColor(int col); //test, ob richtige Farbe; Fehler ausgeben und fehlerhafte Farbe anders setzen
        void CalcGray(); //neuer Grauwert aus Farbe berechnen
        void CalcColor(); //Farbwert aus Grauwert berechnen (nach color_mode_ kodiert) //!
        
        //Farbcode setzen
        void SetHEAT();
        void SetNO();
    
    public:
        Color(); //Standardkonstruktor, RGB, NO 
        Color(int k1, int k2, int k3); //Farbe in RGB geben 
        Color(COLORCODE ccode,  int gray); //aus Graustufe Farbe berechnen
        virtual ~Color(); 
    
        //Rueckgabe der Farben als struct
        K3Color GetColor();
       
        //Rueckgabe von Einzelkomponenten
        int GetK1();
        int GetK2();
        int GetK3();
        int GetGray();
       
        //Setzen von Farben
        void SetColor(int k1, int k2, int k3); //Farbe mit 3 Komponenten setzen- Interpretierung nach color_system_
        void SetGray(int gray);
    
        //Farbkodierung setzen
        void SetColorCode(COLORCODE ccode);
    
        //spezielle Farboperationen
        void InvertColor();
    
        //Zuweisungsoperatoren
        //hier = Operator ueberladen!!!!

};

#endif
