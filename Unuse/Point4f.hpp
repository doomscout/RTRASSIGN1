#pragma once
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
class Point4f
{
private:
	GLfloat _x;
	GLfloat _y;
	GLfloat _z;
	GLfloat _w;

public:
	// Accessors
	GLfloat getX() const;
	GLfloat getY() const;
	GLfloat getZ() const;
	GLfloat getW() const;
	// Mutators
	void setX(GLfloat x);
	void setY(GLfloat y);
	void setZ(GLfloat z);
	void setW(GLfloat w);
	
	//Constructor
	Point4f();
	Point4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	
	//Copy Constructor
	Point4f(const Point4f &a) : _x(a.getX()), _y(a.getY()), _z(a.getZ()), _w(a.getW()){}

	//return reference to the lvalue
	Point4f &operator =(const Point4f &a);
	
	
	//Check for equality from other value of Class
	bool operator ==(const Point4f &a) const;
	
	bool operator !=(const Point4f &a) const;
	
	//Set the vector to zero
	
	void zero();
		
	
	// Unary minus returns the negative of the vector 
	
	Point4f operator -() const { return Point4f(-_x,-_y,-_z,-_w);}

	//Operator Overload for calucation vector3f
	
	Point4f operator + (const Point4f &a) const
	{
		return Point4f( _x + a.getX(), _y + a.getY(), _z + a.getZ(), _w +a.getW() );
	}
		
	Point4f operator - (const Point4f &a) const
	{
		return Point4f( _x - a.getX(), _y - a.getY(), _z - a.getZ(), _w - a.getZ() );
	}

	// Multipilication and division by scalar
	Point4f operator * (GLfloat a) const
	{
		return Point4f( _x * a, _y * a, _z * a, _w * a );
	}
	
	//Divide by GLfloat for Point4f 
	Point4f operator / (GLfloat a) const
	{
		GLfloat oneOverA = 1.0f/a;
		
		return Point4f( _x * oneOverA, _y * oneOverA, _z * oneOverA, _w * oneOverA);
	}
	
	Point4f &operator +=(const Point4f &a);
	Point4f &operator -=(const Point4f &a);
	Point4f &operator *=(GLfloat a); 
	Point4f &operator /=(GLfloat a);
	//Print value
	void getPoint4fprint();
	#if 0
	#endif
};
