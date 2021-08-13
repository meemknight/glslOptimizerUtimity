#include <Windows.h>
#include <iostream>
#include "glsl_optimizer.h"


const char* shader =
R"(
#version 150 core
out vec4 FragColor;
in vec3 v_localPos;
//https://learnopengl.com/PBR/IBL/Diffuse-irradiance

uniform sampler2D u_equirectangularMap;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
	vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
	uv *= invAtan;
	uv += 0.5;
	return uv;
}

void main()
{		
	vec2 uv = SampleSphericalMap(normalize(v_localPos)); // make sure to normalize localPos
	vec3 color = texture(u_equirectangularMap, uv).rgb;
	
	FragColor = vec4(color, 1.0);
}
)";

void finish()
{
	system("pause");
	exit(0);
}


int main()
{
	auto ctx = glslopt_initialize(kGlslTargetOpenGL);

	auto rezult = glslopt_optimize(ctx, glslopt_shader_type::kGlslOptShaderFragment,
		shader, 0);

	if (glslopt_get_status(rezult))
	{
		auto newSource = glslopt_get_output(rezult);
		std::cout << "newSource:\n\n" << newSource << "\n";
	}
	else
	{
		auto errLog = glslopt_get_log(rezult);
		std::cout << "err:\n" << errLog << "\n";
		finish();
	}




	finish();

	//glslopt_cleanup(ctx);
	return 0;
}