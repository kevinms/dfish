#ifndef __STATEVECTOR_H
#define __STATEVECTOR_H

class StateVector
{
	public:
		double *v;
		StateVector() {};

		friend StateVector operator*(const StateVector &v, double s); // scalar * q
		friend StateVector operator*(double s, const StateVector &v); // q * scalar

		friend StateVector operator+(const StateVector &v, double f); // factor + q
		friend StateVector operator+(double f, const StateVector &v); // q + factor
};

#define __STATEVECTOR_H
