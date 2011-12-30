// Copyright (C) 2011  Kevin Smith
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
// MA 02110-1301, USA.

#include "Vector3.h"

#include <cstdio>
#include <cmath>

using namespace math;

const Vector3 ZeroVector(0.0f,0.0f,0.0f);

// Set vector components
void Vector3::Set(float ax, float ay, float az)
{
	x = ax;
	y = ay;
	z = az;
}

// Assignment operator
Vector3 &Vector3::operator =(const Vector3 &a)
{
	x = a.x; y = a.y; z = a.z;
	return *this;
}

// Check for equality
bool Vector3::operator ==( const Vector3 &a ) const
{
	return x==a.x && y==a.y && z==a.z;
}

// Check for inequality
bool Vector3::operator !=(const Vector3 &a) const
{
	return x!=a.x || y!=a.y || z!=a.z;
}

// Set zero vector
void Vector3::Zero()
{
	x = y = z = 0.0f;
}

// Unary minus returns the negative of the vector
Vector3 Vector3::operator -() const
{
	return Vector3(-x,-y,-z);
}

// Add vectors
Vector3 Vector3::operator +(const Vector3 &a) const
{
	return Vector3(x + a.x, y + a.y, z + a.z);
}

// Subtract vectors
Vector3 Vector3::operator -(const Vector3 &a) const
{
	return Vector3(x - a.x, y - a.y, z - a.z);
}

// Multiplication by scalar
Vector3 Vector3::operator *(float a) const
{
	return Vector3(x*a, y*a, z*a);
}

// Division by scalar
Vector3 Vector3::operator /(float a) const
{
	float oneOverA = 1.0f / a; // NOTE: no check for divide by zero here
	return Vector3(x*oneOverA, y*oneOverA, z*oneOverA);
}

// Add vectors shorthand
Vector3 &Vector3::operator +=(const Vector3 &a)
{
	x += a.x; y += a.y; z += a.z;
	return *this;
}

// Subtract vectors shorthand
Vector3 &Vector3::operator -=(const Vector3 &a)
{
	x -= a.x; y -= a.y; z -= a.z;
	return *this;
}

// Multiplication by scalar shorthand
Vector3 &Vector3::operator *=(float a)
{
	x *= a; y *= a; z *= a;
	return *this;
}

// Division by scalar shorthand
Vector3 &Vector3::operator /=(float a)
{
	float oneOverA = 1.0f / a;
	x *= oneOverA; y *= oneOverA; z *= oneOverA;
	return *this;
}

// Normalize vector
void Vector3::Normalize()
{
	float magSq = x*x + y*y + z*z;
	if (magSq > 0.0f) { // check for divide-by-zero
		float oneOverMag = 1.0f / sqrt(magSq);
		x *= oneOverMag;
		y *= oneOverMag;
		z *= oneOverMag;
	}
}

// Dot product
float Vector3::operator *(const Vector3 &a) const
{
	return x*a.x + y*a.y + z*a.z;
}

void Vector3::Print()
{
	fprintf(stderr,"(%f,%f,%f)\n",x,y,z);
}

////////////////////////////////////////////////////////////////////////////////
//
// Friend functions
//
////////////////////////////////////////////////////////////////////////////////

std::ostream& operator <<(std::ostream& os, const Vector3& a)
{
	os << "(" << a.x << "," << a.y << "," << a.z << ")";
	return os;
}

////////////////////////////////////////////////////////////////////////////////
//
// Nonmember functions
//
////////////////////////////////////////////////////////////////////////////////

// Length (magnitude)
inline float Length(const Vector3 &a)
{
	return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}

// Length (magnitude) squared
inline float LengthSquared(const Vector3 &a)
{
	return a.x*a.x + a.y*a.y + a.z*a.z;
}

// Cross product
inline Vector3 Cross(const Vector3 &a, const Vector3 &b)
{
	return Vector3(
		a.y*b.z - a.z*b.y,
		a.z*b.x - a.x*b.z,
		a.x*b.y - a.y*b.x
	);
}

// Multiplicatoin of scalar on the left
inline Vector3 math::operator *(float k, const Vector3 &a)
{
	return Vector3(k*a.x, k*a.y, k*a.z);
}

// Distance between two vectors
inline float Distance(const Vector3 &a, const Vector3 &b)
{
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	float dz = a.z - b.z;
	return sqrt(dx*dx + dy*dy + dz*dz);
}

// Distance between two vectors squared
inline float DistanceSquared(const Vector3 &a, const Vector3 &b)
{
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	float dz = a.z - b.z;
	return dx*dx + dy*dy + dz*dz;
}

// Project vector a onto vector bUnit
inline Vector3 Project(const Vector3 &a, const Vector3 &bUnit)
{
	return ((a * bUnit) * bUnit);
}
