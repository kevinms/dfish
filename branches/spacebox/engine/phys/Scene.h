#ifndef __SCENE_H
#define __SCENE_H

#include <Vector>

class Scene
{
	private:
		double *Y;
		double *rk1;
		double *rk2;
		double *rk3;
		double *rk4;

	public:
		Scene();
		void Simulate();
		void ode(y0,yfinal,STATE*SIZE*NBODIES,t,t+dt,dydt);
		void dydt();
		void ObjectsToArray(double *Y);
		void ArrayToObjects(double *Y);
}

#endif // !__SCENE_H
