#pragma once
#include <vector>
#include "Vertex.h"
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

using namespace std;
using namespace glm;

class PathGenerator
{
public:
	PathGenerator();
	virtual vector<Vertex> Generate();
	~PathGenerator();
protected:
	vec3 PointOnCircle(vec3 center, float radius, float radians);
	void HSVToRGB(float h, float s, float v, float &r, float &g, float &b);
};

