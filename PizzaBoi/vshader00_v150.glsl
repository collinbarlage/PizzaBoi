#version 150

in  vec4 vPosition;
in  vec3 vNormal;
in  vec4 vColor;
out vec4 color;
uniform mat4 model_matrix;
uniform mat4 camera_matrix;
uniform mat4 proj_matrix;
uniform int  flashlight;

uniform vec4 lightPosSun;
uniform vec4 lightAmbientSun, lightDiffuseSun, lightSpecularSun;

uniform vec4 lightPosFlash;
uniform vec4 lightAmbientFlash, lightDiffuseFlash, lightSpecularFlash;

uniform vec4 matAmbient, matDiffuse, matSpecular;
uniform float matAlpha;

void main() 
{ 
	vec4 flashlightColor = vec4(0,0,0,0);
	
	if(flashlight == 1) {
		vec3 Flashpos = (camera_matrix*model_matrix*vPosition).xyz;
		vec3 FlashlightPosInCam = (camera_matrix*lightPosFlash).xyz;
		vec3 FlashL = normalize(FlashlightPosInCam.xyz - Flashpos);
		float Flashdist = 1.0;

		vec3 FlashE = normalize(vec3(0,0,0)-Flashpos);
		vec3 FlashN = normalize(camera_matrix*model_matrix*vec4(vNormal, 0)).xyz;
		vec3 FlashH = normalize(FlashL+FlashE);

		vec4 Flashambient = lightAmbientFlash*matAmbient;

		float FlashKd = max(dot(FlashL,FlashN),0.0);
		vec4 Flashdiffuse = FlashKd*lightDiffuseFlash*matDiffuse;

		float FlashKs = pow(max(dot(FlashN,FlashH), 0.0), matAlpha);
		vec4 Flashspecular = FlashKs*lightSpecularFlash*matSpecular;
		if(dot(FlashL,FlashN)<0.0)
			Flashspecular = vec4(0,0,0,1);

		flashlightColor = (Flashambient+Flashdiffuse+Flashspecular)*(1/pow(Flashdist,2));
	}


	vec3 pos = (camera_matrix*model_matrix*vPosition).xyz;
	vec3 lightPosInCam = (camera_matrix*lightPosSun).xyz;
	vec3 L = normalize(lightPosInCam.xyz - pos);
	float dist = 1.0;

	vec3 E = normalize(vec3(0,0,0)-pos);
	vec3 N = normalize(camera_matrix*model_matrix*vec4(vNormal, 0)).xyz;
	vec3 H = normalize(L+E);

	vec4 ambient = lightAmbientSun*matAmbient;

	float Kd = max(dot(L,N),0.0);
	vec4 diffuse = Kd*lightDiffuseSun*matDiffuse;

	float Ks = pow(max(dot(N,H), 0.0), matAlpha);
	vec4 specular = Ks*lightSpecularSun*matSpecular;
	if(dot(L,N)<0.0)
		specular = vec4(0,0,0,1);

	color = (ambient+diffuse+specular)*(1/pow(dist,2));
	color = color + flashlightColor;
	color.a = 1.0;


	gl_Position = proj_matrix*camera_matrix*model_matrix*vPosition;
}