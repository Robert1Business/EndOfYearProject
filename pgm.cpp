#include "pgm.h"

pgm::pgm(void)
{
      picture = new matrixC(1,1);
      width = 1;
      height = 1;
      greyscale = 255;
      filename = "no_name";
};


pgm::pgm(int w, int h, int greys, std::string file)
{
       picture = new matrixC(w,h);
       width = w;
       height = h;
       greyscale = 255;
       filename = file;
};


pgm::pgm(std::string file)
{
       int w = 0,h = 0,greys = 0;
       
       std::ifstream f (file.c_str(), std::ios::in|std::ios::binary|std::ios::ate);
       f.seekg (0, std::ios::beg); //Erstes Zeichen suchen

       
       filename = file;
       int ascii = 1; //Asciiwert des eingelesenen Zeichens
       
       if (!f.good())
       {
             std::cout << "Datei nicht gefunden!"<< std::endl;
             exit(1);
       }      
       else 
       {
              //Einlesen der Headerinformationen
              char s,s2;
              f.get (s); f.get(s2);
              if (s != 'P'|| s2!= '5')
              {
                   std::cout << "Falsches Dateiformat- keine (unterstuetzte) PGM-Datei!"<< std::endl;
                   exit(1);
              }
              else
              {
                  //Höhe einlesen
                  f.get (s);
		  f.get (s);
		  if ( s == '#') //wenn in Header ein Kommentar auftritt, dann überspringen
		  {	
			while (s != 0x0A) f.get(s);
			f.get(s);
		  }	
		  ascii = s;
                  while (ascii != 32)
                  {
                        if (w == 0) w = ascii - 48;
                        else w = w * 10 + (ascii - 48);
                        f.get (s);
                        ascii = s;
                  }
                  
                  //Breite einlesen
                  f.get (s); 
		  ascii = s;
                  while (ascii != 10)
                  {
                        if (h == 0) h = ascii - 48;
                        else h = h * 10 + (ascii - 48);
                        f.get (s);
                        ascii = s;
                  }
                  
                  //Graustufen einlesen
                  f.get (s); 
		  if ( s == '#') //wenn in Header ein Kommentar auftritt, dann überspringen
		  {	
			while (s != 0x0A) f.get(s);
			f.get(s);
		  }	
		  ascii = s;
                  while (ascii != 10)
                  {
                        if (greys == 0) greys = ascii - 48;
                        else greys = greys * 10 + (ascii - 48);
                        f.get (s);
                        ascii = s;
                  }
                  
                  if (greys != 255)
                  {
                        std::cout << "Es werden nur Graustufen mit 256 Bit unterstützt!" << std::endl;
                        exit(1);
                  }
                  
                  //Bilddaten einlesen und in matrixC schreiben
                  picture = new matrixC(w,h);
                  for(int i1 = 1; i1 < h+1; i1++) 
                           for(int i2 = 1; i2 < w+1; i2++)
                           {
                                   f.get (s);
                                   //Da Ascii-Code nur ein 7 Bit-Code ist, muss zu allen negativen 
                                   //Asciiwerten (Werte > 127) 256 dazu addiert werden ( zur besseren
                                   //Handhabung)
                                   if ((int)s < 0)
                                       picture->set_value(i2,i1,256+(int)s);
                                   else
                                       picture->set_value(i2,i1,s);
                            }
                  
              } 
              f.close();
              width = w;
              height = h;
              greyscale = greys;
       }                
};


pgm::~pgm()
{
    delete(picture);
};


int pgm::getGrey(void)
{
    return(greyscale);
};


int pgm::getWidth(void)
{
    return(width);
};


int pgm::getHeigth(void)
{   
    return(height);
};


std::string pgm::getFilename(void)
{   
    return(filename);
};


int pgm::getValue(int w,int h)
{
    return(picture->get_value(w,h));
};


void pgm::setValue (int w,int h, int value)
{
     if (value >= 0 )
        picture->set_value(w,h,value);
     else
         std::cout << "Wert muss groesser 0 sein!" << std::endl;
};


void pgm::saveAs(std::string file)
{
     std::ofstream f (file.c_str(), std::ios::out|std::ios::binary);
     
     //Header schreiben für PGM
     char t1 = 0x0A; //Trennzeichen1
     char t2 = 0x20; //Trennzeichen2
     f<<"P5"<<t1<<width<<t2<<height<<t1<<greyscale<<t1<<std::flush;
     
     //Bilddaten schreiben
     for(int i1 = 1; i1 < height+1; i1++)
            for(int i2 = 1; i2 < width+1; i2++)
	    {
		f.put(picture->get_value(i2,i1) % 256);
	    }       
     f.close();
};


void pgm::Save(void)
{
     saveAs(filename);
};


pgm& pgm::operator=(const pgm &m)
{
    if(this==&m) return(*this);
    
    //Speicherfreigabe
    delete(picture);
    
    //neue BildmatrixC erzeugen
    picture = new matrixC(m.width,m.height);
    
    // Attribute kopieren
    height = m.height;
    width = m.width;
    greyscale = m.greyscale;
    filename = m.filename;
    
    // Kopieren der Bilddaten
    for (int i1 = 1;i1 < height+1; i1++)
        for (int i2 = 1;i2 < width+1; i2++)
           picture->set_value(i2,i1,m.picture->get_value(i2,i1));

    return(*this);
};


void pgm::Clear(void)
{
     for(int i1 = 1; i1 < height+1; i1++)
            for(int i2 = 1; i2 < width+1; i2++)
                    picture->set_value(i2,i1,0);           
};


void pgm::Invert(void)
{
     for(int i1 = 1; i1 < height+1; i1++)
            for(int i2 = 1; i2 < width+1; i2++)
                    picture->set_value(i2,i1,255-picture->get_value(i2,i1));           
};


void pgm::p_to_pLine (int x0, int y0, int x1, int y1, int greyscale)
{
      /*http://www.cs.unc.edu/~mcmillan/comp136/Lecture6/Lines.html*/  

     int dy = y1 - y0;
     int dx = x1 - x0;
     float t = (float) 0.5;                         // offset for rounding

     if (x0 > 0 && x0 <= width && y0 > 0 && y0 <= height)
        picture->set_value(x0,y0,greyscale);
     if (abs(dx) > abs(dy))                         // slope < 1
     {
            float m = (float) dy / (float) dx;      // compute slope
            t += y0;
            dx = (dx < 0) ? -1 : 1;
            m *= dx;
            while (x0 != x1)
            {
                x0 += dx;                           // step to next x value
                t += m;                             // add slope to y value
		        if (x0 > 0 && x0 <= width && (int)t > 0 && (int)t <= height)
		           picture->set_value(x0,(int)t,greyscale);
            }
     }
     else
     {                                              // slope >= 1
            float m = (float) dx / (float) dy;      // compute slope
            t += x0;
            dy = (dy < 0) ? -1 : 1;
            m *= dy;
            while (y0 != y1)
            {
                y0 += dy;                           // step to next y value
                t += m;                             // add slope to x value
                if ((int)t > 0 && (int)t <= width && y0 > 0 && y0 <= height)
		           picture->set_value((int)t,y0,greyscale);
            }
     }
};


void pgm::Line (int x0, int y0, int length, double angle, int greyscale)
{
     double x1 = length*cos((angle/180)*M_PI) + x0;
     double y1 = length*sin((angle/180)*M_PI) + y0;
          
     this->pgm::p_to_pLine (x0, y0, (int) x1, (int) y1, greyscale);
};


void pgm::angleLine (int x0, int y0, double angle, int greyscale)
{
    int length = 0;
    int x = x0;
    int y = y0;
    
    while (x > 0 && x <= width && y > 0 && y <= height)
    {
        picture->set_value(x,y,greyscale);
        x = int(round(length*cos(((double)angle/180)*M_PI))) + x0;
        y = int(round(length*sin(((double)angle/180)*M_PI))) + y0;
        length++;
    }
};


void pgm::Cross (int x0, int y0, int length, int greyscale)
{
     for (int i = 1; i < 5; i++)
         this->pgm::Line (x0, y0, length,90*i , greyscale);
};


void pgm::fCircle (int x0, int y0, int radius, int greyscale)
{
     for(int i1 = 1; i1 < height+1; i1++)
            for(int i2 = 1; i2 < width+1; i2++)
                    if ((i2-x0)*(i2-x0)+(i1-y0)*(i1-y0)< radius*radius)
                       picture->set_value(i2,i1,greyscale);
};


int pgm::minColorAreaX (int greyscale)
{
    bool found = false; //Grauwert gefunden
    int h = 1,w = 1; //Laufvariablen für while-Schleife
    
    while (!found && w != width + 1)
    {
          for (h = 1;h < height + 1; h++)
              if (picture->get_value(w,h) == greyscale) found = true;
          w++;
    }
    
    if(!found)
          return(0);
    else
          return(w-1);
};


int pgm::maxColorAreaX (int greyscale)
{
    bool found = false; //Grauwert gefunden
    int h = 1,w = width; //Laufvariablen für while-Schleife
    
    while (!found && w != 0)
    {
          for (h = 1;h < height + 1; h++)
              if (picture->get_value(w,h) == greyscale) found = true;
          w--;
    }
    
    if(!found)
          return(0);
    else
          return(w+1);
};


int pgm::minColorAreaY (int greyscale)
{    
    bool found = false; //Grauwert gefunden
    int h = 1,w = 1; //Laufvariablen für while-Schleife
    
    while (!found && h != height + 1)
    {
          for (w = 1;w < width + 1; w++)
              if (picture->get_value(w,h) == greyscale) found = true;
          h++;
    }
    
    if(!found)
          return(0);
    else
          return(h-1);
};


int pgm::maxColorAreaY (int greyscale)
{
    bool found = false; //Grauwert gefunden
    int h = height,w = 1; //Laufvariablen für while-Schleife
    
    while (!found && h != 0)
    {
          for (w = 1;w < width + 1; w++)
              if (picture->get_value(w,h) == greyscale) found = true;
          h--;
    }
    
    if(!found)
          return(0);
    else
          return(h+1);
};


int pgm::nextColorPoint(int &x0, int &y0, double angle, int greyscale)
{
    bool found = false; //Grauwert gefunden
    double x1 = x0, y1 = y0;
    double length = 0; //Laufvariable
       
    while (!found && (int)x1 > 0 && (int)y1 > 0 && (int)x1 <= width && (int)y1 <= height)
    {
          if (picture->get_value((int)x1,(int)y1) == greyscale)
          {
             found = true;
             x0 = (int)x1;
             y0 = (int)y1;
          }
          x1 = length*cos((angle/180)*M_PI) + x0;
          y1 = length*sin((angle/180)*M_PI) + y0;
          length++;
    }
       
    if (!found)
    {
          x0 = 0; y0 = 0;
    }
    return(int(length-1));
};


int pgm::nextPoint(int &x0, int &y0, double angle)
{
    bool found = false; //Grauwert gefunden
    int x1 = x0;
    int y1 = y0;
    int length = 0; //Laufvariable
    int a = (int)angle;
    
    int greyscale = picture->get_value(x0,y0);
       
    while (!found && x1 > 0 && y1 > 0 && x1 <= width && y1 <= height)
    {
          if (picture->get_value((int)x1,(int)y1) != greyscale)
          {
             found = true;
             x0 = x1;
             y0 = y1;
          }
          x1 = int(round(length*cos(((double)a/180)*M_PI))) + x0;
          y1 = int(round(length*sin(((double)a/180)*M_PI))) + y0;
          length++;
    }
       
    if (!found)
    {
          x0 = 0; y0 = 0;
    }
    return(length-1);
};


int pgm::maxColor (void)
{
    int found = 0;
    for (int i = 1; i < width + 1; i++)
        for (int j = 1; j < height + 1; j++)
            if (picture->get_value(i,j) > found) found = picture->get_value(i,j);
    return(found);
};


int pgm::colorValue (int greyscale)
{
	int found = 0;
	for (int i = 1; i < width + 1; i++)
		for (int j = 1; j < height + 1; j++)
			if (picture->get_value(i,j) == greyscale) found++;
	
	return(found);
};


void pgm::boundaryFill(int x, int y, int fill, int greyscale)
{
	//http://www.cs.unc.edu/~mcmillan/comp136/Lecture8/areaFills.html
        if (x <= 0 || x >= width + 1 || y <= 0 || y >= height + 1) return;
        if (picture->get_value(x,y) == greyscale && fill != greyscale)
	{
		picture->set_value(x, y, fill);
		boundaryFill(x+1, y, fill, greyscale);
		boundaryFill(x, y+1, fill, greyscale);
		boundaryFill(x-1, y, fill, greyscale);
		boundaryFill(x, y-1, fill, greyscale);
	}
};


void fillOtherRec(pgm &m, pgm &n, int x, int y, int fill, int greyscale)
//Rekursiver Füll-Algorithmus; angestartet duch fillOther
{
	//http://www.cs.unc.edu/~mcmillan/comp136/Lecture8/areaFills.html
	//eigentlicher Füllalgorithmus
        if (x <= 0 || x >= m.getWidth() + 1 || y <= 0 || y >= m.getHeigth() + 1) return;
        if (m.getValue(x,y) == greyscale && n.getValue(x,y) != fill)
	{
		n.setValue(x, y, fill);
		fillOtherRec(m,n,x+1, y, fill, greyscale);
		fillOtherRec(m,n,x, y+1, fill, greyscale);
		fillOtherRec(m,n,x-1, y, fill, greyscale);
		fillOtherRec(m,n,x, y-1, fill, greyscale);
	}
};


pgm fillOther(pgm &m, int x, int y, int fill)
{
	//neue PGM anlegen mit gleichen Ausmaßen
	pgm n;
	n = m;
	for(int i1 = 1; i1 < m.getHeigth()+1; i1++)
            for(int i2 = 1; i2 < m.getWidth()+1; i2++)
                    n.setValue(i2,i1,255);	
	
	//starten der Rekursion
	fillOtherRec(m,n,x,y,fill,m.getValue(x,y));
	
	return n;
};


int** pgm::copyObjektToArray (void)
{
	//matrixC allokieren
	int ** matrixC;
	matrixC = (int **)calloc(height, sizeof(int*));
	int i,j;
	for (i = 0; i < height; i++) 
		matrixC[i] = (int *)calloc(width, sizeof(int));
	for (i = 0; i < height; i++) 
	    for (j = 0; j < width; j++)
		matrixC [i][j] = picture->get_value(j+1,i+1);
	
	return(matrixC);
};


void pgm::copyArrayToObjekt (int** mat, int h, int w)
{
	delete(picture);
	picture = new matrixC(w,h);
    height = h;
	width = w;
	
	for (int i = 0; i < h; i++) 
		for (int j = 0; j < w; j++)
			picture->set_value(j+1,i+1,mat[i][j]);
};


int* pgm::frequencyDistribution(void)
{
	int *frequency; //Zeiger auf matrixC mit Häufigkeitsverteilung der Farbwerte
	int maxcolor = maxColor(); //höchste mögliche Farbe
	frequency = (int *)calloc(maxcolor+1, sizeof(int));
	
	//matrixC löschen
	for (int i = 0; i <= maxcolor; i++) frequency[i] = 0;
	
	for (int i = 0; i <= maxcolor; i++)
		frequency[i] = this->colorValue (i); //Anzahl der Pixel pro Farbe
	
	return(frequency);
};


void pgm::Quantize (int levels)
{
	for (int i = 1; i <= width; i++)
		for (int j = 1; j <= height; j++)
		{
			int value = 255*(int)(picture->get_value(i,j)*(levels-1)/255)/(levels-1);
			picture->set_value(i,j, value);
		}
};

void pgm::setWidth(int w) {width = w;}

void pgm::setHeigth (int h){height = h;}


void pgm::scaleNearNeigh(double scaleFactorX, double scaleFactorY)
{
	if (scaleFactorX <= 0)
	{
		std::cout<<"Fehler: Skalierung mit Faktor "<< scaleFactorX <<" nicht moeglich!"<<std::endl;
		return;
	}
	else if (scaleFactorY <= 0)
	{
		std::cout<<"Fehler: Skalierung mit Faktor "<< scaleFactorY <<" nicht moeglich!"<<std::endl;
		return;
	}
	
	int newWidth = int(scaleFactorX * width);
	int newHeight = int(scaleFactorY * height);
	
	//neues Bild allokieren
	int ** matrixC;
	matrixC = (int **)calloc(newHeight, sizeof(int*));
	int i,j;
	for (i = 0; i < newHeight; i++) 
		matrixC[i] = (int *)calloc(newWidth, sizeof(int));
	
	for (i = 0; i < newHeight; i++)
	    for (j = 0; j < newWidth; j++)
		    matrixC [i][j] = picture->get_value(int(j/scaleFactorX+1),
							int(i/scaleFactorY+1));
	
	copyArrayToObjekt(matrixC, newHeight, newWidth);

    //Speicher freigeben
    for (i = 0; i < newHeight; i++) 
        delete (matrixC[i]);
}

void pgm::mirrorX(void)
{
    int y;
    
    //neues Bild allokieren
	int ** matrixC;
	matrixC = (int **)calloc(height, sizeof(int*));
	int i,j;
	for (i = 0; i < height; i++) 
		matrixC[i] = (int *)calloc(width, sizeof(int));
	
    //Pixel kopieren
    for (j = 0; j < width; j++)
    {
        y = height;
        for (i = 0; i < height; i++)
        {
		    matrixC [i][j] = picture->get_value(j+1,y);    
            y--;
        }
    }
	
	copyArrayToObjekt(matrixC, height, width);

    //Speicher freigeben
    for (i = 0; i < height; i++) 
        delete (matrixC[i]);
}



//matrixCklasse
matrixC::matrixC(int nrow, int ncolumn) 
{
    // Initialisierung mit Gültigkeitsprüfung
    numrows=nrow;
    numcolumns=ncolumn;
    if ((numrows>0) && (numcolumns>0))
    {
        all_elements = new int[numrows*numcolumns];
        
        for (int i=0;i<numrows*numcolumns;i++)
        {
            all_elements[i]=0;
        };
    }
    else
    {
        // Gewaltsamer Programmabbruch bei Verletzung des
        // zulässigen Wertebereichs der Indizes
        std::cout << "Es wurde versucht, einen negativen Wert für "
             << "mindestens eine matrixC-Dimension anzugeben" << std::endl;
        exit(1);
    };
};


matrixC::~matrixC()
{
    // Speicherfreigabe
    delete[](all_elements);
};


bool matrixC::operator==(const matrixC &m) const
{
    // Überprüfe, ob Matrizen von gleicher Größe sind
    if((this->numrows!=m.numcolumns) || (this->numcolumns!=m.numcolumns))
    {
        std::cout << "Matrizen haben unterschiedliche Groeße! ";
        std::cout << "Kein sinnvoller Vergleich möglich" << std::endl;
        return false;
    }
    
    // Überprüfe Gleichheit für jedes Element
    for(int i=0; i < numrows*numcolumns; i++)
    {
        if( this->all_elements[i] != m.all_elements[i] ){return false;};
    };
    
    // Falls alle matrixCeinträge identisch sind
    return true;
};


matrixC& matrixC::operator=(const matrixC &m)
{
    // Prüfung auf Selbstzuweisung
    if(this==&m) return(*this);
    
    // Falls keine Selbstzuweisung...
    // Dimensionsüberprüfung
    if (numrows == m.numrows && numcolumns == m.numcolumns)
    {
        for (int i=0;i<m.numrows*m.numcolumns;i++)
        {
            all_elements[i]=m.all_elements[i];
        };
        // Rückgabe des dereferenzierten Zeigers auf die aufrufende Klasse
        return(*this);
    };
    
    // Sonst: Speicherfreigabe & Neuanforderung von Speicher
    if(all_elements)
    {
        delete[](all_elements);
    };
    
    all_elements = new int[m.numrows*m.numcolumns];
    
    // Setzen der Attribute
    numrows=m.numrows;
    numcolumns=m.numcolumns;
    
    // Kopieren der Elemente mit Schleife
    for (int i=0;i<m.numrows*m.numcolumns;i++)
    {
        all_elements[i]=m.all_elements[i];
    }

    // Rückgabe des dereferenzierten Zeigers auf die aufrufende Klasse
    return(*this);
};


int& matrixC::operator() (int row, int column) const
{
    if ((column > 0) && (column <= numcolumns) && (row > 0) && (row <= numrows))
    {
	int index = (row-1)*numcolumns+(column-1);
	return all_elements[index];
    }
    else
    {
	std::cout << "Mindestens ein Index liegt außerhalb des gueltigen Bereichs" << std::endl;
	exit(1);
    }
};


void matrixC::set_value(int row, int col, int wert)
{
    // Gültigkeitsprüfung für Indizes row, col
    if((row<1 || row>numrows) || (col<1 || col>numcolumns))
    {
        std::cout << "Ungueltiger Index\n";
        std::cout << "Keine Wertzuweisung erfolgt!!!" << std::endl;
        std::cout << row << " " << col << std::endl;
    }
    else    
    {
        all_elements[(row-1)*numcolumns + col - 1] = wert;
    }
};
    
    
int matrixC::get_value(int row, int col)
{
    // Gültigkeitsprüfung der Argumente
    // Gültigkeitsprüfung für Indizes row, col
    if((row<1 || row>numrows) || (col<1 || col>numcolumns))
    {
        std::cout << "Ungueltiger Index\n";
        std::cout << "Gewaltsamer Programmabbruch!!!" << std::endl;
        exit(1);
    }
    else    // ...gültige Indizes
    {
        return all_elements[(row-1)*numcolumns + col - 1];
    }
};


matrixC transpose(const matrixC &m)
{
    matrixC rm(m.numcolumns, m.numrows);
    
    for(int i=1; i<=m.numrows; i++)
    {
        for(int j=1; j<=m.numcolumns; j++)
        {
            rm(j,i) = m(i,j);
        }
    };
    
    return(rm);
};


int matrixC::get_numrows()const{return numrows;};
int matrixC::get_numcolumns()const{return numcolumns;};


std::ostream& operator<<(std::ostream& s, const matrixC &m)
{
    for (int i=1;i<=m.numrows;i++)
    {
        s << std::right << std::fixed;
        for (int j=1;j<=m.numcolumns;j++)
        {
            s << std::setw(10) << m.all_elements[(i-1)*m.numcolumns + j - 1];
        }
        s << std::endl;
    }
    return s;
};


matrixC operator*(const matrixC &m1, const matrixC &m2)
{
    // Berechnete Indizes der Matrizen für Zugriff über all_elements[index]
    int index_rm;
    int index_m1;
    int index_m2;
    
    //Prüfe Zulässigkeit der Operation
    if(m1.numcolumns!=m2.numrows)
    {
        std::cout<<"Spaltenzahl von matrixC1 stimmt nicht mit Zeilenzahl von matrixC2 überein." << std::endl;
        std::cout<<"Somit kann die angeforderte Matrizenmultiplikation nicht durchgefuehrt werden."<<std::endl;
        exit(1);
    }
    
    //Definition der RückgabematrixC und Initialisierung auf Null (siehe Konstruktor)
    matrixC rm(m1.numrows,m2.numcolumns);
    
    
    int rm_numcolumns =rm.numcolumns;
    int m1_numcolumns =m1.numcolumns;
    int m2_numcolumns =m2.numcolumns;
    
    for(int i=1 ; i<= m1.numrows; i++)
    {
        for(int j=1 ; j<=m2.numcolumns; j++)
        {
            for(int k=1; k<=m1.numcolumns; k++)
            {
                // Indizes der Matrizen
                index_rm = (i-1)*rm_numcolumns+(j-1);
                index_m1 = (i-1)*m1_numcolumns+(k-1);
                index_m2 = (k-1)*m2_numcolumns+(j-1);
                
                // Wichtig: all_elements wurde mit Null initialisiert!!!
                rm.all_elements[index_rm] += m1.all_elements[index_m1]*m2.all_elements[index_m2];
            }
        }
    };
    
    return(rm);
};


matrixC operator*(const matrixC &m, int factor)
{    
    //Definition der RückgabematrixC und Initialisierung auf Null (siehe Konstruktor)
    matrixC rm(m.numrows,m.numcolumns);
    
    for(int i=1 ; i<= m.numrows; i++)
    {
        for(int j=1 ; j<=m.numcolumns; j++)
        {
            rm(i,j) = m(i,j)*factor;
        }
    };
    
    return(rm);
};


matrixC operator*(int factor, const matrixC &m)
{    
    //Definition der RückgabematrixC und Initialisierung auf Null (siehe Konstruktor)
    matrixC rm(m.numrows,m.numcolumns);
    
    for(int i=1 ; i<= m.numrows; i++)
    {
        for(int j=1 ; j<=m.numcolumns; j++)
        {
            rm(i,j) = m(i,j)*factor;
        }
    };
    
    return(rm);
};
