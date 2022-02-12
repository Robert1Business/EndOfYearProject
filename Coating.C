#include "Coating.H"

Coating::Coating(Lattice &inLattice):
  Field<CoatingQueue>(inLattice)
{
}
;

std::ostream& operator<<(std::ostream& s, const Coating& coat)
{
  for(int x = 0; x < coat.getX(); x++)
    {
      for (int y = 0; y < coat.getY(); y++)
	{
	s << " " << x << " " << y << " " << coat[x][y] << "\n";
	}
    }
  
  return s;
}


void Coating::writePovrayFile(string filename, Problem & theProblem)
{
    ofstream f (filename.c_str(), ios::out|ios::binary);
    
    //Povray header (camera and light)
    f << "#include \"colors.inc\" "<< std::endl;
    f << std::endl;
    f << "camera{" << std::endl;
    double dist= 2.5 * MAX(getX(), getY());
    f << "location <" << dist << ", 400, " << dist << ">" << std::endl;
    f << "look_at 0" << std::endl;
    f << "angle 30" << std::endl;
    f << "}" <<std::endl;
    f << std::endl;
    f << "light_source{ <1000,1000,-1000> White }" << std::endl;
    f << "light_source{ <1000,1000,1000> White }" << std::endl;
    f << "sky_sphere { pigment { LightBlue } }" << std::endl;
    f << std::endl;
    // The block macro draws a block at a given position with given
    // height
    // Its edge length is fixed to 1
    f << "#macro block (xpos,ypos, zlow, zhigh, redv, greenv, bluev) " << std::endl;
    f << "polygon {" << std::endl;
    f << "5," << std::endl;
    f << "<xpos-0.5, zhigh, ypos-0.5> " << std::endl;
    f << "<xpos-0.5, zhigh, ypos+0.5> " << std::endl;
    f << "<xpos+0.5, zhigh, ypos+0.5> " << std::endl;
    f << "<xpos+0.5, zhigh, ypos-0.5> " << std::endl;
    f << "<xpos-0.5, zhigh, ypos-0.5> " << std::endl;
    f << "texture { " << std::endl;
    f << "pigment {rgb <redv, greenv, bluev>} }" << std::endl;
    f << "} " << std::endl;
    f << "polygon {" << std::endl;
    f << "5," << std::endl;
    f << "<xpos-0.5, zhigh, ypos-0.5> " << std::endl;
    f << "<xpos-0.5, zhigh, ypos+0.5> " << std::endl;
    f << "<xpos-0.5, zlow, ypos+0.5> " << std::endl;
    f << "<xpos-0.5, zlow, ypos-0.5> " << std::endl;
    f << "<xpos-0.5, zhigh, ypos-0.5> " << std::endl;
    f << "texture { " << std::endl;
    f << "pigment {rgb <redv, greenv, bluev>} }" << std::endl;
    f << "} " << std::endl;
    f << "polygon {" << std::endl;
    f << "5," << std::endl;
    f << "<xpos+0.5, zhigh, ypos-0.5> " << std::endl;
    f << "<xpos+0.5, zhigh, ypos+0.5> " << std::endl;
    f << "<xpos+0.5, zlow, ypos+0.5> " << std::endl;
    f << "<xpos+0.5, zlow, ypos-0.5> " << std::endl;
    f << "<xpos+0.5, zhigh, ypos-0.5> " << std::endl;
    f << "texture { " << std::endl;
    f << "pigment {rgb <redv, greenv, bluev>} }" << std::endl;
    f << "} " << std::endl;
    f << "polygon {" << std::endl;
    f << "5," << std::endl;
    f << "<xpos-0.5, zhigh, ypos+0.5> " << std::endl;
    f << "<xpos+0.5, zhigh, ypos+0.5> " << std::endl;
    f << "<xpos+0.5, zlow, ypos+0.5> " << std::endl;
    f << "<xpos-0.5, zlow, ypos+0.5> " << std::endl;
    f << "<xpos-0.5, zhigh, ypos+0.5> " << std::endl;
    f << "texture { " << std::endl;
    f << "pigment {rgb <redv, greenv, bluev>} }" << std::endl;
    f << "} " << std::endl;
    f << "polygon {" << std::endl;
    f << "5," << std::endl;
    f << "<xpos-0.5, zhigh, ypos-0.5> " << std::endl;
    f << "<xpos+0.5, zhigh, ypos-0.5> " << std::endl;
    f << "<xpos+0.5, zlow, ypos-0.5> " << std::endl;
    f << "<xpos-0.5, zlow, ypos-0.5> " << std::endl;
    f << "<xpos-0.5, zhigh, ypos-0.5> " << std::endl;
    f << "texture { " << std::endl;
    f << "pigment {rgb <redv, greenv, bluev>} }" << std::endl;
    f << "} " << std::endl;
    f << "#end" << std::endl;

    // This assumes that dx=dy, which should always be the case
    double zscale = 1./theProblem.dx;
    double zmin,zmax;
    // Color values
    double rcol, gcol, bcol;
    
    for(int x = 0; x < (*this).getX(); x++)
      for (int y = 0; y < (*this).getY(); y++)
	{
	  // Iterate through the queue and write all grid points
	  for (int i=0; i<(*this)[x][y].size(); i++)
	    {
	      if (i==0)
		zmin=0;
	      else
		zmin=((*this)[x][y])[i-1].splatZ;
	      zmax =((*this)[x][y])[i].splatZ;
	      int repeatFrequency=50;
	      int num=((*this)[x][y])[i].splatNumber;
	      int value = int(double(num%repeatFrequency)/repeatFrequency
	   			*255);
	      rcol=(double)ppm::GetColor(HEAT, R, value)/255.;
	      gcol=(double)ppm::GetColor(HEAT, G, value)/255.;
	      bcol=(double)ppm::GetColor(HEAT, B, value)/255.;
	      
	      f << "block(" << x << "," << y << "," << zmin*zscale << "," << zmax*zscale << "," << 
		rcol << "," <<  gcol << "," <<  bcol << ")\n" ;
	      
	    }
	  

	}
    
}

