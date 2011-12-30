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

#ifndef __VECTOR3_H
#define __VECTOR3_H

#include <iostream>

namespace math
{
	class Vector3
	{
		public:
			float x,y,z;

			Vector3() {}
			Vector3(const Vector3& a) : x(a.x), y(a.y), z(a.z) {}
			Vector3(float ax, float ay, float az) : x(ax), y(ay), z(az) {}
			~Vector3() {}
			void Set(float ax, float ay, float az);

			Vector3 &operator =(const Vector3 &a);
			bool operator ==( const Vector3 &a ) const;
			bool operator !=(const Vector3 &a) const;
			void Zero();
			Vector3 operator -() const;
			Vector3 operator +(const Vector3 &a) const;
			Vector3 operator -(const Vector3 &a) const;
			Vector3 operator *(float a) const;
			Vector3 operator /(float a) const;
			Vector3 &operator +=(const Vector3 &a);
			Vector3 &operator -=(const Vector3 &a);
			Vector3 &operator *=(float a);
			Vector3 &operator /=(float a);
			void Normalize();
			float operator *(const Vector3 &a) const; // Dot product
			void Print();
			friend std::ostream& operator <<(std::ostream& os, const Vector3& a);
	};

	inline float Length(const Vector3 &a); // Length
	inline float LengthSquared(const Vector3 &a); // Length squared
	inline Vector3 Cross(const Vector3 &a, const Vector3 &b); // Cross product
	inline Vector3 operator *(float k, const Vector3 &v);
	inline float Distance(const Vector3 &a, const Vector3 &b); // Distance
	inline float DistanceSquared(const Vector3 &a, const Vector3 &b); // Distance squared
	inline Vector3 Project(const Vector3 &a, const Vector3 &bUnit); // Project

	extern const Vector3 ZeroVector;
}
#endif /* !__VECTOR3_H */
