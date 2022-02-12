//Version 1 (Januar 2008)
//Klasse zum Erstellen von PPM-Graphiken
//unterst�tzt 24Bit Farben
#ifndef __PPM_H
#define __PPM_H

#include <iostream>
#include <string>
#include <fstream>
#include <math.h> 
#include <iomanip>
#include "pgm.h"


enum ColorCode {HEAT, GREY}; //Codierung von Grauwerten in Farbwerte
enum ColorComponent {R,G,B};

class ppm
{
   private:
        
        //Farbkomponenten
        pgm *RKomp;
        pgm *GKomp;
        pgm *BKomp;

        //Bildparameter
        int width;
        int heigth;
        std::string filename;

   public:
        static int GetColor(ColorCode cCode, ColorComponent cComponent, int value);
        ppm(int w, int h, std::string file);
        ~ppm();

        void Save(void);       

        static void pgm2ppm(std::string fileNamePgm, ColorCode cCode);

        int getWidth(void); //�bergibt Bildbreite
        int getHeigth(void); //�bergibt Bildh�he
        std::string getFilename(void); //�bergibt Dateinamen

        void setValue (int w,int h, int R, int G, int B);
};


#endif
