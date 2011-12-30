#include "Particle.h"

void Particle::StateToArray(double **Y)
{
	*(*Y)++ = x[0];
	*(*Y)++ = x[1];
	*(*Y)++ = x[2];

	*(*Y)++ = P[0];
	*(*Y)++ = P[1];
	*(*Y)++ = P[2];
}

void Particle::ArrayToState(double **Y)
{
	x[0] = *(*Y)++;
	x[1] = *(*Y)++;
	x[2] = *(*Y)++;

	P[0] = *(*Y)++;
	P[1] = *(*Y)++;
	P[2] = *(*Y)++;

	// Compute auxiliary variables...
	v = s.P / mass;
}

void RigidBody::ddt_StateToArray(double **Ydot)
{
	*(*Ydot)++ = v[0];
	*(*Ydot)++ = v[1];
	*(*Ydot)++ = v[2];

	*(*Ydot)++ = force[0];
	*(*Ydot)++ = force[1];
	*(*Ydot)++ = force[2];
}
