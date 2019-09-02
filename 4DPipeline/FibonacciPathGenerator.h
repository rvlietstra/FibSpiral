#pragma once
#include "PathGenerator.h"

class FibonacciPathGenerator : public PathGenerator
{
public:
	FibonacciPathGenerator();
	vector<Vertex> Generate() override;
	~FibonacciPathGenerator();

private:
	vector<int> Fib(int numItems);
	vector<vec3> QuaterCircleVertices(vec3 center, float radius, float startRadians, int numSegments);
};

