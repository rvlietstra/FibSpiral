#pragma once
#include "PathGenerator.h"
class SinPathGenerator : public PathGenerator
{
public:
	SinPathGenerator();
	vector<Vertex> Generate() override;
	~SinPathGenerator();
};

