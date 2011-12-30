#include "RigidBody.h"

void RigidBody::StateToStateVector(double **Y)
{
	*(*Y)++ = x[0];
	*(*Y)++ = x[1];
	*(*Y)++ = x[2];

	*(*Y)++ = P[0];
	*(*Y)++ = P[1];
	*(*Y)++ = P[2];

	*(*Y)++ = q[0];
	*(*Y)++ = q[1];
	*(*Y)++ = q[2];
	*(*Y)++ = q[3];

	*(*Y)++ = L[0];
	*(*Y)++ = L[1];
	*(*Y)++ = L[2];
}

void RigidBody::StateVectorToState(StateVector **Y)
{
	x[0] = *(*Y)++;
	x[1] = *(*Y)++;
	x[2] = *(*Y)++;

	P[0] = *(*Y)++;
	P[1] = *(*Y)++;
	P[2] = *(*Y)++;

	q[0] = *(*Y)++;
	q[1] = *(*Y)++;
	q[2] = *(*Y)++;
	q[3] = *(*Y)++;

	L[0] = *(*Y)++;
	L[1] = *(*Y)++;
	L[2] = *(*Y)++;

	// Compute auxiliary variables...
	v = s.P / mass;
	Iinv = R * Ibodyinv * R.transpose();
	omega = Iinv * s.L;
	R = Q.rotation();
}

void RigidBody::ddt_StateToStateVector(StateVector **Ydot)
{
	*(*Ydot)++ = v[0];
	*(*Ydot)++ = v[1];
	*(*Ydot)++ = v[2];

	*(*Ydot)++ = force[0];
	*(*Ydot)++ = force[1];
	*(*Ydot)++ = force[2];

	Quaternion qdot = .5 * (omega * s.q);
	*(*ydot)++ = qdot[0];
	*(*ydot)++ = qdot[1];
	*(*ydot)++ = qdot[2];
	*(*ydot)++ = qdot[3];

	*(*Ydot)++ = torque[0];
	*(*Ydot)++ = torque[1];
	*(*Ydot)++ = torque[2];
}
