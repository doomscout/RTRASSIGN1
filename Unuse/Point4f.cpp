
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "Point4f.hpp"

// Accessors

GLfloat Point4f::getX() const
{
	return _x;
}

GLfloat Point4f::getY() const
{
	return _y;
}

GLfloat Point4f::getZ() const
{
	return _z;
}

GLfloat Point4f::getW() const
{
	return _w;
}

// Mutators

void Point4f::setX(GLfloat x)
{
	_x = x;
}

void Point4f::setY(GLfloat y)
{
	_y = y;
}

void Point4f::setZ(GLfloat z)
{
	_z = z;
}

void Point4f::setW(GLfloat w)
{
	_w = w;
}

//Constructor

Point4f::Point4f()
{
	_x = 0;
	_y = 0;
	_z = 0;
	_w = 0;
}

Point4f::Point4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
	_x = x;
	_y = y;
	_z = z;
	_w = w;
}

//return reference to the lvalue
Point4f& Point4f::operator =(const Point4f &a)
{		_x = a.getX();
		_y = a.getY();
		_z = a.getZ();
		_w = a.getW();
		return *this;
}	

//Check for equality/not equality from other value of Class

bool Point4f::operator ==(const Point4f &a) const
{
	return _x == a.getX() && _y == a.getY() && _z == a.getZ() && _w == a.getW();
}

bool Point4f::operator !=(const Point4f &a) const
{
	return _x != a.getX() || _y != a.getY() || _z != a.getZ() || _w == a.getW();
}

//Set the vector to zero
void Point4f::zero()
{
	_x = _y = _z = _w = 0;
}

Point4f &Point4f::operator +=(const Point4f &a)
{
		_x += a.getX();
		_y += a.getY();
		_z += a.getZ();
		_w += a.getW();
		return *this;
}

Point4f &Point4f::operator -=(const Point4f &a)
{
		_x -= a.getX();
		_y -= a.getY();
		_z -= a.getZ();
		_w -= a.getW();
		return *this;
}

Point4f &Point4f::operator *=(GLfloat a)
{
	_x *=a;
	_y *=a;
	_z *=a;
	_w *=a;
	return *this;
}


Point4f &Point4f::operator /=(GLfloat a)
{
	_x /=a;
	_y /=a;
	_z /=a;
	_w /=a;
	return *this;
}

//Print value

void Point4f::getPoint4fprint()
{
	std::cout << "x: " << _x << " y: " << _y << " z: " << _z << " w: " << _w << std::endl;
}
#if 0
#endif 
