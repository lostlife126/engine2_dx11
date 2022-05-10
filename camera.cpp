#include "camera.h"
namespace MyEngine
{
	double getAngle(double x, double y)
	{
		double res;
		if (fabs(x) > 1e-10)
			res = atanf(y / x);
		else
			res = XM_PIDIV2;
		if (y < 0.0)
			res = -res;
		if (x < 0.0)
			res = XM_PI - res;
		return res;
	}

}