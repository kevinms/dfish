#ifndef __RIGIDBODY_H
#define __RIGIDBODY_H

#include "Vector.h"
#include "Matrix.h"
#include "Quaternion.h"

typedef struct StateRigidBody : State
{
	Vector3d x;   // Position
	Quaternion q; // Orientation
	Vector3d P;   // Linear Momentum
	Vector3d L;   // Angular Momentum
} StateRigidBody;

class RigidBody : Object
{
	public:
		// Constant quantities
		double mass;
		Matrix3x3 Ibody;
		Matrix3x3 Ibodyinv;

		// State Variables
		StateRigidBody s;

		// Derived quantities (auxiliary variables)
		Matrix3x3 Iinv;
		Matrix3x3 R;
		Vector3d v;
		Vector3d omega;

		// Computed Quantities
		Vector3d force;
		Vector3d torque;
};

typedef struct Contact
{
	RigidBody *a; // body containing vertex
	RigidBody *b; // body containing face
	Vector3d p;   // world space vertex location
	Vector3d n;   // outwards pointing normal of face
	Vector3d ea;  // edge direction for A
	Vector3d eb;  // edge direction for B
	bool vf;      // true if vertex/face contact
} Contact;

#endif // !__RIGIDBODY_H
