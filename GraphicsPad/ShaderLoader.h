#pragma once
#include "string"

struct ShaderSource {
	std::string VertexSource;
	std::string FragmentSource;
};

ShaderSource GetShaderSource(const char* fileName);