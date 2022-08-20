#pragma once
#include <numeric>
namespace Orion
{
	class MathHelper
	{
	public:
        static std::pair<uint32_t, uint32_t> ReduceFraction(uint32_t x, uint32_t y)
        {
            int d;
            d = std::gcd(x, y);

            x = x / d;
            y = y / d;

            return  {x, y};
        }
       

	};
}

