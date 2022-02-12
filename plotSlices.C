//Main program for three-dimensional MC simultion of a tbc coating process

#include "Splat.H"
#include "Field.H"
#include "Lattice.H"
#include "definitions.H"
#include "Problem.H"
#include "Field.H"
#include "Field3D.H"
#include "rng.h"
#include "pgm.h"
#include "ppm.h"


int main (int argc, char *argv[])
{    
  
    //Parameterliste ueberpruefen
    if (argc != 2) 
    {
	cout<<"Ungueltige Parameterliste!"<<endl;
	cout<<"Programm bitte ausschliesslich mit dem Dateinamen der Input-Datei starten!"<<endl;
	exit(1);
    }
    
    string filename = argv[1];
    
    if(!renamer::FileExists(filename))
    {
        cout<<"Datei "<<filename<<" nicht gefunden!"<<endl;
        exit(1);
    }
    
    Problem theProblem(filename); //Problem einlesen

    Field3D<int> Volume(theProblem.dimXMain,theProblem.dimYMain,theProblem.dimZMain, "resulttec.txt"); 
    Field3D<int> slice1= Volume.slice("z", 0, 2);
    cout << "BEfore" << slice1.at(0,0,0) <<endl;
    modulo(slice1,250);
    cout << OUT(slice1.getXDim()) << OUT(slice1.getYDim()) << OUT(slice1.getZDim()) << endl;
    slice1.writeTecplotfile("resultSlice1.txt");

//   for (int x = 0; x < slice1.getXDim(); x++)
//     for (int y = 0; y < slice1.getYDim(); y++)
//       for (int z = 0; z < slice1.getZDim(); z++)
// 	if (slice1.at(x,y,z) ==0)
// 	  cout << "0 value at " << OUT(x) << OUT(y) << OUT(z);
//   exit(1);
  

    Field3D<int> slice2= Volume.slice("y", 0, 2);
    cout << "BEfore" << slice2.at(0,0,0) <<endl;
    modulo(slice2,250);
    cout << OUT(slice2.getXDim()) << OUT(slice2.getYDim()) << OUT(slice2.getZDim()) << endl;
    slice2.writeTecplotfile("resultSlice2.txt");

    Field3D<int> slice3= Volume.sliceZSurface();
    modulo(slice3,250);
    cout << OUT(slice3.getXDim()) << OUT(slice3.getYDim()) << OUT(slice3.getZDim()) << endl;
    slice3.writeTecplotfile("resultSliceZ.txt");
    Field3D<int> slice4= slice3.slice("y", 0, 75);
    Field3D<int> slice5= slice4.slice("x", 0, 75);
    cout << OUT(slice5.getXDim()) << OUT(slice5.getYDim()) << OUT(slice5.getZDim()) << endl;
    slice5.writeTecplotfile("resultSlice5.txt");

    std::cout<<"Program finished"<<std::endl;
}
