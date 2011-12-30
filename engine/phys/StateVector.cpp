#include "StateVector.h"

StateVector operator+(const StateVector &v, double f)
{
	StateVector newv(v.n);

	for(int i = 0; i < n; i++)
		*(newv.v+i) = f + *(v.v+i);

	return newv;
}

StateVector operator+(double f, const StateVector &v)
{
	StateVector newv(v.n);

	for(int i = 0; i < n; i++)
		*(newv.v+i) = f + *(v.v+i);

	return newv;
}

StateVector operator*(const StateVector &v, double s)
{
	StateVector newv(v.n);

	for(int i = 0; i < n; i++)
		*(newv.v+i) = s * *(v.v+i);

	return newv;
}

StateVector operator*(double s, const StateVector &v)
{
	StateVector newv(v.n);

	for(int i = 0; i < n; i++)
		*(newv.v+i) = s * *(v.v+i);

	return newv;
}
