#pragma once
#include "PathGenerator.h"
class CirclePathGenerator : public PathGenerator
{
public:
	CirclePathGenerator(float radius, int segments);
	vector<Vertex> Generate() override;
	~CirclePathGenerator();
private:
	float radius_;
	int segments_;
};

