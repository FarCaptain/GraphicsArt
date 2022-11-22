#include "ShaderLoader.h"

ShaderSource GetShaderSource(const char* fileName)
{
	std::ifstream stream(fileName);

	std::string line;
	std::stringstream str[2];

	int index = 0;
	if (getline(stream, line) && line.find("#shader") == std::string::npos) {
		assert(false);
	}
	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			index++;
		} else {
			str[index] << line << "\n";
		}
	}
	
	return { str[0].str(), str[1].str() };
}
