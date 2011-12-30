#include "Scene.h"

void Scene::ObjectsToStateVector(StateVector *Y)
{
	for(list<Object *>::iterator it = objs.begin(); it != objs.end(); it++)
		(*it)->StateToStateVector(Y);
}

void Scene::StateVectorToObjects(StateVector *Y)
{
	for(list<Object *>::iterator it = objs.begin(); it != objs.end(); it++)
		(*it)->StateToStateVector(Y);
}

void Scene::ComputeForceAndTorque(double dt, Object &o)
{
	
}

/*
void Scene::ode(double dt, double *Y, double *Yfinal)
{
	Derivative a = evaluate(state, t, 0.0f, Derivative());
	Derivative b = evaluate(state, t+dt*0.5f, dt*0.5f, a);
	Derivative c = evaluate(state, t+dt*0.5f, dt*0.5f, b);
	Derivative d = evaluate(state, t+dt, dt, c);

	const float dxdt = 1.0f/6.0f * (a.dx + 2.0f*(b.dx + c.dx) + d.dx);
	const float dvdt = 1.0f/6.0f * (a.dv + 2.0f*(b.dv + c.dv) + d.dv)

	state.x = state.x + dxdt * dt;
	state.v = state.v + dvdt * dt;
}

Derivative evaluate(const State &initial, float t, float dt, const Derivative &d)
{
	State state;
	state.x = initial.x + d.dx*dt;
	state.v = initial.v + d.dv*dt;

	Derivative output;
	output.dx = state.v;
	output.dv = acceleration(state, t+dt);
	return output;
}

void Strut::dotimestep(double dt){
  double dxdt, dvdt;
  double oldx, oldv;
  double k1x, k1v, k2x, k2v, k3x, k3v, k4x, k4v;

  oldx = x; oldv = v;
  derivs(dxdt, dvdt);
  k1x = dxdt * dt; k1v = dvdt * dt;

  x = oldx + 0.5 * k1x;
  v = oldv + 0.5 * k1v;
  derivs(dxdt, dvdt);
  k2x = dxdt * dt; k2v = dvdt * dt;

  x = oldx + 0.5 * k2x;
  v = oldv + 0.5 * k2v;
  derivs(dxdt, dvdt);
  k3x = dxdt * dt; k3v = dvdt * dt;

  x = oldx + k3x;
  v = oldv + k3v;
  derivs(dxdt, dvdt);
  k4x = dxdt * dt; k4v = dvdt * dt;

  x = oldx + (k1x + 2 * k2x + 2 * k3x + k4x) / 6;
  v = oldv + (k1v + 2 * k2v + 2 * k3v + k4v) / 6;
}
*/

void Scene::ode()
{
	dydt(dt,Y,Ydot);
	K1 = Ydot * dt;

	Ydot = Y + 0.5 * K1;
	dydt(dt,Ydot,Ydot);
	K2 = Ydot * dt;

	Ydot = Y + 0.5 * K2;
	dydt(dt,Ydot,Ydot);
	K3 = Ydot * dt;

	Ydot = Y + K3;
	dydt(dt,Ydot,Ydot);
	K4 = Ydot * dt;

	Y = Y + 1.0f/6.0f * (K1 + 2 * (K2 + K3) + K4);
}

// Parameters:
//   Y: Initial state
//   X: Derivative
//   Ydot: Derivative of Y
void Scene::dydt(double dt, StateVector *Y, StateVector *Ydot)
{
	ArrayToObjects(Y);

	for(list<Object *>::iterator it = objs.begin(); it != objs.end(); it++)
	{
		ComputeForceAndTorque(dt, *it);
		(*it)->ddt_State_to_Array(Ydot);
	}
}
