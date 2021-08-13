#include <Windows.h>
#include <iostream>
#include "glsl_optimizer.h"
#include <fstream>

void finish()
{
	//system("pause");
	exit(0);
}

bool endsWith(std::string const& value, std::string const& ending)
{
	if (ending.size() > value.size()) return false;
	return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

void addOptimized(std::string& s) 
{
	auto pos = std::find(s.rbegin(), s.rend(), '.');
	auto iter = pos.base();

	s = std::string(s.begin(), iter-1) + "_optimized" + std::string(iter-1, s.end());

};

int WINAPI WinMain(HINSTANCE ,
	HINSTANCE ,
	PSTR pCmdLine, 
	int )
{

	std::string fileName = pCmdLine;
	glslopt_shader_type shaderType = {};

	if (
		endsWith(fileName, ".frag")
		|| endsWith(fileName, ".fragment")
		)
	{
		shaderType = glslopt_shader_type::kGlslOptShaderFragment;
	}
	else if (
		endsWith(fileName, ".vert")
		|| endsWith(fileName, ".vertex")
		)
	{
		shaderType = glslopt_shader_type::kGlslOptShaderVertex;
	}
	else
	{
		finish();
	}


	std::ifstream inFile(pCmdLine);
	if (!inFile.is_open())
	{
		finish();
	}

	std::string shaderSource((std::istreambuf_iterator<char>(inFile)),
		std::istreambuf_iterator<char>());

	inFile.close();

	auto ctx = glslopt_initialize(kGlslTargetOpenGL);

	auto rezult = glslopt_optimize(ctx, shaderType,
		shaderSource.c_str(), 0);

	addOptimized(fileName);
	std::ofstream ofFile(fileName);

	if (glslopt_get_status(rezult))
	{
		auto newSource = glslopt_get_output(rezult);
		ofFile << newSource;
	}
	else
	{
		//auto errLog = glslopt_get_log(rezult);
		//std::cout << "err:\n" << errLog << "\n";
		ofFile << shaderSource;
	}

	ofFile.close();


	finish();

	//glslopt_cleanup(ctx);
	return 0;
}