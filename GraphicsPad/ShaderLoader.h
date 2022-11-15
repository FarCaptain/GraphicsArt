#pragma once
#include "string"
#include <fstream>
#include <cassert>
#include <sstream>

struct ShaderSource {
	std::string VertexSource;
	std::string FragmentSource;
};

ShaderSource GetShaderSource(const char* fileName);