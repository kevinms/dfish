#ifndef __PARTICLE_H
#define __PARTICLE_H

#include "Object.h"
#include "State.h"

typedef struct StateParticle : State
{
	Vector3d x; // Position
	Vector3d P; // Linear Momentum
} StateParticle;

class Particle : Object
{
	public:
		StateParticle s;
		Vector3d v;

		Particle(Vector3d x, Vector3d P);
		void StateToArray(double *Y);
		void Draw();

	private:
		
};

#endif // !__PARTICLE_H
