#include "pch.h"
#include "CirclePathGenerator.h"


CirclePathGenerator::CirclePathGenerator(float radius, int segments)
{
	radius_ = radius;
	segments_ = segments;
}

vector<Vertex> CirclePathGenerator::Generate()
{
	vector<Vertex> ret;

	float delta = (pi<float>() * 2.0f) / (segments_ - 1);
	float radians = 0;
	float r, g, b;
	for (int iSegment = 0; iSegment < segments_ + 2; iSegment++) // +2 for adjacency requirements
	{
		vec3 p = PointOnCircle(vec3(0), radius_, radians);
		HSVToRGB(degrees<float>(radians), 1.0f, 1.0f, r, g, b);
		ret.push_back({ p.x, p.y, sinf(radians * 2.0f) * 2.0f, r, g, b });
		radians += delta;
	}

	return ret;
}

CirclePathGenerator::~CirclePathGenerator()
{
}
