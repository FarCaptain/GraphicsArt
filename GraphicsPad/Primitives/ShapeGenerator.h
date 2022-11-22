#pragma once
#include <Primitives\ShapeData.h>
typedef unsigned int uint;

class ShapeGenerator
{
public:
	static ShapeData makeTriangle();
	static ShapeData makeCube();
	static ShapeData makePlane(uint dimensions);
	static ShapeData makePlaneVerts(uint dimensions);
	static ShapeData makePlaneIndices(uint dimensions);
};

