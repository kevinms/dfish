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

#include "Matrix33.h"

using namespace math;

Matrix33::Matrix33( float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22 )
{
	m[0] = m00;
	m[3] = m01;
	m[6] = m02;
	m[1] = m10;
	m[4] = m11;
	m[7] = m12;
	m[2] = m20;
	m[5] = m21;
	m[8] = m22;
}

float &Matrix33::operator()(unsigned int row, unsigned int col)
{
	return m[row + 3*col];
}

Vector3 Matrix33::operator*( const Vector3& v ) const
{
	Vector3 result;
	result.x = m[0]*v.x + m[3]*v.y + m[6]*v.z;
	result.y = m[1]*v.x + m[4]*v.y + m[7]*v.z;
	result.z = m[2]*v.x + m[5]*v.y + m[8]*v.z;
	return result;
}

Matrix33 Matrix33::operator*( const Matrix33& A ) const
{
	Matrix33 result;
	result.m[0] = m[0]*A.m[0] + m[3]*A.m[1] + m[6]*A.m[2];
	result.m[1] = m[1]*A.m[0] + m[4]*A.m[1] + m[7]*A.m[2];
	result.m[2] = m[2]*A.m[0] + m[5]*A.m[1] + m[8]*A.m[2];
	result.m[3] = m[0]*A.m[3] + m[3]*A.m[4] + m[6]*A.m[5];
	result.m[4] = m[1]*A.m[3] + m[4]*A.m[4] + m[7]*A.m[5];
	result.m[5] = m[2]*A.m[3] + m[5]*A.m[4] + m[8]*A.m[5];
	result.m[6] = m[0]*A.m[6] + m[3]*A.m[7] + m[6]*A.m[8];
	result.m[7] = m[1]*A.m[6] + m[4]*A.m[7] + m[7]*A.m[8];
	result.m[8] = m[2]*A.m[6] + m[5]*A.m[7] + m[8]*A.m[8];
	return result;
}

Matrix33 Matrix33::operator*( const float k ) const
{
	Matrix33 result;
	for(int i = 0; i < 9; i++)
	{
		result.m[i] = m[i] * k;
	}
	return result;
}

Matrix33 Matrix33::operator+( const Matrix33& A ) const
{
	Matrix33 result;
	for (int i = 0; i < 9; i++)
	{
		result.m[i] = m[i] + A.m[i];
	}
	return result;
}

Matrix33 Matrix33::Transpose()
{
	Matrix33 result;
	result.m[0] = m[0];
	result.m[1] = m[3];
	result.m[2] = m[6];
	result.m[3] = m[1];
	result.m[4] = m[4];
	result.m[5] = m[7];
	result.m[6] = m[2];
	result.m[7] = m[5];
	result.m[8] = m[8];
	return result;
}

Matrix33 Matrix33::Identity()
{
	Matrix33 result;
	result.m[0] = 1; result.m[1] = 0; result.m[2] = 0;
	result.m[0] = 0; result.m[1] = 1; result.m[2] = 0;
	result.m[0] = 0; result.m[1] = 0; result.m[2] = 1;
	return result;
}

Matrix33 Matrix33::Inverse()
{
	return Matrix33();
}

////////////////////////////////////////////////////////////////////////////////
//
// Nonmember functions
//
////////////////////////////////////////////////////////////////////////////////


