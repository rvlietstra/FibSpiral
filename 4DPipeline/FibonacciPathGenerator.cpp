#include "pch.h"
#include "FibonacciPathGenerator.h"

FibonacciPathGenerator::FibonacciPathGenerator()
{
}

vector<int> FibonacciPathGenerator::Fib(int numItems)
{
	vector<int> ret;
	int current = 0;
	int previous = 1;
	ret.push_back(0);
	for (int n = 0; n < numItems - 1; n++)
	{
		int p = current;
		current = previous + current;
		ret.push_back(current);
		previous = p;
	}
	return ret;
}

vector<vec3> FibonacciPathGenerator::QuaterCircleVertices(vec3 center, float radius, float startRadians, int numSegments)
{
	vector<vec3> points;

	float delta = (pi<float>() / 2.0f) / (float)numSegments;	

	float currentRadians = startRadians;
	for (int iSegment = 0; iSegment < numSegments; iSegment++)
	{
		points.push_back(PointOnCircle(center, radius, currentRadians));
		currentRadians += delta;
	}

	return points;
}

vector<Vertex> FibonacciPathGenerator::Generate()
{
	int numFibs = 14;
	int segmentsPerQuater = 10;
	vector<int> fibs = Fib(numFibs);
	
	//
	vector<Vertex> vertices;
	float startRadians = 0;
	vec3 center(0);
	vec3 lastPointOnCircle(0, 0, 0);

	// Calculate the quater circle vertices for each of the fibonacci 
	for (int iFib = 1; iFib < numFibs; iFib++)
	{
		int fib = fibs[iFib];

		// Shift the center
		int quadrant = iFib % 4;
		if (quadrant == 0)
			center = lastPointOnCircle + vec3(0, fib, 0);
		if (quadrant == 1)
			center = lastPointOnCircle - vec3(fib, 0, 0);
		if (quadrant == 2)
			center = lastPointOnCircle - vec3(0, fib, 0);
		if (quadrant == 3)
			center = lastPointOnCircle + vec3(fib, 0, 0);

		// 
		vector<vec3> circleVertices = QuaterCircleVertices(center, (float)fib, startRadians, segmentsPerQuater);
		for (auto v : circleVertices)
			vertices.push_back({ v.x, v.y, v.z, (float)(fib % 2), 0.5f, 1.0f - (float)(fib % 2) });

		//
		lastPointOnCircle = PointOnCircle(center, (float)fib, startRadians + (pi<float>() / 2.0f));
		startRadians = iFib * (pi<float>() / 2.0f);
	}

	return vertices;
}

FibonacciPathGenerator::~FibonacciPathGenerator()
{

}
