#include "Color.h"

void Color::SetHEAT()
{
}

void Color::SetNO()
{
    K3Color new_elem;
    new_elem.k1 = 0;
    new_elem.k2 = 0;
    new_elem.k3 = 0;
    new_elem.gray = 0;
    defined_color_code_.push_back (new_elem);
    new_elem.gray = 255;
    defined_color_code_.push_back (new_elem);
}

bool Color::TrueColor(int col)
{
    bool value;

    if (col > 255 || col < 0)
        value = false;
    else
        value = true;

    return value;
}

void Color::CalcGray()
{
    //aus Farbe Grauwert errechnen mit Formel aus Gimp:
    //www.lernnetz-sh.de/kmLinux/doc/Grokking-the-GIMP-v1.0/node54.html
    // Y = 0.3*R + 0.59*G + 0.11*B
    gray_ = (int)(0.3*(double)color_.k1 + 0.59*(double)color_.k2 + 0.11*(double)color_.k3);
}

void Color::CalcColor()
{
    if (color_code_ == NO)
    {
        color_.k1 = gray_;
        color_.k2 = gray_;
        color_.k3 = gray_;
    }
    else
    {
        cIter = defined_color_code_.begin();
        //for(elemsIter=elems.begin(); elemsIter!=elems.end(); elemsIter++)
        while(cIter!=defined_color_code_.end() || cIter->gray >= gray_)
            cIter++;
        
        //Farben Interpolieren
        if (cIter->gray = gray_)
        {
            //Wenn Farbwert direkt kodiert, dann nur kopieren
            color_.k1 = cIter->k1;
            color_.k2 = cIter->k2;
            color_.k3 = cIter->k3;
        }
        else
        {
            //ansonsten interpolieren
            int k1H = cIter->k1;
            int k2H = cIter->k2;
            int k3H = cIter->k3;
            int grayH = cIter->gray;
            cIter--;
            int k1L = cIter->k1;
            int k2L = cIter->k2;
            int k3L = cIter->k3;
            int grayL = cIter->gray;
            
            //berechnen der Parameter zur Interpolation (y = a*x+b)
            double a1 = (double)(k1H - k1L)/(double)(grayH - grayL); 
            double a2 = (double)(k2H - k2L)/(double)(grayH - grayL); 
            double a3 = (double)(k3H - k3L)/(double)(grayH - grayL);
            double b1 = k1L - a1*grayL;
            double b2 = k2L - a2*grayL;
            double b3 = k3L - a3*grayL;
            
            //Anwendung formel zur linearen Interpolation (y = a*x+b)
            color_.k1 = (int)round(a1*gray_ + b1);
            color_.k2 = (int)round(a2*gray_ + b2);
            color_.k3 = (int)round(a2*gray_ + b3);
        }
    }
}


Color::Color()
{
    gray_ = 0;
    color_.k1 = 0;
    color_.k2 = 0;
    color_.k3 = 0;
    color_system_ = RGB;
    color_code_ = NO;
}

Color::Color(int k1, int k2, int k3)
{
    color_system_ = RGB;
    if (!TrueColor(k1))
    {
        k1 = 0;
        std::cout<<"Komponente1 ausserhalb des Wertebereichs- auf 0 gesetzt!"<<std::endl;
    }
    if (!TrueColor(k2))
    {    
        k2 = 0;
        std::cout<<"Komponente2 ausserhalb des Wertebereichs- auf 0 gesetzt!"<<std::endl;
    }
    if (!TrueColor(k3))
    {    
        k3 = 0;
        std::cout<<"Komponente3 ausserhalb des Wertebereichs- auf 0 gesetzt!"<<std::endl;
    }
    color_.k1 = k1;
    color_.k2 = k2;
    color_.k3 = k3;
    color_code_ = NO;
    CalcGray();
}

Color::Color(COLORCODE ccode,  int gray)
{
    color_system_ = RGB;
    gray_ = gray;
    color_code_ = ccode;
    CalcColor();
}

Color::~Color()
{
    //alle Elemente löschen
    defined_color_code_.clear();
}

K3Color Color::GetColor()
{
    return color_;
}

int Color::GetK1()
{
    return color_.k1;
}

int Color::GetK2()
{
    return color_.k2;
}

int Color::GetK3()
{
    return color_.k3;
}

int Color::GetGray()
{
    return gray_;
}

void Color::SetColor(int k1, int k2, int k3)
{
    //Farben testen, ob Werte zwischen 0 und 255
    if (!TrueColor(k1))
    {
        k1 = 0;
        std::cout<<"Komponente1 ausserhalb des Wertebereichs- auf 0 gesetzt!"<<std::endl;
    }
    if (!TrueColor(k2))
    {    
        k2 = 0;
        std::cout<<"Komponente2 ausserhalb des Wertebereichs- auf 0 gesetzt!"<<std::endl;
    }
    if (!TrueColor(k3))
    {    
        k3 = 0;
        std::cout<<"Komponente3 ausserhalb des Wertebereichs- auf 0 gesetzt!"<<std::endl;
    }
    color_.k1 = k1;
    color_.k2 = k2;
    color_.k3 = k3;
    CalcGray();
}

void Color::SetGray(int gray)
{
    //Wertebereich von gray testen
    if (!TrueColor(gray))
    {
        gray = 0;
        std::cout<<"Grauwert ausserhalb des Wertebereichs- auf 0 gesetzt!"<<std::endl;
    }
    gray_ = gray;
    CalcColor();
}

void Color::SetColorCode(COLORCODE ccode)
{
    color_code_ = ccode;
    if (ccode == NO)
        SetNO();
    else if (ccode == HEAT)
        SetHEAT();
    CalcColor();  
}

void Color::InvertColor()
{
    color_.k1 = 255 - color_.k1;
    color_.k2 = 255 - color_.k2;
    color_.k3 = 255 - color_.k3;
    CalcGray();
} 
