#pragma once
#include <numeric>
#include <math.h>
namespace Orion
{
	class MathHelper
	{
	public:
        static std::pair<float, float> ReduceFraction(float x, float y)
        {
            float d = MathHelper::gcd(x, y);

            float newX = x / d;
            float newY = y / d;

            return  { newX, newY};
        }
       
        static double gcd(double a, double b)
        {
            if (a < b)
                return gcd(b, a);

            // base case
            if (fabs(b) < 0.001)
                return a;

            else
                return (gcd(b, a - floor(a / b) * b));
        }
	};
}

