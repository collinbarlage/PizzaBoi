#version 150

in  vec4 vPosition;
in  vec3 vNormal;
in  vec2 vTexCoord;

out vec2 texCoord;

out vec3 fNSun;
out vec3 fESun;
out vec3 fLSun;

out vec3 fNFlash;
out vec3 fEFlash;
out vec3 fLFlash;

uniform mat4 model_matrix;
uniform mat4 camera_matrix;
uniform mat4 proj_matrix;

uniform vec4 SunlightPos;
uniform vec4 FlashlightPos;

void main() 
{ 
	texCoord = vTexCoord;

	vec3 Flashpos = (camera_matrix*model_matrix*vPosition).xyz;
	vec3 FlashlightPosInCam = (camera_matrix*FlashlightPos).xyz;

	fNFlash = normalize(camera_matrix*model_matrix*vec4(vNormal, 0)).xyz;
	fEFlash = normalize(vec3(0,0,0)-Flashpos);
	fLFlash = normalize(FlashlightPosInCam.xyz - Flashpos);

	vec3 Sunpos = (camera_matrix*model_matrix*vPosition).xyz;
	vec3 SunlightPosInCam = (camera_matrix*SunlightPos).xyz;

	fNSun = normalize(camera_matrix*model_matrix*vec4(vNormal, 0)).xyz;
	fESun = normalize(vec3(0,0,0)-Sunpos);
	fLSun = normalize(SunlightPosInCam.xyz - Sunpos);

	gl_Position = proj_matrix*camera_matrix*model_matrix*vPosition;


}