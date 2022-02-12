#include "Field.H"


template<class T> Field1D<T>::Field1D()
{
    sizeX = 0;
}


template<class T> Field1D<T>::Field1D(int sizeXin)
{
    this->resize(sizeXin);
    sizeX = sizeXin;
}


template<class T> void Field1D<T>::initialize(int sizeXin)
{
    this->resize(sizeXin);
    sizeX = sizeXin;
}




template<class T> Field<T>::Field( )                    //empty Field-constructor
{
    sizeX = -1;
    sizeY = -1;
}


template<class T> Field<T>::Field(Lattice &inLattice)   //Field-constructor
{
    theLattice = &inLattice;
    this->resize(theLattice->Lx());
    for (int i=0;i<theLattice->Lx();i++)
	(*this)[i].initialize(theLattice->Ly());
    sizeX = theLattice->Lx();
    sizeY = theLattice->Ly();
}

//Field-constructor with initialization
template<class T> Field<T>::Field(Lattice &inLattice, const T init)   
{
    theLattice = &inLattice;
    this->resize(theLattice->Lx());
    for (int i=0;i<theLattice->Lx();i++)
	(*this)[i].initialize(theLattice->Ly());
    sizeX = theLattice->Lx();
    sizeY = theLattice->Ly();
    for(int i=0;i<sizeX;i++)
      for(int j=0;j<sizeY;j++)                             
	(*this)[i][j] = init;
}


template<class T> Field<T>::Field(const Field& that)    //copy-constructor
{
    (*this).sizeX = that.sizeX;
    (*this).sizeY = that.sizeY;
    (*this).resize(that.sizeX);
    (*this).theLattice = that.theLattice;

    for(int i=0;i<sizeX;i++)
    {
	(*this)[i].resize(sizeY);
	for(int j=0;j<sizeY;j++)                             
	    (*this)[i][j] = that[i][j];
    }

//     std::cout<<"  "<<std::endl;
//     std::cout << "Copy finished. New field: "<<theLattice->Lx()<<"  "<<theLattice->Ly()<<
// 	"  "<<theLattice->dx()<<"  "<<theLattice->dy()<< std::endl;

    return;
}


template<class T> Field<T>& Field<T>::operator=(const Field& that)
{
    (*this).sizeX = that.sizeX;
    (*this).sizeY = that.sizeY;
    (*this).resize(that.sizeX);
    (*this).theLattice = that.theLattice;

    //std::cout << "Use operator=" << std::endl;
    for(int i=0;i<sizeX;i++)
    {
	(*this)[i].resize(sizeY);
	for(int j=0;j<sizeY;j++)                             
	    (*this)[i][j] = that[i][j];
    }

    return (*this);
}


template<class T> Field<T> Field<T>::operator+(Field& addField)
{
    Field<T> resultField(*theLattice);

//     if((*this).theLattice==addField.theLattice)       
    if(  ((*this).theLattice->Lx()==addField.theLattice->Lx()) &&
	 ((*this).theLattice->Ly()==addField.theLattice->Ly()) &&
	 ((*this).theLattice->dx()==addField.theLattice->dx()) &&
	 ((*this).theLattice->dy()==addField.theLattice->dy()))
    {                                                 
	std::cout<<"Summation of fields successful."<<std::endl;

	for(int m=0;m<sizeX;m++)
	    for(int n=0;n<sizeY;n++)
		resultField[m][n] = (*this)[m][n] + addField[m][n];

	return resultField;
    }
    else
    {
	std::cout<<"Summation of fields not possible!!"<<std::endl;
	return resultField;
    }
}

template<class T> Field<T> Field<T>::operator-(Field& subField)
{
    Field<T> resultField(*theLattice);

    if(  ((*this).theLattice->Lx()==subField.theLattice->Lx()) &&
	 ((*this).theLattice->Ly()==subField.theLattice->Ly()) &&
	 ((*this).theLattice->dx()==subField.theLattice->dx()) &&
	 ((*this).theLattice->dy()==subField.theLattice->dy()))
    {                                           
	for(int m=0;m<sizeX;m++)
	    for(int n=0;n<sizeY;n++)
		resultField[m][n] = (*this)[m][n] - subField[m][n];
	return resultField;
    }
    else
    {
	std::cout<<"Summation of fields not possible!!"<<std::endl;
	return resultField;
    }
}


template<class T> void Field<T>::write()
{
    std::cout << "Field on Lattice " << theLattice->Lx() << "x" << theLattice->Ly() << std::endl;
    for(int j=0; j<sizeY; j++)
    {
	for(int i=0;i<sizeX;i++)
	    std::cout <<(*this)[i][j]; //ohne Leerezeichen?
//	    std::cout << " " << (*this)[i][j] << " ";
       	    std::cout << std::endl;
            //Hier schreibt er 3x4-Feld ohne Angabe der (i,j)-Position
            //x=i horizontal, y=j vertikal
    }
}


template<class T> void Field<T>::writetofile()
{
    ofstream ausgabe("gnuplotdata.txt");
    for(int i=0;i<theLattice->Lx();i++)
    {
	ausgabe << std::endl;     //statt std::cout schreibt man ausgabe, um Zahlen in gnutest.txt einzulesen

	for(int j=0;j<theLattice->Ly();j++)
	{
	    //if((*this)[i][j]!=0) ausgabe <<i<<" "<<j<<" " <<(*this)[i][j]<< std::endl;
	    ausgabe <<i<<" "<<j<<" " <<(*this)[i][j]<< std::endl;
	}
    }
    return;
}

// Readfromfile requires that the Lattice is still the same.
template<class T> void Field<T>::readfromfile()
{
    ifstream eingabe("gnuplotdata.txt");
    string dummy;
    
    for(int i=0;i<theLattice->Lx();i++)
    {
      eingabe >> dummy;     //statt std::cout schreibt man eingabe, um Zahlen in gnutest.txt einzulesen
      int dumi, dumj;
      for(int j=0;j<theLattice->Ly();j++)
	{
	  //if((*this)[i][j]!=0) eingabe <<i<<" "<<j<<" " <<(*this)[i][j]<< std::endl;
	  T readme;
	  eingabe >> dumi  >> dumj >> readme;
	  (*this)[i][j]= readme;
	}
    }
    return;
}


template<class T>  const int Field<T>::getX() const
{
    return sizeX;
}

template<class T>  const int Field<T>::getY() const
{
    return sizeY;
}


//Problem: wenn der Operator [] überladen wird, vergisst er den "normalen" [] für int o.Ä.
//das heißt er kann dann nicht mehr that[i] wie z.B. in opertor+ gewollt
//deshalb wird der Operator einfach anders betitelt, nämlich mit runden Klammern ()
template<class T> T & Field<T>::operator()(const Point& p)
{
    return (*this)[p.x][p.y];
}

//und hier nochmal für zwei int, dann kann man auch F1(3,5)=77; schreiben
template<class T> T & Field<T>::operator()(int x, int y)
{
    return (*this)[x][y];
}


template<class T> Field<T> Field<T>::Cut(Point p, Lattice theLattice)
{
    Field<T> newField (theLattice);
    for(int x = 0; x < newField.getX(); x++)
        for (int y = 0; y < newField.getY(); y++)
        {
            int xdirection = (x + p.x - (int)(newField.getX()/2)) % (*this).getX();
            int ydirection = (y + p.y - (int)(newField.getY()/2)) % (*this).getY();
            if (xdirection < 0 ) xdirection = (*this).getX() + xdirection;
            if (ydirection < 0 ) ydirection = (*this).getY() + ydirection;
            //std::cout<<"x "<<xdirection <<std::endl;
           // std::cout<<"y "<<ydirection <<std::endl;
            newField(x,y)= (*this)[xdirection][ydirection];
        }
        
    return newField;
}


// Weight function for averaging (see below)
real weight(double step, int range)
{
  return 1./exp( pow(step,2) / pow(real(range),2));
}



template<class T> void Field<T>::MakeGlobal(Point p, Field<T> &field)
{
    for(int x = 0; x < field.getX(); x++)
        for (int y = 0; y < field.getY(); y++)
        {
	  // MB: Changed /2 to /2. to make sure that doubles are used
            int xdirection = (x + p.x - (int)round(double(field.getX())/2.) + (*this).getX()) % (*this).getX();
            int ydirection = (y + p.y - (int)round(double(field.getY())/2.) + (*this).getY()) % (*this).getY();
            (*this)[xdirection][ydirection] = field(x,y);
        }
}

template<class T> void Field<T>::Deposit(Point p, Field<T> &field)
{
    for(int x = 0; x < field.getX(); x++)
        for (int y = 0; y < field.getY(); y++)
        {
	  // MB: Changed /2 to /2. to make sure that doubles are used
            int xdirection = (x + p.x - (int)round(double(field.getX())/2.) + (*this).getX()) % (*this).getX();
            int ydirection = (y + p.y - (int)round(double(field.getY())/2.) + (*this).getY()) % (*this).getY();
            (*this)[xdirection][ydirection] = (*this)[xdirection][ydirection] + field(x,y);
        }
}


//----------------------------------------------------------------------
// Ab hier keine Templat-Routinen einfuegen!!!!!
// Hinweis Breymann, S.263,264
// Erzeuge Instanzen fuer alle tatsaechlich benutzten Templates.
// D.h., fuer jedes template Field<XYZ> muss hier stehen
// template class Field<XYZ>;
// !! Muss am Ende stehen, aber instanzierte Funktionen dürfen danach
// noch kommen

template class Field<real>;

template class Field<int>;

template class Field<FluidState>;

template class Field<NeighbourState>;

template class Field<CoatingQueue>;


// The next routine calculates the average of the field for smoothing. The range
// gives +/- lattice steps, i.e. a range of 2 gives a 5-point stencil
// etc. 
// The points are weighted using the weight function given above, at
// the moment a Gaussian as in 2D
Field<real> averageField(Field<real> field, int range)
{
  Field<real> average(field);
  if (range<1) 
    {
      warning("Cannot average a Field with range %i. Field unchanged", range);
      return average;
    }
  
  int xmax=average.getX();
  int ymax=average.getY();
    
  for(int x = 0; x < xmax; x++)
    for (int y = 0; y < ymax; y++)
      {
	real hMean=0;
	real totalWeight=0;
	// Now we are at point (x,y)
	for (int xs=-range; xs<=+range; xs++)
	  for (int ys=-range; ys<=+range; ys++)
	    {
	      // Sweep over neighbours with Dirichlet cut-off
	      double dist = sqrt( double(xs*xs+ys*ys));
	      int xtot=x+xs;
	      int ytot=y+ys;
	      
	      if ( (xtot<0) || (xtot>=xmax) || (ytot<0) || (ytot>ymax)) continue;
	      double thisWeight=weight(dist, range);
	      hMean += field(xtot,ytot) * thisWeight;
	      totalWeight+=thisWeight;
// 	      if (field(xtot,ytot) >1.e-10)
// 		std::cout << OUT(x) << OUT(xs) << OUT(y) << OUT(ys)
// 			  << OUT(dist) << OUT(thisWeight) << OUT(hMean)
// 			  << OUT(totalWeight) << std::endl;
	    }
	average(x,y) = hMean/totalWeight;
	// THis is needed mainly for drawing purposes - the
	// 3D-Field-drawing routine may think that a value is non-zero
	// if it is exceedingly small and fill a cell
	if ((average(x,y) < 1.e-30) && (average(x,y) > 0.))
	  average(x,y) = 0.;
      }
  
    return average;
    
}

// Ra is defined as  1/(M*N) Sum_p abs(z(p) - <z> )
double mean(Field<real> field)
{
  // determine mean value
  double meany=0;
  int xmax=field.getX();
  int ymax=field.getY();
  for(int j=0; j<ymax; j++)
    for(int i=0;i<xmax;i++)
      meany +=field[i][j]; 
  meany /= (ymax*xmax);
  return meany;
}



// Ra is defined as  1/(M*N) Sum_p abs(z(p) - <z> )
double roughnessRa(Field<real> field)
{
  // determine mean value
  int xmax=field.getX();
  int ymax=field.getY();
  double meany=mean(field);
  double sum=0;
  for(int j=0; j<ymax; j++)
    for(int i=0;i<xmax;i++)
      sum+= fabs(field[i][j] -meany); 
  sum /= (ymax*xmax);
  return sum;
}


// Ra is defined as  1/(M*N) Sum_p abs(z(p) - <z> )
double roughnessMinMax(Field<real> field)
{
  double max=-1e30;
  double min= 1e30;
  int xmax=field.getX();
  int ymax=field.getY();
  for(int j=0; j<ymax; j++)
    for(int i=0;i<xmax;i++)
      {
	if (field[i][j]  > max) max = field[i][j]; 
	if (field[i][j]  < min) min = field[i][j]; 
      }
  return max-min;
}

