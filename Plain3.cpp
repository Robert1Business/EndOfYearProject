#include "Plain3.h"

bool Plain3::GetSign(Vector3 &p)
{
    Vector3 c = p - p1;
    double spat = a.MixedProduct(b, c);

    if (spat >= 0)
        return true;
    else
        return false;
}


Plain3::Plain3(Vector3 &p1_, Vector3 &p2_, Vector3 &p3_, bool allPoints)
{
    p1 = p1_;
    p2 = p2_;
    p3 = p3_;

    if (allPoints)
    {
        //Vektoren a, b berechnen, die Ebene Aufspannen
        a = p3 - p1;
        b = p2 - p1;
    }
    else
    {
        a = p2_;
        b = p3_;
    }
}

