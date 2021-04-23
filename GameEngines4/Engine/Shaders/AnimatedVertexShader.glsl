/*#version 410 core

layout (location = 0) in  vec3 position;
layout (location = 1) in  vec3 normal;
layout (location = 2) in  vec2 uv;
layout (location = 3) in  vec3 color;

out vec3 Normal; 
out vec2 TexCoords;
out vec3 Color;
out vec3 FragPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	gl_Position = proj * view * model * vec4(position, 1.0f);

	Normal = mat3(transpose(inverse(model))) * normal;
    FragPosition = vec3(model * vec4(position, 1.0f));

	TexCoords = uv;
	Color = color;
}
*/

#version 440 core
layout (location = 0) in  vec3 position;
layout (location = 1) in  vec3 normal;
layout (location = 2) in  vec2 uv;
layout (location = 3) in  vec4 boneIds;
layout (location = 4) in  vec4 boneWeights;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec4 FragPosLightSpace;

uniform mat4 bone_transforms[250];
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 lightSpaceMatrix;

out vec4 bw;

void main()
{
	bw = vec4(0);
	if(int(boneIds.x) == 1)
	bw.z = boneIds.x;

	mat4 boneTransform  =  mat4(0.0);
	boneTransform  +=    bone_transforms[int(boneIds.x)] * boneWeights.x;
	boneTransform  +=    bone_transforms[int(boneIds.y)] * boneWeights.y;
	boneTransform  +=    bone_transforms[int(boneIds.z)] * boneWeights.z;
	boneTransform  +=    bone_transforms[int(boneIds.w)] * boneWeights.w;

	vec4 pos = boneTransform * vec4(position, 1.0);
//
//	FragPos = vec3(model * boneTransform * pos);
//	TexCoords = uv;
//	FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
//	Normal = mat3(transpose(inverse(model * boneTransform))) * normal;
//
//
//	gl_Position = proj * view * model * pos;

	FragPos = vec3(model * vec4(position, 1.0));
    Normal = transpose(inverse(mat3(model))) * normal;
    TexCoords = uv;
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
    gl_Position = proj * view * model * pos;


}


