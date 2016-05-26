#pragma once
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
class Point2f
{
private:
	GLfloat _x;
	GLfloat _y;


public:
	// Accessors
	GLfloat getX() const;
	GLfloat getY() const;


	// Mutators
	void setX(GLfloat x);
	void setY(GLfloat y);
	
	
	//Constructor
	Point2f();
	Point2f(GLfloat x, GLfloat y);
	
	//Copy Constructor
	Point2f(const Point2f &a) : _x(a.getX()), _y(a.getY()){}
	
	
	//return reference to the lvalue
	Point2f &operator =(const Point2f &a);
	
	//Check for equality from other value of Class
	bool operator ==(const Point2f &a) const;
	bool operator !=(const Point2f &a) const;
	
	//Set the vector to zero
	
	void zero();
	
	
	// Unary minus returns the negative of the vector 
	
	Point2f operator -() const { return Point2f(-_x,-_y);}
	
	//Operator Overload for calucation vector3f
	
	Point2f operator + (const Point2f &a) const
	{
		return Point2f( _x + a.getX(), _y + a.getY());
	}
	
	Point2f operator - (const Point2f &a) const
	{
		return Point2f( _x - a.getX(), _y - a.getY());
	}
	
	// Multipilication and division by scalar
	Point2f operator * (GLfloat a) const
	{
		return Point2f( _x * a, _y * a);
	}

	//Divide by GLfloat for Point2f 
	Point2f operator / (GLfloat a) const
	{
		GLfloat oneOverA = 1.0f/a;
		
		return Point2f( _x * oneOverA, _y * oneOverA);
	}
	
	Point2f &operator +=(const Point2f &a);
	Point2f &operator -=(const Point2f &a);
	Point2f &operator *=(GLfloat a); 
	Point2f &operator /=(GLfloat a);
	//Print value
	void getPoint2fprint();
};
