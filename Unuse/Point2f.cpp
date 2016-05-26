
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "Point2f.hpp"

// Accessors

GLfloat Point2f::getX() const
{
	return _x;
}

GLfloat Point2f::getY() const
{
	return _y;
}


// Mutators

void Point2f::setX(GLfloat x)
{
	_x = x;
}

void Point2f::setY(GLfloat y)
{
	_y = y;
}

//Constructor

Point2f::Point2f()
{
	_x = 0;
	_y = 0;

}

Point2f::Point2f(GLfloat x, GLfloat y)
{
	_x = x;
	_y = y;

}

//return reference to the lvalue
Point2f& Point2f::operator =(const Point2f &a)
{		_x = a.getX();
		_y = a.getY();
		return *this;
}	

//Check for equality/not equality from other value of Class

bool Point2f::operator ==(const Point2f &a) const
{
	return _x == a.getX() && _y == a.getY();
}

bool Point2f::operator !=(const Point2f &a) const
{
	return _x != a.getX() || _y != a.getY();
}

//Set the vector to zero
void Point2f::zero()
{
	_x = _y = 0;
}

Point2f &Point2f::operator +=(const Point2f &a)
{
		_x += a.getX();
		_y += a.getY();
		return *this;
}

Point2f &Point2f::operator -=(const Point2f &a)
{
		_x -= a.getX();
		_y -= a.getY();
		return *this;
}

Point2f &Point2f::operator *=(GLfloat a)
{
	_x *=a;
	_y *=a;
	return *this;
}


Point2f &Point2f::operator /=(GLfloat a)
{
	_x /=a;
	_y /=a;
	return *this;
}

//Print value

void Point2f::getPoint2fprint()
{
	std::cout << "x: " << _x << " y: " << _y  << std::endl;
}
