#include "Vector3.h"

Vector3::Vector3()
{
	EqualityTolerance = 1e-24;
	X = 0;
	Y = 0;
	Z = 0;
}

Vector3::Vector3(double x, double y, double z)
{
	// Initialisation
	EqualityTolerance = 1e-24;
	X = x;
	Y = y;
	Z = z;
}

Vector3::Vector3(double xyz[])
{
	// Initialisation
	EqualityTolerance = 1e-24;
	X = xyz[0];
	Y = xyz[1];
	Z = xyz[2];
}

Vector3::Vector3(const Vector3 &v1)
{
	// Initialisation
	EqualityTolerance = 1e-24;
	X = v1.X;
	Y = v1.Y;
	Z = v1.Z;
}

/*Vector3::Vector3(const Vector3 v1)
{
	// Initialisation
	EqualityTolerance = 1e-24;
	X = v1.X;
	Y = v1.Y;
	Z = v1.Z;
}*/


Vector3::~Vector3()
{
	//nothing to do
}

//Operationen

Vector3 Vector3::operator+(const Vector3 &v)const
{
	Vector3 v2 (this->X + v.X, this->Y + v.Y, this->Z + v.Z);
	return v2;
}

Vector3 Vector3::operator+(double val) const
{
    Vector3 v2 (this->X + val, this->Y + val, this->Z + val);
	return v2;
}

Vector3 Vector3::operator-(const Vector3 &v)const
{
	Vector3 v2 (this->X - v.X, this->Y - v.Y, this->Z - v.Z);
	return v2;
}

Vector3 Vector3::operator-(double val) const
{
    Vector3 v2 (this->X - val, this->Y - val, this->Z - val);
	return v2;
}

Vector3 Vector3::operator*(double val) const
{
    Vector3 v2 (this->X * val, this->Y * val, this->Z * val);
	return v2;
}

Vector3 operator*(double val, const Vector3& v1)
{
    Vector3 v2;
	v2 = v1 * val;
	return v2;
}

Vector3 Vector3::operator/(double val) const
{
    Vector3 v2 (this->X / val, this->Y / val, this->Z / val);
	return v2;
}

Vector3& Vector3::operator=(const Vector3 &v)
{
	if (this == &v)
		return *this;
	else
	{
		this->X = v.X;
		this->Y = v.Y;
		this->Z = v.Z;
	}

	return *this;
}

void Vector3::Negate()
{
	this->X *= -1;
	this->Y *= -1;
	this->Z *= -1;
}
Vector3 Vector3::operator-()
{
	Vector3 v(*this);
	v.Negate();
	return v;
}

void Vector3::Reinforcement()
{
	this->X = fabs(this->X);
	this->Y = fabs(this->Y);
	this->Z = fabs(this->Z);
}
Vector3 Vector3::operator+()
{
	Vector3 v(*this);
	v.Reinforcement();
	return v;
}

std::ostream& operator<<(std::ostream& os, Vector3& v)
{
	os << "[" << v.X << " " << v.Y << " " << v.Z << "]T"; //in Ostream schieben
    return os; 
}

bool Vector3::operator<(Vector3 &v2)
{
	if (this->Magnitude() < v2.Magnitude())
		return true;
	else
		return false;
}

bool Vector3::operator>(Vector3 &v2)
{
	if (this->Magnitude() > v2.Magnitude())
		return true;
	else
		return false;
}

bool Vector3::operator<=(Vector3 &v2)
{
	if (this->Magnitude() <= v2.Magnitude())
		return true;
	else
		return false;
}

bool Vector3::operator>=(Vector3 &v2)
{
	if (this->Magnitude() >= v2.Magnitude())
		return true;
	else
		return false;
}

bool Vector3::operator==(Vector3 &v2)
{
	return
        (
            fabs(this->X - v2.X) <= EqualityTolerance &&
            fabs(this->Y - v2.Y) <= EqualityTolerance &&
            fabs(this->Z - v2.Z) <= EqualityTolerance
        );
}

bool Vector3::operator!=(Vector3 &v2)
{
	return !(*this==v2);
}

double Vector3::Magnitude(void)
{
	return sqrt(SumSqComponents());
}

double Vector3::SumComponents()
{
	return (X+Y+Z);
}

double Vector3::SumComponents (Vector3 &v)
{
	return (v.SumComponents());
}

double Vector3::SumSqComponents()
{
	return (pow(X,2)+pow(Y,2)+pow(Z,2));
}

double Vector3::SumSqComponents(Vector3 &v)
{
	return (v.SumSqComponents());
}

Vector3 Vector3::CrossProduct(Vector3 v1, Vector3 v2)
{
	Vector3 v3 (v1.Y * v2.Z - v1.Z * v2.Y,
				v1.Z * v2.X - v1.X * v2.Z,
				v1.X * v2.Y - v1.Y * v2.X);
	return v3;
}

Vector3 Vector3::CrossProduct(Vector3 other)
{
	return CrossProduct(*this, other);
}

double Vector3::DotProduct(Vector3 v1, Vector3 v2)
{
	return (v1.X * v2.X + 
			v1.Y * v2.Y +
			v1.Z * v2.Z);
}

double Vector3::DotProduct(Vector3 other)
{
	return DotProduct(*this, other);
}

double Vector3::MixedProduct(Vector3 v1, Vector3 v2, Vector3 v3)
{
	return DotProduct(CrossProduct(v1, v2), v3);
}

double Vector3::MixedProduct(Vector3 other_v1, Vector3 other_v2)
{
	return DotProduct(CrossProduct(*this, other_v1), other_v2);
}

Vector3 Vector3::Normalize(Vector3 v)
{
	Vector3 v1 (v);
	if ( v.Magnitude() != 0 ) 
		{
			// find the inverse of the vectors magnitude
			double inverse = 1 / v.Magnitude();
			// multiply each component by the inverse of the magnitude
			v1.X *= inverse;
			v1.Y *= inverse;
			v1.Z *= inverse;
		}
	return v1;
}

void Vector3::Normalize(void)
{
	*this = Normalize(*this);
}

double Vector3::Distance(Vector3 v1, Vector3 v2)
{
	Vector3 v;
	v = v2 - v1;
	return  (v.Magnitude());
}

double Vector3::Distance(Vector3 other)
{
	return Distance(*this, other);
}

double Vector3::Angle(Vector3 v1, Vector3 v2)
{
	return 
		(
			acos(Normalize(v1).DotProduct(Normalize(v2)))
		);
}

double Vector3::Angle(Vector3 other)
{
	return Angle(*this, other);
}

Vector3 Vector3::Max(Vector3 v1, Vector3 v2)
{
	if (v1 >= v2){return v1;}
		return v2;
}

Vector3 Vector3::Max(Vector3 other)
{
	return Max(*this, other);
}

Vector3 Vector3::Min(Vector3 v1, Vector3 v2)
{
	if (v1 <= v2){return v1;}
		return v2;
}

Vector3 Vector3::Min(Vector3 other)
{
	return Min(*this, other);
}

void Vector3::appendToFile(Vector3 &start, std::string filename)
{
	//Datei öffnen zum anhängen
	std::ofstream f (filename.c_str(), std::ios::app );
	
	//aktuellen Vektor ausgeben
	f<<start.X<<" "<<start.Y<<" "<<start.Z<<" "<<X<<" "<<Y<<" "<<Z<<std::endl;
	
	f.close();
}

void Vector3::Turn(Vector3 &vnorm, double alpha)
{
	//allgemeine Drehmatrix definieren
	double matrix[3][3];
	matrix[0][0] = cos(alpha) + pow(vnorm.X, 2) * (1.0 - cos(alpha));
	matrix[0][1] = vnorm.X * vnorm.Y * (1.0 - cos(alpha)) - vnorm.Z * sin(alpha);
	matrix[0][2] = vnorm.X * vnorm.Z * (1.0 - cos(alpha)) + vnorm.Y * sin(alpha);

	matrix[1][0] = vnorm.Y * vnorm.X * (1.0 - cos(alpha)) + vnorm.Z * sin(alpha);
	matrix[1][1] = cos(alpha) + pow(vnorm.Y, 2) * (1.0 - cos(alpha));
	matrix[1][2] = vnorm.Y * vnorm.Z * (1.0 - cos(alpha)) - vnorm.X * sin(alpha);

	matrix[2][0] = vnorm.Z * vnorm.X * (1.0 - cos(alpha)) - vnorm.Y * sin(alpha);
	matrix[2][1] = vnorm.Z * vnorm.Y * (1.0 - cos(alpha)) + vnorm.X * sin(alpha);
	matrix[2][2] = cos(alpha) + pow(vnorm.Z, 2) * (1.0 - cos(alpha));

	//tempuräre Variablen
	double tx;
	double ty;
	double tz;

	//neue Koordinaten durch Matrixmultiplikation berechnen	
	tx = X * matrix[0][0] + Y * matrix[0][1] + Z * matrix[0][2];
	ty = X * matrix[1][0] + Y * matrix[1][1] + Z * matrix[1][2];
	tz = X * matrix[2][0] + Y * matrix[2][1] + Z * matrix[2][2];

	//globale Koordinaten überschreiben
	X = tx;
	Y = ty;
	Z = tz;
}

void Vector3::Yaw(double alpha)
{
	double x = (Z * sin(alpha) ) + (X * cos(alpha) );
	double z = (Z * cos(alpha) ) - (X * sin(alpha) );
	
	X = x;
	Z = z;
}

void Vector3::Pitch(double alpha)
{
	double y = ( Y * cos(alpha) ) - ( Z * sin(alpha) );
	double z = ( Y * sin(alpha) ) + ( Z * cos(alpha) );
	
	Y = y;
	Z = z;
}

void Vector3::Roll(double alpha)
{
	double x = ( X * cos(alpha) ) - ( Y * sin(alpha) );
	double y = ( X * sin(alpha) ) + ( Y * cos(alpha) );	
	X = x;
	Y = y;
}

