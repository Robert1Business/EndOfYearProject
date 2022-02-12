#include "Field3D.H"

template <class T> Field3D<T>::Field3D(unsigned x, unsigned y, unsigned z, T defval,  int numObjects): x_(x), y_(y), z_(z), defaultValue(defval), numObjects_(numObjects)
{
    StartIteration();
    depositedParticles_ = defval;
    //Speicher allokieren
    elems = new T[x_*y_*z_];
    Reset();
}

template <class T> Field3D<T>::~Field3D()
{
    delete [] elems;
}

template <class T> void Field3D<T>::Reset()
{
    for(int i = 0; i < x_*y_*z_; i++)
        elems[i] = defaultValue;  
}

template <class T> int Field3D<T>::getXDim()
{return(x_);}

template <class T> int Field3D<T>::getYDim()
{return(y_);}

template <class T> int Field3D<T>::getZDim()
{return(z_);}

template <class T> void Field3D<T>::StartIteration()
{
    iterend_ = false;
    iterx_ = 0;
    itery_ = 0;
    iterz_ = 0;
}

template <class T> bool Field3D<T>::EndIteration()
{
    if(iterend_)
        return true;
    else
        return false;
}

template<class T> Field3D<T>& Field3D<T>::operator++(int)
{
    iterx_++;
    if (iterx_ == x_) 
    {
        iterx_ = 0;
        itery_++;
        if (itery_ == y_) 
        {
            itery_ = 0;
            iterz_++;
            if (iterz_ == z_)
            {
                iterz_--;
                iterend_ = true;
                iterx_ = x_ - 1;
                itery_ = y_ - 1;
            }
        }
    }
    return *this;
}

template <class T> bool Field3D<T>::indexOk(unsigned x, unsigned y, unsigned z )
{
    return (x_ > x) && (y_ > y) && (z_ > z);
}

template <class T> inline int Field3D<T>::makeIndex(unsigned x, unsigned y, unsigned z )
{
    return x + x_*y + y_*x_*z;
}

template <class T> T& Field3D<T>::at(unsigned x, unsigned y, unsigned z)
{
    if( indexOk(x,y,z))
        return elems[makeIndex(x,y,z)];
    else
    {
        std::cout<<"Wrong Index-Parameters "<<x<<" "<<y<<" "<<z<<std::endl;
        exit(1);
    }
}

template <class T> T& Field3D<T>::at(void)
{
    return elems[makeIndex(iterx_,itery_,iterz_)];
}

template <class T> bool Field3D<T>::set(unsigned x, unsigned y, unsigned z, T value)
{
    if( indexOk(x,y,z))
    {
        elems[makeIndex(x,y,z)]=value;
        //if (value > depositedParticles_) depositedParticles_ = value;
        return true;
    }
    else
    {
        std::cout<<"Wrong Index-Parameters "<<x<<" "<<y<<" "<<z<<std::endl;
        return false;
    }
}

template <class T> void Field3D<T>::set(T value)
{
    elems[makeIndex(iterx_,itery_,iterz_)] = value;
}

template <class T> void Field3D<T>::Print()
{
    for (int z = 0; z < z_; z++)
    {
        std::cout<<"z= "<<z<<":"<<std::endl;
        for (int y = 0; y < y_; y++)
        {
            for (int x = 0; x < x_; x++)
                std::cout<<elems[makeIndex(x,y,z)]<<" ";
            std::cout<<std::endl;
        }
    
        std::cout<<std::endl;
    }
}

template <class T> pgm Field3D<T>::XYLayer(unsigned z)
{
    pgm slice(x_,y_, 255, "xyslice.pgm");

    for (int y = 0; y < y_; y++)
        for (int x = 0; x < x_; x++)
            slice.setValue(x+1,y+1,elems[makeIndex(x,y,z)]);

    return slice;
}

template <class T> pgm Field3D<T>::XZLayer(unsigned y)
{
    pgm slice(x_,z_, 255, "xyslice.pgm");
    int z_volume = z_;

    for (int z = 0; z < z_; z++)
    {
        //z Koordinate muss gespiegelt werden damit am unteren Bildrand der PGM
        //auch der Untergrund ist
        z_volume--;
        for (int x = 0; x < x_; x++)
        {
            slice.setValue(x+1,z+1,elems[makeIndex(x,y,z_volume)]);
        }
    }

    return slice;
}


template <class T> pgm Field3D<T>::YZLayer(unsigned x)
{
    pgm slice(y_,z_, 255, "xyslice.pgm");
    int z_volume = z_;
    
    for (int z = 0; z < z_; z++)
    {
        //z Koordinate muss gespiegelt werden damit am unteren Bildrand der PGM
        //auch der Untergrund ist
        z_volume--;
        for (int y = 0; y < y_; y++)
        {
            
            slice.setValue(y+1,z+1,
			   int(double(elems[makeIndex(x,y,z_volume)])/ (double)numObjects_ * 255. ));
        }
    }

    return slice;
}

template <class T> T& Field3D<T>::Max(void)
{return depositedParticles_;}

// DepositNew gets *globalised* fields!!!
template <class T> void Field3D<T>::DepositNew(Field<real> &field, Field<real> &Ground, double dz)
{
  // note that 1 is the particle number of the Ground
    depositedParticles_ ++;
    int value = int(depositedParticles_);// No modulo anymore  % 255+1;    
    cout<<OUT(value)<<" "<<OUT(depositedParticles_)<<endl;
  
    for(int x = 0; x < field.getX(); x++)
        for (int y = 0; y < field.getY(); y++)
        {
	  // Directly calculate the z-coordinates that get filled with
	  // this splat on the current Ground height
	  int zMin=int(Ground[x][y]/dz);
	  int zMax=int((Ground[x][y] +field[x][y])/dz);
	  if (zMax >= getZDim()) 
	    error("WARNING! zMax is %i at coordinates %i %i\n",
		    zMax, x,y);
	  // This is tricky because if the field is very small, zMin
	  // may be equal to zMax. So only deposit if the field is
	  // large enough.
	  //  ?? This should be thought about again because it can
	  //  lead to some empty cells unless the comparison is very
	  //  small ??
	  if ( field[x][y] > 0.00005 * dz)
	    for (int zdirection = zMin; zdirection<=zMax;zdirection++)
	      (*this).set(x,y,zdirection,value);
	  else if ( field[x][y] > 0.000000005 * dz)
	    std::cout << "Small field value at " << OUT(x) << OUT(y) << OUT(field[x][y]) << std::endl;
        }
}


template <class T> void Field3D<T>::Deposit(Point p,Field<real> &field, double dz)
{
    depositedParticles_ ++;
    int value = int(depositedParticles_); // No modulo anymore  % 255+1;    
    cout<<OUT(value)<<" "<<OUT(depositedParticles_)<<endl;
  
    for(int x = 0; x < field.getX(); x++)
        for (int y = 0; y < field.getY(); y++)
        {
            int xdirection = (x + p.x - (int)round(field.getX()/2) + (*this).getXDim()) % (*this).getXDim();
            int ydirection = (y + p.y - (int)round(field.getY()/2) + (*this).getYDim()) % (*this).getYDim();
            int zdirection = 0;
            while (zdirection < z_ && elems[makeIndex(xdirection,ydirection,zdirection)] != defaultValue)
                zdirection++;
            double zvalue = field[x][y];
            if (zvalue > 1.e-15)
            {
                
                while (zvalue > 0)
                {
                    if (zdirection < z_)
                    {
                        (*this).set(xdirection,ydirection,zdirection,value);
                        zdirection++;
                        zvalue -= dz;
                    }
                    else
                        zvalue = 0;
                }
            }
        }
}

template <class T> void Field3D<T>::writeMatlabfile(void)
{
     string name = "splat";
     string space = ",";
     string fname = "runmatlab_data";
     
     int seed = 100;
     RNG Rand(seed);
     
     //Daten schreiben
     string filename = "runmatlab_plot.m";
     ofstream f (filename.c_str(), ios::out|ios::binary);
     f<<"set(0, 'DefaultAxesFontSize', 16);"<<endl;
     f<<"set(0, 'DefaultAxesFontName', 'Courier');"<<endl;
     f<<endl;
     f<<"ambs=0.5"<<endl;
     f<<"diffs=0.7"<<endl;
     f<<"SpecS=1."<<endl;
     f<<"SpecExp=5"<<endl;
     f<<endl;
     f<<"close"<<endl;
     f<<"camlight('left')"<<endl;
     f<<endl;
     std::cout << "writeMatlabFile " << OUT(Max()) << std::endl;
     
     //zufaellige Farben als RGB festlegen
     for (int i = 1; i <= Max(); i++)
         f<<"color"<<i<<" = ["<<Rand.uniform(0,1)<<" "<<Rand.uniform(0,1)<<" "<<Rand.uniform(0,1)<<"];"<<endl;
     f<<endl;
     
     for (int i = 1; i <= Max(); i++)
     {
         
         //Arrayausdehnung
         f<<name<<"(1:"<<x_+1<<space<<"1:"<<y_+1<<space<<"1:"<<z_+1<<")="<<0<<";"<<endl;
            
         //Werte speichern
	 int value = (int) ((double)i / (double)numObjects_ * 255.);
//          for (int x = 0; x < x_; x++)
//              for (int y = 0; y < y_; y++)
//                  for (int z = 0; z < z_; z++)
//                      if (elems[makeIndex(x,y,z)] != 0)
// 		       cout << OUT(x) << OUT(y) << OUT(z) << OUT(value) << " " << elems[makeIndex(x,y,z)] << endl;
	 
         for (int x = 0; x < x_; x++)
             for (int y = 0; y < y_; y++)
                 for (int z = 0; z < z_; z++)
		   // Strange error was here: Comparison was with
		   // value, output was i. Exchanged these values...?
		   // Use i+1 because Deposit writes (%255+1)
                     if (elems[makeIndex(x,y,z)] == i+1)
                         f<<name<<"("<<x+1<<space<<y+1<<space<<z+1<<")="<<value<<";"<<endl;
                         
         f<<endl;
         f<<i<<endl;
         f<<"[X1,Y1,Z1] = meshgrid(1:1:"<<x_+1<<space<<" 1:1:"<<y_+1<<space<<" 1:1:"<<z_+1<<");"<<endl;
         //f<<"patch(isocaps(X1,Y1,Z1,splat,"<<(double)i - 0.5<<", 'below' ), 'FaceColor',color"<<i<<", 'EdgeColor','none','FaceAlpha',1);"<<endl;
         //f<<"patch(isosurface(X1,Y1,Z1,splat,"<<(double)i - 0.5<<" ), 'FaceColor','blue', 'EdgeColor','none','FaceAlpha',1);"<<endl;
         f<<"patch(isosurface(X1,Y1,Z1,splat,"<<(double)i - 0.5<<" ), 'FaceColor',color"<<i<<", 'EdgeColor','none','FaceAlpha',1,..."<<endl;
         f<<"     'FaceLighting','phong','AmbientStrength',ambs,'DiffuseStrength',diffs,..."<<endl;
         f<<"     'SpecularStrength',SpecS,'SpecularExponent',SpecExp,'BackFaceLighting','unlit');"<<endl;
         f<<"clear splat;"<<endl;
         f<<endl;
         
         //Matlabdatei ausfuehren
         //string command = "matlab -nosplash -nodesktop -r ";
         //command = command + fname + itoa(i);
         //system(command.c_str());
     }
     
     f<<endl;
     f<<"daspect([1 1 1])"<<endl;
     f<<"view(3); axis tight"<<endl;
     //f<<"camlight"<<endl;
     //f<<"lighting phong"<<endl;
     //f<<"caxis([0 50])"<<endl;
     f<<" title(' ')"<<endl;
     f<<" ylim([0 "<<x_<<"])"<<endl;
     f<<" xlim([0 "<<y_<<"])"<<endl;
     f<<" zlim([0 "<<z_<<"])"<<endl;
     f<<" grid on"<<endl;
     f<<" colormap hsv"<<endl;
     f<<" set(gca, 'XTickMode', 'manual');"<<endl;
     f<<" set(gca, 'YTickMode', 'manual');"<<endl;
     f<<" set(gca, 'ZTickMode', 'manual');"<<endl;
     f<<" set(gca, 'XTick', [0,40,80,120,160,200,240,280,320,360,400]);"<<endl;
     f<<" set(gca, 'YTick', [0,40,80,120,160,200,240,280,320,360,400]);"<<endl;
     f<<" set(gca, 'ZTick', [0,40,80,120,160,200]);"<<endl;
     f<<"print -djpeg99 -cmyk SplatVolume.jpg"<<endl;
     f<<"clear"<<endl;
     f.close();
}

void shuffle(int * arr, int i, int j)
{
  int aux=arr[i];
  arr[i]=arr[j];
  arr[j]=aux;
  return;
}

template <class T> void Field3D<T>::writeTecplotfile(string filename)
{
     ofstream f (filename.c_str(), ios::out|ios::binary);
     for (int x = 0; x < x_; x++)
       for (int y = 0; y < y_; y++)
	 for (int z = 0; z < z_; z++)
	   {
	     f << elems[makeIndex(x,y,z)]  <<endl;
	     // This error occurs somehow when the field size check in
	     // DepositNew does not work correctly.
	     if (elems[makeIndex(x,y,z)] >10000)
	       error("Huge Field value at %i %i %i  value %i\n  ",
		     x,y,z, elems[makeIndex(x,y,z)]);
	     
	   }
     f.close();
}


// colorCode is not a color code, but the splat number
// the routine looks for the lowest and highest z coordinate at the
// given point (x,y) with the given "colorCode"
template <class T> bool Field3D<T>::findColor(int x, int y, int colorCode, int &zLow, int &zHigh)
{
    zLow = 0;
    zHigh = 0;
    int i = 0;
    bool found = false;

    for (int i = 0; i <z_; i++)
    {
	if  (elems[makeIndex(x,y,i)] == colorCode)
	    found = true;
    }

    if (found)
    {
	i = 0;
	while (i < z_ && elems[makeIndex(x,y,i)] != colorCode)
	    i++;
	zLow = i;

	while (i < z_ && elems[makeIndex(x,y,i)] == colorCode)
	    i++;
	zHigh = i;
	
	return true;
    }
    else
    {
	return false;
    }
    
}

template <class T> double Field3D<T>::getCoatingHeight(int x, int y, double scaleZ)
{
    int z = 0;
    int value = elems[makeIndex(x,y,z)];
    while (value != 0 && z < z_)
    {
	z++;
	value = elems[makeIndex(x,y,z)];
    }
       
    return z*scaleZ;
}

template <class T> Vector3 Field3D<T>::getNormalVector(int x, int y, double scale)
{
    Vector3 v (x, y, getCoatingHeight(x,y,scale));
    Vector3 v1 (x+1, y, getCoatingHeight(x+1,y,scale));
    Vector3 v2 (x, y-1, getCoatingHeight(x,y-1,scale));
    Vector3 v3 (x-1, y, getCoatingHeight(x-1,y,scale));
    Vector3 v4 (x, y+1, getCoatingHeight(x,y+1,scale));
    Vector3 V = (Vector3::CrossProduct(v1-v, v4-v) + Vector3::CrossProduct(v2-v, v1-v) + 
		 Vector3::CrossProduct(v3-v, v2-v) + Vector3::CrossProduct(v4-v, v3-v))/4;

    V.Normalize();
    
    return V;
	   
}

template <class T> void Field3D<T>::writePovrayFile(string filename, Problem & theProblem)
{
    ofstream f (filename.c_str(), ios::out|ios::binary);
    
    //Povray header (camera and light)
    f << "#include \"colors.inc\" "<< std::endl;
    f << std::endl;
    f << "camera{" << std::endl;
    f << "location <750, 400, 750>" << std::endl;
    f << "look_at 0" << std::endl;
    f << "angle 30" << std::endl;
    f << "}" <<std::endl;
    f << std::endl;
    f << "light_source{ <1000,1000,-1000> White }" << std::endl;
    f << "light_source{ <1000,1000,1000> White }" << std::endl;
    f << "sky_sphere { pigment { LightBlue } }" << std::endl;
    f << std::endl;

    double scale=theProblem.dz/theProblem.dx;

    //rng object (to introduce a color "error" for every Splat
    RNG Rand(1);

    //generate color error for every splat
    vector<double> colorError;
    for (int i = 0; i <= numObjects_; i++)
    {
	double randNumber = Rand.uniform(0.80, 1.2);
	//generate new rand number, so that max. value of i*rand is still numObjects_
	while (randNumber * i > numObjects_)
	    randNumber = Rand.uniform(0.80, 1.2);

	colorError.push_back(randNumber);	
    }


    for (int x = 0; x < x_-1; x++)
       for (int y = 0; y < y_-1; y++)
       {
	   int z1 = int(getCoatingHeight(x,y,1));
	   int z2 = int(getCoatingHeight(x+1,y,1));
	   int z3 = int(getCoatingHeight(x+1,y+1,1));
	   int z4 = int(getCoatingHeight(x,y+1,1));
	   z1--; z2--; z3--; z4--;
	   // value is the splat number
	   double value1 = elems[makeIndex(x,y,z1)];
	   double value2 = elems[makeIndex(x+1,y,z2)];
	   double value3 = elems[makeIndex(x+1,y+1,z3)];
	   double value4 = elems[makeIndex(x,y+1,z4)];
	   // This assumes that dx and dy are the same

	   //color of the highest element of the triangle/polygon
	   int highTriangle = MAX(MAX(z1,z2),z4);
	   int colorTriangle;
	   if (highTriangle == z1) colorTriangle = int(value1); 
	   if (highTriangle == z2) colorTriangle = int(value2);
	   if (highTriangle == z4) colorTriangle = int(value4);

	   int value = int(fabs((double(colorTriangle * colorError[colorTriangle] ))/
	   			(double)numObjects_ * 255. - 255.));

	   //NormalenVektoren bestimmen
	   Vector3 v = getNormalVector(x, y, scale);
	   Vector3 a = getNormalVector(x+1, y, scale);
	   Vector3 b = getNormalVector(x+1, y+1, scale);
	   Vector3 c = getNormalVector(x, y+1, scale);
	  
	   f << "smooth_triangle {" << std::endl;
	   f << "<" << x << ", " << (z1+1)*scale << ", " << y <<">, ";
	   f << "<" << v.X << ", " << v.Z << ", " << v.Y <<">, "<< std::endl;
	   f << "<" << x+1 << ", " << (z2+1)*scale << ", " << y <<">, ";
	   f << "<" << a.X << ", " << a.Z << ", " << a.Y <<">, "<< std::endl;
	   f << "<" << x << ", " << (z4+1)*scale << ", " << y+1 <<">, ";
	   f << "<" << c.X << ", " << c.Z << ", " << c.Y <<"> " << std::endl;
	   f << "texture { ";
	   f << "pigment {rgb <" <<(double)ppm::GetColor(HEAT, R, value)/255. << ", "
	     << (double)ppm::GetColor(HEAT, G, value)/255. << ", " 
		 << (double)ppm::GetColor(HEAT, B, value)/255. << ">} }" << std::endl;
	   f << "} " << std::endl;
	   

	   highTriangle = MAX(MAX(z2,z3),z4);
	   colorTriangle;
	   if (highTriangle == z2) colorTriangle = int(value2); 
	   if (highTriangle == z3) colorTriangle = int(value3);
	   if (highTriangle == z4) colorTriangle = int(value4);

	   value = int(fabs((double(colorTriangle * colorError[colorTriangle]))/
			    ((double)numObjects_) * 255. - 255.));
	   f << "smooth_triangle {" << std::endl;;
	   f << "<" << x+1 << ", " << (z2+1)*scale << ", " << y <<">, ";
	   f << "<" << a.X << ", " << a.Z << ", " << a.Y <<">, " << std::endl;
	   f << "<" << x+1 << ", " << (z3+1)*scale << ", " << y+1 <<">, ";
	   f << "<" << b.X << ", " << b.Z << ", " << b.Y <<">, " << std::endl;
	   f << "<" << x << ", " << (z4+1)*scale << ", " << y+1 <<">, ";
	   f << "<" << c.X << ", " << c.Z << ", " << c.Y <<"> " << std::endl;
	   f << "texture { ";
	   f << "pigment {rgb <" <<(double)ppm::GetColor(HEAT, R, value)/255. << ", "
	     << (double)ppm::GetColor(HEAT, G, value)/255. << ", " 
	     << (double)ppm::GetColor(HEAT, B, value)/255. << ">} }" << std::endl;
	   f << "} " << std::endl;
       }

    //print 4 side walls
    //1. side
    for (int x = 0; x < x_-1; x++)
    {
	int y = y_-1;
	int z = 0; //Höhenlaufvariable
	int colorTriangle;
	int zLow1;
	int zHigh1;
	int zLow2;
	int zHigh2;
	while (z < z_ && elems[makeIndex(x,y,z)] != 0)
	{
	    colorTriangle = elems[makeIndex(x,y,z)];
	    int value = int(fabs((double(colorTriangle * colorError[colorTriangle]))/
				 ((double)numObjects_) * 255. - 255.));
	    // find color does not find a color, but the z-coordinate
	    //of the given colorCode (splat number)
	    findColor(x, y, colorTriangle, zLow1, zHigh1);

	    //write Polygons with 4 edges if the same color appears in the next line
	    if (findColor(x+1, y, colorTriangle, zLow2, zHigh2))
	    {
	    
		f << "polygon {5," << std::endl;;
		f << "<" << x << ", " << zLow1*scale << ", " << y <<"> ";
		f << "<" << x << ", " << zHigh1*scale << ", " << y <<"> ";
		f << "<" << x+1 << ", " << zHigh2*scale << ", " << y <<"> ";
		f << "<" << x+1 << ", " << zLow2*scale << ", " << y <<"> ";
		f << "<" << x << ", " << zLow1*scale << ", " << y <<"> " << std::endl;
		f << "texture { ";
		f << "pigment {rgb <" <<(double)ppm::GetColor(HEAT, R, value)/255. << ", "
		  << (double)ppm::GetColor(HEAT, G, value)/255. << ", " 
		  << (double)ppm::GetColor(HEAT, B, value)/255. << ">} }" << std::endl;
		f << "} " << std::endl;
	    }
	    else
	      // draw a triangle in +x-direction to smooth edges
	    {
		//find color below current color
		int colorBelow = elems[makeIndex(x,y,z-1)];
		int colorAbove = elems[makeIndex(x,y,z+zHigh1+1)];
		if (!findColor(x+1, y, colorBelow, zLow2, zHigh2))
		{
		    //if color from below not found, search color above
		    findColor(x+1, y, colorAbove, zLow2, zHigh2);
		    zHigh2 = zLow2-1;
		}
		if (colorBelow == 0 || (colorBelow == 0 && colorAbove == 0)) zHigh2 = 0;

		f << "polygon {4," << std::endl;;
		f << "<" << x << ", " << zLow1*scale << ", " << y <<"> ";
		f << "<" << x << ", " << zHigh1*scale << ", " << y <<"> ";
		f << "<" << x+1 << ", " << (zHigh2)*scale << ", " << y <<"> ";
		f << "<" << x << ", " << zLow1*scale << ", " << y <<"> " << std::endl;
		f << "texture { ";
		f << "pigment {rgb <" <<(double)ppm::GetColor(HEAT, R, value)/255. << ", "
		  << (double)ppm::GetColor(HEAT, G, value)/255. << ", " 
		  << (double)ppm::GetColor(HEAT, B, value)/255. << ">} }" << std::endl;
		f << "} " << std::endl;

	    }
	    
	    if (x >= 1 && findColor(x-1, y, colorTriangle, zLow2, zHigh2) == false)
	      {
		// draw a triangle in -x-direction to smooth edges
		//find color below current color
		int colorBelow =  elems[makeIndex(x,y,z-1)];
		int colorAbove =  elems[makeIndex(x,y,z+zHigh1+1)];
		if (!findColor(x-1, y, colorBelow, zLow2, zHigh2))
		{
		    findColor(x-1, y, colorAbove, zLow2, zHigh2);
		    zHigh2 = zLow2-1;
		}
		if (colorBelow == 0 || (colorBelow == 0 && colorAbove == 0)) zHigh2 = 0;

		f << "polygon {4," << std::endl;;
		f << "<" << x << ", " << zLow1*scale << ", " << y <<"> ";
		f << "<" << x << ", " << zHigh1*scale << ", " << y <<"> ";
		f << "<" << x-1 << ", " << (zHigh2)*scale << ", " << y <<"> ";
		f << "<" << x << ", " << zLow1*scale << ", " << y <<"> " << std::endl;
		f << "texture { ";
		f << "pigment {rgb <" <<(double)ppm::GetColor(HEAT, R, value)/255. << ", "
		  << (double)ppm::GetColor(HEAT, G, value)/255. << ", " 
		  << (double)ppm::GetColor(HEAT, B, value)/255. << ">} }" << std::endl;
		f << "} " << std::endl;

		}
	    
	    z = zHigh1;
	}

    }

    //2. side
    for (int y = 0; y < y_-1; y++)
    {
	int x = x_-1;
	int z = 0; //Höhenlaufvariable
	int colorTriangle;
	int zLow1;
	int zHigh1;
	int zLow2;
	int zHigh2;
	while (z < z_ && elems[makeIndex(x,y,z)] != 0)
	{
	    colorTriangle = elems[makeIndex(x,y,z)];
	    int value = int(fabs((double(colorTriangle * colorError[colorTriangle]))/
				 ((double)numObjects_) * 255. - 255.));

	    findColor(x, y, colorTriangle, zLow1, zHigh1);

	    //write Polygons with 4 edges if the same color appears in the next line
	    if (findColor(x, y+1, colorTriangle, zLow2, zHigh2))
	    {
	    
		f << "polygon {5," << std::endl;;
		f << "<" << x << ", " << zLow1*scale << ", " << y <<"> ";
		f << "<" << x << ", " << zHigh1*scale << ", " << y <<"> ";
		f << "<" << x << ", " << zHigh2*scale << ", " << y+1 <<"> ";
		f << "<" << x << ", " << zLow2*scale << ", " << y+1 <<"> ";
		f << "<" << x << ", " << zLow1*scale << ", " << y <<"> " << std::endl;
		f << "texture { ";
		f << "pigment {rgb <" <<(double)ppm::GetColor(HEAT, R, value)/255. << ", "
		  << (double)ppm::GetColor(HEAT, G, value)/255. << ", " 
		  << (double)ppm::GetColor(HEAT, B, value)/255. << ">} }" << std::endl;
		f << "} " << std::endl;
	    }
	    else
	    {
		//find color below current color
		int colorBelow = elems[makeIndex(x,y,z-1)];
		int colorAbove = elems[makeIndex(x,y,z+zHigh1+1)];
		if (!findColor(x, y+1, colorBelow, zLow2, zHigh2))
		{
		    //if color from below not found, search color above
		    findColor(x, y+1, colorAbove, zLow2, zHigh2);
		    zHigh2 = zLow2-1;
		}
		if (colorBelow == 0 || (colorBelow == 0 && colorAbove == 0)) zHigh2 = 0;
		f << "polygon {4," << std::endl;;
		f << "<" << x << ", " << zLow1*scale << ", " << y <<"> ";
		f << "<" << x << ", " << zHigh1*scale << ", " << y <<"> ";
		f << "<" << x << ", " << (zHigh2)*scale << ", " << y+1 <<"> ";
		f << "<" << x << ", " << zLow1*scale << ", " << y <<"> " << std::endl;
		f << "texture { ";
		f << "pigment {rgb <" <<(double)ppm::GetColor(HEAT, R, value)/255. << ", "
		  << (double)ppm::GetColor(HEAT, G, value)/255. << ", " 
		  << (double)ppm::GetColor(HEAT, B, value)/255. << ">} }" << std::endl;
		f << "} " << std::endl;

	    }
	    
	    if (y >= 1 && findColor(x, y-1, colorTriangle, zLow2, zHigh2) == false)
	    {
		//find color below current color
		int colorBelow =  elems[makeIndex(x,y,z-1)];
		int colorAbove =  elems[makeIndex(x,y,z+zHigh1+1)];
		if (!findColor(x, y-1, colorBelow, zLow2, zHigh2))
		{
		    findColor(x, y-1, colorAbove, zLow2, zHigh2);
		    zHigh2 = zLow2-1;
		}
		if (colorBelow == 0 || (colorBelow == 0 && colorAbove == 0)) zHigh2 = 0;

		f << "polygon {4," << std::endl;;
		f << "<" << x << ", " << zLow1*scale << ", " << y <<"> ";
		f << "<" << x << ", " << zHigh1*scale << ", " << y <<"> ";
		f << "<" << x << ", " << (zHigh2)*scale << ", " << y-1 <<"> ";
		f << "<" << x << ", " << zLow1*scale << ", " << y <<"> " << std::endl;
		f << "texture { ";
		f << "pigment {rgb <" <<(double)ppm::GetColor(HEAT, R, value)/255. << ", "
		  << (double)ppm::GetColor(HEAT, G, value)/255. << ", " 
		  << (double)ppm::GetColor(HEAT, B, value)/255. << ">} }" << std::endl;
		f << "} " << std::endl;

		}
	    
	    z = zHigh1;
	}

    }

    //3. side
    for (int x = 0; x < x_-1; x++)
    {
	int y = 0;
	int z = 0; //Höhenlaufvariable
	int colorTriangle;
	int zLow1;
	int zHigh1;
	int zLow2;
	int zHigh2;
	while (z < z_ && elems[makeIndex(x,y,z)] != 0)
	{
	    colorTriangle = elems[makeIndex(x,y,z)];
	    int value = int(fabs((double(colorTriangle * colorError[colorTriangle]))/
				 ((double)numObjects_) * 255. - 255.));

	    findColor(x, y, colorTriangle, zLow1, zHigh1);

	    //write Polygons with 4 edges if the same color appears in the next line
	    if (findColor(x+1, y, colorTriangle, zLow2, zHigh2))
	    {
	    
		f << "polygon {5," << std::endl;;
		f << "<" << x << ", " << zLow1*scale << ", " << y <<"> ";
		f << "<" << x << ", " << zHigh1*scale << ", " << y <<"> ";
		f << "<" << x+1 << ", " << zHigh2*scale << ", " << y <<"> ";
		f << "<" << x+1 << ", " << zLow2*scale << ", " << y <<"> ";
		f << "<" << x << ", " << zLow1*scale << ", " << y <<"> " << std::endl;
		f << "texture { ";
		f << "pigment {rgb <" <<(double)ppm::GetColor(HEAT, R, value)/255. << ", "
		  << (double)ppm::GetColor(HEAT, G, value)/255. << ", " 
		  << (double)ppm::GetColor(HEAT, B, value)/255. << ">} }" << std::endl;
		f << "} " << std::endl;
	    }
	    else
	    {
		//find color below current color
		int colorBelow = elems[makeIndex(x,y,z-1)];
		int colorAbove = elems[makeIndex(x,y,z+zHigh1+1)];
		if (!findColor(x+1, y, colorBelow, zLow2, zHigh2))
		{
		    //if color from below not found, search color above
		    findColor(x+1, y, colorAbove, zLow2, zHigh2);
		    zHigh2 = zLow2-1;
		}
		if (colorBelow == 0 || (colorBelow == 0 && colorAbove == 0)) zHigh2 = 0;

		f << "polygon {4," << std::endl;;
		f << "<" << x << ", " << zLow1*scale << ", " << y <<"> ";
		f << "<" << x << ", " << zHigh1*scale << ", " << y <<"> ";
		f << "<" << x+1 << ", " << (zHigh2)*scale << ", " << y <<"> ";
		f << "<" << x << ", " << zLow1*scale << ", " << y <<"> " << std::endl;
		f << "texture { ";
		f << "pigment {rgb <" <<(double)ppm::GetColor(HEAT, R, value)/255. << ", "
		  << (double)ppm::GetColor(HEAT, G, value)/255. << ", " 
		  << (double)ppm::GetColor(HEAT, B, value)/255. << ">} }" << std::endl;
		f << "} " << std::endl;

	    }
	    
	    if (x >= 1 && findColor(x-1, y, colorTriangle, zLow2, zHigh2) == false)
	    {
		//find color below current color
		int colorBelow =  elems[makeIndex(x,y,z-1)];
		int colorAbove =  elems[makeIndex(x,y,z+zHigh1+1)];
		if (!findColor(x-1, y, colorBelow, zLow2, zHigh2))
		{
		    findColor(x-1, y, colorAbove, zLow2, zHigh2);
		    zHigh2 = zLow2-1;
		}
		if (colorBelow == 0 || (colorBelow == 0 && colorAbove == 0)) zHigh2 = 0;

		f << "polygon {4," << std::endl;;
		f << "<" << x << ", " << zLow1*scale << ", " << y <<"> ";
		f << "<" << x << ", " << zHigh1*scale << ", " << y <<"> ";
		f << "<" << x-1 << ", " << (zHigh2)*scale << ", " << y <<"> ";
		f << "<" << x << ", " << zLow1*scale << ", " << y <<"> " << std::endl;
		f << "texture { ";
		f << "pigment {rgb <" <<(double)ppm::GetColor(HEAT, R, value)/255. << ", "
		  << (double)ppm::GetColor(HEAT, G, value)/255. << ", " 
		  << (double)ppm::GetColor(HEAT, B, value)/255. << ">} }" << std::endl;
		f << "} " << std::endl;

		}
	    
	    z = zHigh1;
	}

    }

    //4. side
    for (int y = 0; y < y_-1; y++)
    {
	int x = 0;
	int z = 0; //Höhenlaufvariable
	int colorTriangle;
	int zLow1;
	int zHigh1;
	int zLow2;
	int zHigh2;
	while (z < z_ && elems[makeIndex(x,y,z)] != 0)
	{
	    colorTriangle = elems[makeIndex(x,y,z)];
	    int value = int(fabs((double(colorTriangle * colorError[colorTriangle]))/
				 ((double)numObjects_) * 255. - 255.));

	    findColor(x, y, colorTriangle, zLow1, zHigh1);

	    //write Polygons with 4 edges if the same color appears in the next line
	    if (findColor(x, y+1, colorTriangle, zLow2, zHigh2))
	    {
	    
		f << "polygon {5," << std::endl;;
		f << "<" << x << ", " << zLow1*scale << ", " << y <<"> ";
		f << "<" << x << ", " << zHigh1*scale << ", " << y <<"> ";
		f << "<" << x << ", " << zHigh2*scale << ", " << y+1 <<"> ";
		f << "<" << x << ", " << zLow2*scale << ", " << y+1 <<"> ";
		f << "<" << x << ", " << zLow1*scale << ", " << y <<"> " << std::endl;
		f << "texture { ";
		f << "pigment {rgb <" <<(double)ppm::GetColor(HEAT, R, value)/255. << ", "
		  << (double)ppm::GetColor(HEAT, G, value)/255. << ", " 
		  << (double)ppm::GetColor(HEAT, B, value)/255. << ">} }" << std::endl;
		f << "} " << std::endl;
	    }
	    else
	    {
		//find color below current color
		int colorBelow = elems[makeIndex(x,y,z-1)];
		int colorAbove = elems[makeIndex(x,y,z+zHigh1+1)];
		if (!findColor(x, y+1, colorBelow, zLow2, zHigh2))
		{
		    //if color from below not found, search color above
		    findColor(x, y+1, colorAbove, zLow2, zHigh2);
		    zHigh2 = zLow2-1;
		}
		if (colorBelow == 0 || (colorBelow == 0 && colorAbove == 0)) zHigh2 = 0;

		f << "polygon {4," << std::endl;;
		f << "<" << x << ", " << zLow1*scale << ", " << y <<"> ";
		f << "<" << x << ", " << zHigh1*scale << ", " << y <<"> ";
		f << "<" << x << ", " << (zHigh2)*scale << ", " << y+1 <<"> ";
		f << "<" << x << ", " << zLow1*scale << ", " << y <<"> " << std::endl;
		f << "texture { ";
		f << "pigment {rgb <" <<(double)ppm::GetColor(HEAT, R, value)/255. << ", "
		  << (double)ppm::GetColor(HEAT, G, value)/255. << ", " 
		  << (double)ppm::GetColor(HEAT, B, value)/255. << ">} }" << std::endl;
		f << "} " << std::endl;

	    }
	    
	    if (y >= 1 && findColor(x, y-1, colorTriangle, zLow2, zHigh2) == false)
	    {
		//find color below current color
		int colorBelow =  elems[makeIndex(x,y,z-1)];
		int colorAbove =  elems[makeIndex(x,y,z+zHigh1+1)];
		if (!findColor(x, y-1, colorBelow, zLow2, zHigh2))
		{
		    findColor(x, y-1, colorAbove, zLow2, zHigh2);
		    zHigh2 = zLow2-1;
		}
		if (colorBelow == 0 || (colorBelow == 0 && colorAbove == 0)) zHigh2 = 0;

		f << "polygon {4," << std::endl;;
		f << "<" << x << ", " << zLow1*scale << ", " << y <<"> ";
		f << "<" << x << ", " << zHigh1*scale << ", " << y <<"> ";
		f << "<" << x << ", " << (zHigh2)*scale << ", " << y-1 <<"> ";
		f << "<" << x << ", " << zLow1*scale << ", " << y <<"> " << std::endl;
		f << "texture { ";
		f << "pigment {rgb <" <<(double)ppm::GetColor(HEAT, R, value)/255. << ", "
		  << (double)ppm::GetColor(HEAT, G, value)/255. << ", " 
		  << (double)ppm::GetColor(HEAT, B, value)/255. << ">} }" << std::endl;
		f << "} " << std::endl;

		}
	    
	    z = zHigh1;
	}

    }


    f.close();
    system("povray polygon.pov -W4096 -H3072 -D");
    //system("xv polygon.png&");
}

template <class T> void Field3D<T>::writeTecplotfile(void)
{
     //Daten schreiben
     string filename = "resulttec.txt";
     ofstream f (filename.c_str(), ios::out|ios::binary);
     for (int x = 0; x < x_; x++)
       for (int y = 0; y < y_; y++)
	 for (int z = 0; z < z_; z++)
	   {
	     f << elems[makeIndex(x,y,z)]  <<endl;
	     if (elems[makeIndex(x,y,z)] >10000)
	       error("Huge Field value at %i %i %i  value %i\n  ",
		     x,y,z, elems[makeIndex(x,y,z)]);
	     
	   }
     f.close();
}


// Read in a Field from a file
template <class T>  Field3D<T>::Field3D(unsigned x, unsigned y, unsigned z, std::string filename): x_(x), y_(y), z_(z)
{
  elems = new T[x_*y_*z_];
  //Daten schreiben
  ifstream f (filename.c_str(), ios::out|ios::binary);
  T readMe;
  
  for (int x = 0; x < x_; x++)
    for (int y = 0; y < y_; y++)
      for (int z = 0; z < z_; z++)
	{
	  f >> readMe;
	  elems[makeIndex(x,y,z)] = readMe;
	}
  f.close();
}

template<class T> Field3D<T>& Field3D<T>::slice(string coord, int xmin, int xmax)
{
  int xF = x_;
  int yF = y_;
  int zF = z_;
  if (coord=="x") xF = xmax-xmin+1;
  if (coord=="y") yF = xmax-xmin+1;
  if (coord=="z") zF = xmax-xmin+1;
  Field3D<T> *result = new Field3D<T>(xF, yF,zF, defaultValue, numObjects_);
  int xOff=0, yOff=0, zOff=0;
  if (coord=="x") xOff = xmin;
  if (coord=="y") yOff = xmin;
  if (coord=="z") zOff = xmin;
  
  for (int x = 0; x < result->x_; x++)
    for (int y = 0; y < result->y_; y++)
      for (int z = 0; z < result->z_; z++)
	{
	  result->elems[result->makeIndex(x,y,z)] = elems[makeIndex(x+xOff,y+yOff,z+zOff)];
	}
  return  *result;
}

// This routine slices the given field so that the complete free top
// surface is in the new slice
template<class T> Field3D<T>& Field3D<T>::sliceZSurface()
{
  int zmax=-1;
  int zmin=z_+1000;
  for (int x = 0; x < x_; x++)
    for (int y = 0; y < y_; y++)
      for (int z = 0; z < z_; z++)
	{
	  T val = elems[makeIndex(x,y,z)];
	  if (val==0)
	    if (z<zmin) zmin= z;
	  if (val!=0)
	      if (z>zmax) zmax= z;
	}
  int minOff=2, maxOff=2;
  if (zmin==0) minOff=0;
  if (zmax==z_-1) maxOff=0;
  cout << OUT(zmin) << OUT(zmax) << endl;
  
  Field3D<T> *result = &(slice("z",zmin,zmax)) ;
  return *result;
}

template class Field3D<int>; 

void modulo(Field3D<int> & f, int mod)
{
  for (int x = 0; x < f.getXDim(); x++)
    for (int y = 0; y < f.getYDim(); y++)
      for (int z = 0; z < f.getZDim(); z++)
	f.set(x,y,z, f.at(x,y,z) % mod);
  return;
  
}

