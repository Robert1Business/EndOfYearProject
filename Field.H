#ifndef _FIELD
#define _FIELD
#include "definitions.H"
#include "Point.H"
#include "FluidState.H"
#include "CoatingQueue.H"
#include "NeighbourState.H"


template<class T> class Field;

template<class T> class Field1D: public std::vector <T>
{
private:
    int sizeX;

public:
    Field1D();
    Field1D(int sizeXin);

    void initialize(int sizeXin);

};

template<class T> class Field: public std::vector <Field1D<T> >
{
private:
    Lattice * theLattice;    //damit jedes Field sein Gitter kennt: hier Zeiger auf das Gitter
    int sizeX, sizeY;

public:

    Field( ) ;
    Field(Lattice &inLattice);
    Field<T>(const Field& that);
    Field<T>(Lattice &inLattice, const T init);
    
     const int getX() const;
     const int getY() const;
    
    void write();
    void writetofile();
  void readfromfile();
  
    Field<T> Cut(Point p, Lattice theLattice);
  void Deposit(Point p, Field<T> &field); //deposite/add a second Field
  void MakeGlobal(Point p, Field<T> &field);
  
  
    Field operator+(Field& addField);
    Field operator-(Field& subField);
    Field& operator=(const Field& that);
    T & operator()(const Point& p);
    T & operator()(int x, int y);
    friend class Lattice;
};


Field<real> averageField(Field<real> field, int range);

double roughnessRa(Field<real> field);
double roughnessMinMax(Field<real> field);
double mean(Field<real> field);




#endif
