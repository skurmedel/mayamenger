#include "mengerfun.hpp"

#define foreach_cube(n) for (vcube_it it = (n).begin(); it != (n).end(); it++)

typedef std::vector<cube>::iterator vcube_it;

dice_err::val dice(cube const &c, unsigned int iterations, std::vector<cube> &out)
{
	if (!c.valid())
	{
		return dice_err::bounds_err;
	}

	std::vector<cube> tmp;
	if (iterations != 0)
	{
		foreach_cube(out) 
		{
			/* Diagonal of the cube, scaled to 33%. */
			point3 dia = it->diagonal() / 3.0f;
			float bx = dia.x, by = dia.y, bz = dia.z;

			for (int z = 0; z < 3; z++)
			{
				cube newc;
				for (int y = 0; y < 3; y++)
				{
					for (int x = 0; x < 3; x++)
					{
						newc.start.x = x * bx;
						newc.start.y = y * by;
						newc.start.z = z * by;

						newc.end.x = (x + 1) * bx;
						newc.end.y = (y + 1) * by;
						newc.end.z = (z + 1) * by;

						tmp.push_back(newc);
					}
				}
			}
		}

		out.clear();

		foreach_cube(tmp) 
		{
			return dice(*it, iterations - 1, out);
		}
	} 

	foreach_cube(tmp)
	{
		out.push_back(*it);
	}

	return dice_err::success;
}