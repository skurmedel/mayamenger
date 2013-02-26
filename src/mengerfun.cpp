#include "mengerfun.hpp"

#include <maya\MGlobal.h>

#define foreach_cube(n) for (vcube_it it = (n).begin(); it != (n).end(); it++)

typedef std::vector<cube>::iterator vcube_it;

dice_err::val dice_impl(unsigned int iterations, std::vector<cube> &out)
{
	if (iterations < 1)
		return dice_err::success;

	std::vector<cube> tmp;
	foreach_cube(out) 
	{
		/* Diagonal of the cube, scaled to 33%. */
		point3 dia = it->diagonal();
		point3 incr = dia * 0.33333333f;
		
		for (int z = 0; z < 3; z++)
		{
			for (int y = 0; y < 3; y++)
			{
				for (int x = 0; x < 3; x++)
				{
					if ((y == 0 || y == 2) && x == 1 && z == 1) 
					{ 
						/* do nothing */ 
					}
					else if (y == 1 && (x == 1 || z == 1)) 
					{ 
						/* zzz */ 
					}
					else
					{
						cube newc;

						newc.start = it->start + point3(x * incr.x, y * incr.y, z * incr.z);

						newc.end = newc.start + incr;

						tmp.push_back(newc);
					}
				}
			}
		}
	}

	out.clear();

	foreach_cube(tmp) 
	{
		out.push_back(*it);
	}

	tmp.clear();

	return dice_impl(iterations - 1, out);
}

dice_err::val dice(cube const &c, unsigned int iterations, std::vector<cube> &out)
{
	if (!c.valid())
	{
		return dice_err::bounds_err;
	}

	out.push_back(c);

	return dice_impl(iterations, out);
}