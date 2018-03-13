#version 150


uniform vec4 SunlightAmbient, SunlightDiffuse, SunlightSpecular;
in vec3  fNSun;
in vec3  fLSun;
in vec3  fESun;

uniform vec4 FlashlightAmbient, FlashlightDiffuse, FlashlightSpecular;
uniform int  flashlight;
in vec3  fNFlash;
in vec3  fLFlash;
in vec3  fEFlash;

uniform vec4 matAmbient, matDiffuse, matSpecular;
uniform float matAlpha;

out vec4 fColor;

void main() 
{ 

	vec4 flashlightColor = vec4(0,0,0,0);

	if(flashlight == 1) {
		vec3 FlashN = normalize(fNFlash);
		vec3 FlashE = normalize(fEFlash);
		vec3 FlashL = normalize(fLFlash);

		vec3 FlashH = normalize(FlashL+FlashE);

		vec4 Flashambient = FlashlightAmbient;

		float FlashKd = max(dot(FlashL,FlashN),0.0);
		vec4 Flashdiffuse = FlashKd*FlashlightDiffuse;

		float FlashKs =  pow(max(dot(FlashN,FlashH), 0.0), matAlpha);
		vec4 Flashspec = FlashKs*FlashlightSpecular;
		if(dot(FlashL,FlashN)<0.0)
			Flashspec = vec4(0,0,0,1);

		flashlightColor = Flashambient+Flashdiffuse+Flashspec;
	}

	vec3 SunN = normalize(fNSun);
	vec3 SunE = normalize(fESun);
	vec3 SunL = normalize(fLSun);

	vec3 SunH = normalize(SunL+SunE);

	vec4 Sunambient = SunlightAmbient;

	float SunKd = max(dot(SunL,SunN),0.0);
	vec4 Sundiffuse = SunKd*SunlightDiffuse;

	float SunKs = pow(max(dot(SunN,SunH), 0.0), matAlpha);
	vec4 Sunspec = SunKs*SunlightSpecular;
	if(dot(SunL,SunN)<0.0)
		Sunspec = vec4(0,0,0,1);

	fColor = Sunambient+Sundiffuse+Sunspec;
	fColor = fColor + flashlightColor;
	fColor.a = 1.0;
} 

