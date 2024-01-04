#version 420
//Written by Scott Nykl. Default Shader for a Core 3.2+.
//Performs per fragment ambient/diffuse/specular shading with material
//properties, textures, and lights.

layout ( binding = 0 ) uniform sampler2D TexUnit0;
layout ( binding = 7 ) uniform sampler2DShadow ShadowMap;

in vec3 VertexES;

struct MaterialInfo
{
   vec4 Ka; //Ambient
   vec4 Kd; //Diffuse
   vec4 Ks; //Specular
   float SpecularCoeff; // Specular Coefficient
};
uniform MaterialInfo Material;


layout ( binding = 1, std140 ) uniform LightInfo
{
   vec4 PosEye[8]; // Light's Eye space position (same as view space)
   vec4 Irgba[8]; // Light's Intensity for red, green, blue reflectivity components
   vec4 GlobalAmbient;
   int NumLights; // Number of lights in the LightInfo array   
} Lights;

layout ( location = 0 ) out vec4 FragColor;

vec3 density(vec3 samplePosition)
{
	vec3 extents = vec3(50,100,150);//uniform
	vec3 max = extents + wPos;
	vec3 coord = vec3( ( max.x - samplePosition.x) / extents.x ),
					   ( max.y - samplePosition.y) / extents.y ),
					   ( max.z - samplePosition.z) / extents.z ));
	texture(TexUnit0,coord);
}

vec4 raymarch(vec3 samplePosition, vec3 marchDirection, int stepCount, float stepSize) {

    float transmittance = 1.0;

    for (int i = 0; i < stepCount; i++) {
        samplePosition += marchDirection * stepSize;
        transmittance *= exp(-density(samplePosition) * extinctionCoef * stepSize);
    )

    return vec4(transmittance,transmittance,transmittance transmittance);
}

void main()
{
   vec3 samplePosition = VertexES;
   vec3 marchDirection = normalize(samplePosition);//since we are in eye space
   int stepCount = 10;//make uniform
   float stepSize = 1;//make uniform

   vec4 color = raymarch(samplePosition, marchDirection, stepCount, stepSize);
   FragColor = color;
}