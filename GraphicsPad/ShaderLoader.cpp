#include "ShaderLoader.h"
#include <fstream>
#include <string>
#include <cassert>
#include <sstream>

ShaderSource GetShaderSource(const char* fileName)
{
	std::ifstream stream(fileName);

	std::string line;
	std::stringstream ss[2];

	int currentSource = 0;
	if (getline(stream, line) && line.find("#shader") == std::string::npos) {
		// The first line should start with #shader
		assert(false);
	}
	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			currentSource++;
		} else {
			//std::cout << "Line = " <<currentSource << "  " << line << std::endl;
			ss[currentSource] << line << "\n";
		}
	}
	
	return { ss[0].str(), ss[1].str() };
}
