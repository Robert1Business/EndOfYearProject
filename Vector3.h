//3dimensionale Vektorklasse
#ifndef __VECTOR3_H
#define __VECTOR3_H

#include <iostream>
#include <string>
#include <fstream>
#include <math.h>

class Vector3
{

public:
	//Attribute
	double	X; // x- Komponente des Vektors
	double	Y; // z- Komponente des Vektors
	double	Z; // z- Komponente des Vektors

	double EqualityTolerance; //Toleranz zur Bestimmung der Gleichheit von Vektoren

	//Konstruktoren
	Vector3(); //Nullvektor
	Vector3(double x, double y, double z);
	Vector3(double xyz[]);
	Vector3(const Vector3 &v1);

	//Destruktor
	~Vector3();

	
	//Funktionen (skalarer Rückgabe)
    	double Magnitude();//Betrag zurückgeben
	void Magnitude(double newMagnitude);//Betrag setzen (Winkel bleiben gleich)
	
	double SumComponents(); //Komponenten summieren
	static double SumComponents (Vector3 &v);
	
	double SumSqComponents(); //Komponten quadrieren und summieren (X^2+Y^2+Z^2) 
	static double SumSqComponents(Vector3 &v);
	
	static double DotProduct(Vector3 v1, Vector3 v2); //Skalarprodukt
	double DotProduct(Vector3 other);

	static double MixedProduct(Vector3 v1, Vector3 v2, Vector3 v3); //Spatprodukt
	double MixedProduct(Vector3 other_v1, Vector3 other_v2);

	static double Distance(Vector3 v1, Vector3 v2); //Abstand zw. 2 Vektoren
	double Distance(Vector3 other);

	static double Angle(Vector3 v1, Vector3 v2); //Winkel zw. 2 Vektoren
	double Angle(Vector3 other);


	//Vektorfunktionen
	static Vector3 CrossProduct(Vector3 v1, Vector3 v2); //Kreuzprodukt
	Vector3 CrossProduct(Vector3 other);

	static Vector3 Normalize(Vector3 v); //Normalisieren (Betrag = 1)
	void Normalize(void);

	static Vector3 Max(Vector3 v1, Vector3 v2); //gibt größeren Vektor zurück
	Vector3 Max(Vector3 other);

	static Vector3 Min(Vector3 v1, Vector3 v2); //gibt kleineren Vektor zurück
	Vector3 Min(Vector3 other);


	//Vektor drehen	(alle Winkel in Rad)
	void Yaw(double alpha); //Drehung um Y-Achse
	
	void Pitch(double alpha); //Drehung um X-Achse

	void Roll(double alpha); //Drehung um Z-Achse

	void Turn(Vector3 &vnorm, double alpha);  //allgemeine Drehung um einen beliebigen Einheitsvektor p um den Winkel alpha


	//überladene Operationen
	friend std::ostream& operator<<(std::ostream &os, Vector3 &v); //Ausgabe

	Vector3 operator+(const Vector3 &v)const; //Vektoraddition
	Vector3 operator+(double val) const;

	Vector3 operator-(const Vector3 &v)const; //Vektorsubtraktion
	Vector3 operator-(double val) const;
	
	Vector3 operator*(double val) const; //Skalarmultiplikation
	friend Vector3 operator*(double val, const Vector3& v1);

	Vector3 operator/(double val) const; //Division

	void Negate(); //Negation der Vektorelemente
    	Vector3 operator-(); //Negation als Operator

	void Reinforcement(); //Vektorelemente positiv machen
    	Vector3 operator+(); //als Operator

	Vector3& operator=(const Vector3 &v); //Zuweisung/ kopieren
	
	//Vergleichsoperatoren
	bool operator<(Vector3 &v2); //Betrag kleiner
	bool operator>(Vector3 &v2); //Betrag größer
	bool operator<=(Vector3 &v2); //Betrag kleiner
	bool operator>=(Vector3 &v2); //Betrag größer
	bool operator==(Vector3 &v2); //Vektoren gleich (mit "EqualityTolerance")
	bool operator!=(Vector3 &v2); //Vektoren ungleich

	//gibt Vektor in Datei durch anhängen aus
	//start= Ursprungsvektor
	void appendToFile(Vector3 &start, std::string filename);


	
	
//sonstige Funktionen (noch in C++ umzusetzen
/*

	

    /// <summary>
    /// The individual multiplication to a power of a Vector3's components
    /// </summary>
    /// <param name="v1">The vector whose scalar components to multiply by a power</param>
    /// <param name="power">The power by which to multiply the components</param>
    /// <returns>The multiplied Vector3</returns>
    public static Vector3 PowComponents(Vector3 v1, double power)
    {
        return
        (
            new Vector3
                (
                    Math.Pow(v1.X, power),
                    Math.Pow(v1.Y, power),
                    Math.Pow(v1.Z, power)
                )
        );
    }

    /// <summary>
    /// The individual multiplication to a power of this Vector3's components
    /// </summary>
    /// <param name="power">The power by which to multiply the components</param>
    /// <returns>The multiplied Vector3</returns>
    /// <implementation>
    /// <see cref="PowComponents(Vector3, Double)"/>
    /// The Components.PowComponents(Vector3, double) function has been used to prevent code duplication
    /// </implementation>
    public void PowComponents(double power)
    {
        this = PowComponents(this, power);
    }

    /// <summary>
    /// The individual square root of a Vector3's components
    /// </summary>
    /// <param name="v1">The vector whose scalar components to square root</param>
    /// <returns>The rooted Vector3</returns>
    public static Vector3 SqrtComponents(Vector3 v1)
    {
        return
            (
            new Vector3
                (
                    Math.Sqrt(v1.X),
                    Math.Sqrt(v1.Y),
                    Math.Sqrt(v1.Z)
                )
            );
    }

    /// <summary>
    /// The individual square root of this Vector3's components
    /// </summary>
    /// <returns>The rooted Vector3</returns>
    /// <implementation>
    /// <see cref="SqrtComponents(Vector3)"/>
    /// The Components.SqrtComponents(Vector3) function has been used to prevent code duplication
    /// </implementation>
    public void SqrtComponents()
    {
        this = SqrtComponents(this);
    }

    /// <summary>
    /// The Vector3's components squared
    /// </summary>
    /// <param name="v1">The vector whose scalar components are to square</param>
    /// <returns>The squared Vector3</returns>
    public static Vector3 SqrComponents(Vector3 v1)
    {
        return
            (
            new Vector3
                (
                    v1.X * v1.X,
                    v1.Y * v1.Y,
                    v1.Z * v1.Z
                )
            );
    }

    /// <summary>
    /// The Vector3's components squared
    /// </summary>
    /// <returns>The squared Vector3</returns>
    /// <implementation>
    /// <see cref="SqrtComponents(Vector3)"/>
    /// The Components.SqrComponents(Vector3) function has been used to prevent code duplication
    /// </implementation>
    public void SqrComponents()
    {
        this = SqrtComponents(this);
    }



    /// <summary>
    /// Textual description of the Vector3
    /// </summary>
    /// <param name="format">Formatting string: 'x','y','z' or '' followed by standard numeric format string characters valid for a double precision floating point</param>
    /// <param name="formatProvider">The culture specific fromatting provider</param>
    /// <returns>Text (String) representing the vector</returns>
    public string ToString(string format, IFormatProvider formatProvider)
    {
        // If no format is passed
        if (format == null || format == "") return String.Format("({0}, {1}, {2})", X, Y, Z);

        char firstChar = format[0];
        string remainder = null;

        if (format.Length > 1)
        remainder = format.Substring(1);

        switch (firstChar)
        {
            case 'x': return X.ToString(remainder, formatProvider);
            case 'y': return Y.ToString(remainder, formatProvider);
            case 'z': return Z.ToString(remainder, formatProvider);
            default: 
                return String.Format
                    (
                        "({0}, {1}, {2})",
                        X.ToString(format, formatProvider),
                        Y.ToString(format, formatProvider),
                        Z.ToString(format, formatProvider)
                    );
        }
    }

	/// <summary>
    /// compares the magnitude of this instance against the magnitude of the supplied vector
    /// </summary>
    /// <param name="other">The vector to compare this instance with</param>
    /// <returns>
    /// -1: The magnitude of this instance is less than the others magnitude
    /// 0: The magnitude of this instance equals the magnitude of the other
    /// 1: The magnitude of this instance is greater than the magnitude of the other
    /// </returns>
    /// <implementation>
    /// Implemented to fulfil the IComparable interface
    /// </implementation>
    /// <Acknowledgement>This code is adapted from Exocortex - Ben Houston </Acknowledgement>
    public int CompareTo(Vector3 other)
    {
        if (this < other)
        {
            return -1;
        }
        else if (this > other)
        {
            return 1;
        }
        return 0;
    }

	/// <summary>
	/// Checks if a vector a unit vector
	/// Checks if the Vector3 has been normalized
	/// Checks if a vector has a magnitude of 1
	/// </summary>
	/// <param name="v1">
	/// The vector to be checked for Normalization
	/// </param>
	/// <returns>Truth if the vector is a unit vector</returns>
	/// <implementation>
	/// <see cref="Magnitude"/>	
	/// Uses the Magnitude property in the check to avoid code duplication
    /// Within a tolerence
	/// </implementation>
	public static bool IsUnitVector(Vector3 v1)
	{
        return Math.Abs(v1.Magnitude -1) <= EqualityTolerence;
	}

	/// <summary>
	/// Checks if the vector a unit vector
	/// Checks if the Vector3 has been normalized 
	/// Checks if the vector has a magnitude of 1
	/// </summary>
	/// <returns>Truth if this vector is a unit vector</returns>
	/// <implementation>
	/// <see cref="IsUnitVector(Vector3)"/>	
	/// Uses the isUnitVector(Vector3) property in the check to avoid code duplication
    /// Within a tolerence
	/// </implementation>
	public bool IsUnitVector()
	{
		return IsUnitVector(this);
	}

	/// <summary>
	/// Checks if two Vectors are perpendicular
	/// Checks if two Vectors are orthogonal
	/// Checks if one Vector3 is the normal of the other
	/// </summary>
	/// <param name="v1">
	/// The vector to be checked for orthogonality
	/// </param>
	/// <param name="v2">
	/// The vector to be checked for orthogonality to
	/// </param>
	/// <returns>Truth if the two Vectors are perpendicular</returns>
	/// <implementation>
	/// Uses the DotProduct function in the check to avoid code duplication
	/// </implementation>
	public static bool IsPerpendicular(Vector3 v1, Vector3 v2)
	{
        return v1.DotProduct(v2) == 0;
	}

	/// <summary>
	/// Checks if two Vectors are perpendicular
	/// Checks if two Vectors are orthogonal
	/// Checks if one Vector3 is the Normal of the other
	/// </summary>
	/// <param name="other">
	/// The vector to be checked for orthogonality
	/// </param>
	/// <returns>Truth if the two Vectors are perpendicular</returns>
	/// <implementation>
	/// Uses the isPerpendicualr(Vector3, Vector3) function in the check to avoid code duplication
	/// </implementation>
	public bool IsPerpendicular(Vector3 other)
	{
		return IsPerpendicular(this, other);
	}

	#endregion

	#region Messages

	/// <summary>
	/// Exception message descriptive text 
	/// Used for a failure for an array argument to have three components when three are needed 
	/// </summary>
	private const string THREE_COMPONENTS = "Array must contain exactly three components , (x,y,z)";

	/// <summary>
	/// Exception message descriptive text 
	/// Used for a divide by zero event caused by the normalization of a vector with magnitude 0 
	/// </summary>
	private const string NORMALIZE_0 = "Can not normalize a vector when it's magnitude is zero";

	/// <summary>
	/// Exception message descriptive text 
	/// Used when interpolation is attempted with a control parameter not between 0 and 1 
	/// </summary>
	private const string INTERPOLATION_RANGE = "Control parameter must be a value between 0 & 1";

	/// <summary>
	/// Exception message descriptive text 
	/// Used when attempting to compare a Vector3 to an object which is not a type of Vector3 
	/// </summary>
	private const string NON_VECTOR_COMPARISON = "Cannot compare a Vector3 to a non-Vector3";

	/// <summary>
	/// Exception message additional information text 
	/// Used when adding type information of the given argument into an error message 
	/// </summary>
	private const string ARGUMENT_TYPE = "The argument provided is a type of ";

	/// <summary>
	/// Exception message additional information text 
	/// Used when adding value information of the given argument into an error message 
	/// </summary>
	private const string ARGUMENT_VALUE = "The argument provided has a value of ";

	/// <summary>
	/// Exception message additional information text 
	/// Used when adding length (number of components in an array) information of the given argument into an error message 
	/// </summary>
	private const string ARGUMENT_LENGTH = "The argument provided has a length of ";

	/// <summary>
	/// Exception message descriptive text 
	/// Used when attempting to set a Vectors magnitude to a negative value 
	/// </summary>
	private const string NEGATIVE_MAGNITUDE = "The magnitude of a Vector3 must be a positive value, (i.e. greater than 0)";

	/// <summary>
	/// Exception message descriptive text 
	/// Used when attempting to set a Vectors magnitude where the Vector3 represents the origin
	/// </summary>
	private const string ORAGIN_VECTOR_MAGNITUDE = "Cannot change the magnitude of Vector3(0,0,0)";

	///////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////


	//Konstanten
    /// <summary>
    /// The tolerence used when determining the equality of two vectors 
    /// </summary>
    public const double EqualityTolerence = Double.Epsilon;

    /// <summary>
    /// The smallest vector possible (based on the double precision floating point structure)
    /// </summary>
    public static readonly Vector3 MinValue = new Vector3(Double.MinValue, Double.MinValue, Double.MinValue);

    /// <summary>
    /// The largest vector possible (based on the double precision floating point structure)
    /// </summary>
    public static readonly Vector3 MaxValue = new Vector3(Double.MaxValue, Double.MaxValue, Double.MaxValue);

    /// <summary>
    /// The smallest positive (non-zero) vector possible (based on the double precision floating point structure)
    /// </summary>
    public static readonly Vector3 Epsilon = new Vector3(Double.Epsilon, Double.Epsilon, Double.Epsilon);
	*/
};


//Ausgezeichnete Vektoren
static Vector3 origin(0,0,0);
static Vector3 xAxis (1,0,0);
static Vector3 yAxis (0,1,0);
static Vector3 zAxis (0,0,1);

#endif

