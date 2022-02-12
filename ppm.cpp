#include "ppm.h"

ppm::ppm(int w, int h, std::string file)
{
	width = w;
	heigth = h;
	filename = file;
	
	RKomp = new pgm (w,h,255,file);
	GKomp = new pgm (w,h,255,file);
	BKomp = new pgm (w,h,255,file);
}

ppm::~ppm()
{
	delete(RKomp);
	delete(GKomp);
	delete(BKomp);
}

int ppm::getWidth(void)
{
    return(width);
};


int ppm::getHeigth(void)
{   
    return(heigth);
};


std::string ppm::getFilename(void)
{   
    return(filename);
};

void ppm::setValue (int w,int h, int R, int G, int B)
{
    if (R >= 0 && G >= 0 && B >= 0)
	{
		RKomp->setValue(w,h,R);
		GKomp->setValue(w,h,G);
		BKomp->setValue(w,h,B);
	}
    else
        std::cout << "Wert muss groesser 0 sein!" << std::endl;
};

int ppm::GetColor(ColorCode cCode, ColorComponent cComponent, int value)
{
    if (cCode == GREY)    
        return value;
    if (cCode == HEAT)
    {  
        //HSV-Farben        
        double h = value / 255.*240.; //normieren auf 240° des H-Anteils
        double s = 1.;
        double v = 1.;

        //Umwandelung HSV->RGB (Wikipedia)
        int hi = (int)(h / 60.0) % 6;
        double f = h / 60.  - (double)hi;
        double p = v * (1. - s);
        double q = v * (1. - f * s);
        double t = v * (1. - (1. - f) * s);

        //RGB-definieren
        double r;
        double g;
        double b;

        switch(hi){
            case 0:
                r = v; g = t; b = p;
                break;
            case 1:
                r = q; g = v; b = p;
                break;
            case 2:
                r = p; g = v; b = t;
                break;
            case 3:
                r = p; g = q; b = v;
                break;
            case 4:
                r = t; g = p; b = v;
                break;
            case 5:
                r = v; g = p; b = q;
                break;
        };                 
        
        //Schwarz und Weiß gesondert behandeln (beibehalten)
        //if ((int)(r*255.) == 255 && (int)(g*255.) ==  0 && (int)(b*255.) == 0) return 0;
        if ((int)(r*255.) == 0 && (int)(g*255.) ==  0 && (int)(b*255.) == 255) return 255;

        if (cComponent == R)
        {                                
            return (int)(r*255.);
        }
        if (cComponent == G)
        {
            return (int)(g*255.);
        }
        if (cComponent == B)
        {
            return (int)(b*255.);
        }
    } 
    else
        return value; 
}


void ppm::pgm2ppm(std::string fileNamePgm, ColorCode cCode)
{
    pgm pgmPic(fileNamePgm);    
     
    std::string fileNamePpm = fileNamePgm.erase(fileNamePgm.size() - 4, fileNamePgm.size()) + ".ppm";    

    std::ofstream f (fileNamePpm.c_str(), std::ios::out|std::ios::binary);
     
     //Header schreiben für PPM
     char t1 = 0x0A; //Trennzeichen1
     char t2 = 0x20; //Trennzeichen2
     f<<"P6"<<t1<<pgmPic.getWidth()<<t2<<pgmPic.getHeigth()<<t1<<pgmPic.getGrey()<<t1<<std::flush;
     
     //Bilddaten schreiben
     for(int i1 = 1; i1 <= pgmPic.getHeigth(); i1++)
        for(int i2 = 1; i2 <= pgmPic.getWidth(); i2++)
	    {
            //Rot-Komponente:                   
            f.put(GetColor(cCode, R, abs(pgmPic.getValue(i2,i1)-255)));
            //Grün-Komponente:
            f.put(GetColor(cCode, G, abs(pgmPic.getValue(i2,i1)-255)));
            //Blau-Komponente:
            f.put(GetColor(cCode, B, abs(pgmPic.getValue(i2,i1)-255)));
	    }
       
     f.close();
}


void ppm::Save(void)
{   

    std::ofstream f (filename.c_str(), std::ios::out|std::ios::binary);
     
     //Header schreiben für PPM
     char t1 = 0x0A; //Trennzeichen1
     char t2 = 0x20; //Trennzeichen2
     f<<"P6"<<t1<<width<<t2<<heigth<<t1<<255<<t1<<std::flush;
     
     //Bilddaten schreiben
     for(int i1 = 1; i1 <= heigth; i1++)
        for(int i2 = 1; i2 <= width; i2++)
	    {
            //Rot-Komponente:                   
            f.put(RKomp->getValue(i2,i1));
            //Grün-Komponente:
            f.put(GKomp->getValue(i2,i1));
            //Blau-Komponente:
            f.put(BKomp->getValue(i2,i1));
	    }
       
     f.close();
}
