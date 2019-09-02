#include "pch.h"
#include "PathGenerator.h"

PathGenerator::PathGenerator()
{
}

vec3 PathGenerator::PointOnCircle(vec3 center, float radius, float radians)
{
	return vec3(cosf(radians) * radius, sinf(radians) * radius, 0) + center;
}

void PathGenerator::HSVToRGB(float h, float s, float v, float &r, float &g, float &b)
{
	r = 0;
	g = 0;
	b = 0;

	if (s == 0)
	{
		r = v;
		g = v;
		b = v;
		return;
	}

	int i;
	double f, p, q, t;

	if (h == 360)
		h = 0;
	else
		h = h / 60;

	i = (int)trunc(h);
	f = h - i;

	p = v * (1.0 - s);
	q = v * (1.0 - (s * f));
	t = v * (1.0 - (s * (1.0 - f)));

	switch (i)
	{
	case 0:
		r = v;
		g = t;
		b = p;
		break;

	case 1:
		r = q;
		g = v;
		b = p;
		break;

	case 2:
		r = p;
		g = v;
		b = t;
		break;

	case 3:
		r = p;
		g = q;
		b = v;
		break;

	case 4:
		r = t;
		g = p;
		b = v;
		break;

	default:
		r = v;
		g = p;
		b = q;
		break;
	}
}

vector<Vertex> PathGenerator::Generate()
{
	return vector<Vertex>();
}

PathGenerator::~PathGenerator()
{
}
