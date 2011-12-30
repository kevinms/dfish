#ifndef __SPRING_H
#define __SPRING_H

class Spring : Object
{
	public:
		double k; // Spring Constant
		double d; // Damping Constant
		double r; // Rest Length

		Spring(double k, double d);
		void Draw();
}

#endif // !__SPRING_H
