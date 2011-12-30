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

#ifndef __MATRIX33_H
#define __MATRIX33_H

#include <iostream>

#include "Vector3.h"

namespace math
{
	class Matrix33
	{
		public:
			//Important: Stored in column major order!
			float m[9];

			Matrix33() {}
			Matrix33( float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22 );
			~Matrix33() {}

			float &operator()( unsigned int row, unsigned int col );
			Vector3 operator*( const Vector3& v ) const;
			Matrix33 operator*( const Matrix33& A ) const;
			Matrix33 operator*( const float k ) const;
			Matrix33 operator+( const Matrix33& A ) const;
			Matrix33 Transpose();
			Matrix33 Identity();
			Matrix33 Inverse();
	};
}
#endif /* !__MATRIX33_H */
