#ifndef __OBJECT_H
#define __OBJECT_H

#define OBJECT_PARTICLE  0
#define OBJECT_RIGIDBODY 1

class Object
{
	int type;
	virtual StateToArray(double **Y);
};

#endif // !__OBJECT_H
