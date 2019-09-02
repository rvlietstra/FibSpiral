#include "pch.h"
#include "SinPathGenerator.h"


SinPathGenerator::SinPathGenerator()
{
}

vector<Vertex> SinPathGenerator::Generate()
{
	vector<Vertex> ret;

	int segments = 100;
	float amplitide = 40;
	float delta = (pi<float>() * 2.0f) / (segments - 1);
	float r, g, b;
	for (float x = -pi<float>(); x < pi<float>(); x += delta)
	{
		HSVToRGB(degrees<float>(x + pi<float>()), 1.0f, 1.0f, r, g, b);
		ret.push_back({ x * 20.0f, sinf(x) * amplitide, 0, r, g, b });
	}

	return ret;
}

SinPathGenerator::~SinPathGenerator()
{
}
