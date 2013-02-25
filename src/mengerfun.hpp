#ifndef  MENGERFUN_HPP
#define MENGERFUN_HPP

#include <vector>

struct point3
{
	float x, y, z;

	bool equal(point3 const &o) const
	{
		return x == o.x
		     && y == o.y
		     && z == o.z;
	}

	point3 between(point3 const &o) const
	{
		point3 p;
		p.x = x - o.x;
		p.y = y - o.y;
		p.z = z - o.z;
		return p;
	}

	point3 operator/(float scalar)
	{
		point3 p;
		p.x = x / scalar;
		p.y = y / scalar;
		p.z = z / scalar;
		return p;
	}

	point3 operator*(float scalar)
	{
		point3 p;
		p.x = x * scalar;
		p.y = y * scalar;
		p.z = z * scalar;
		return p;
	}

	point3 operator+(point3 const &o)
	{
		point3 p;
		p.x = x + o.x;
		p.y = y + o.y;
		p.z = z + o.z;
		return p;
	}
};

struct cube
{
	point3 start;
	point3 end;

	bool valid() const
	{
		return !start.equal(end);
	}

	point3 diagonal() const
	{
		return end.between(start);
	}
};

namespace dice_err
{
	enum val
	{ 
		invalid_param_err,
		bounds_err,
		memory_err,
		success
	};
}

/*
	Dice a cube (defined by s and diagonal endpoint e) into a menger sponge.

	The menger sponge is made up of the cubes written to the out vector.
*/
dice_err::val dice(cube const &c, unsigned int iterations, std::vector<cube> &out);

#endif