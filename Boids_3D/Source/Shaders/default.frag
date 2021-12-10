#version 460 core

#define MAX_LIGHTS 10

struct AppLight {
	vec3 diffuse;
	vec3 specular;
	vec3 position;
	vec3 attenuation;
    bool directional;
};

uniform int lightNumber;
uniform AppLight lights[MAX_LIGHTS];
uniform vec3 DirLightPos;

struct Material {
	vec3 diffuse;
	vec3 specular;
	float shininess;
	float reflective;
};

uniform Material material;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D shadow;

uniform samplerCube skybox;

uniform vec3 cameraPos;

in vec3 Normal;
in vec3 Position;
in vec2 TexCoords;
in vec4 FragPosLightSpace;

layout (location = 0) out vec4 FragColour;
layout (location = 1) out vec4 BloomColour;

float ShadowCalculation(vec4 fragPosLightSpace, AppLight light);
vec3 CalcDirLight(AppLight light, vec3 viewDir);
vec3 CalcPointLight(AppLight light, vec3 normal, vec3 position, vec3 viewDir);

void main() {
	vec3 colour = vec3(0.0,0.0,0.0);
    vec3 viewDir = normalize(cameraPos - Position);
    float shadowVal = 0;

	if (lightNumber >= 1) {
		for (int i = 0; i < lightNumber; i++) {
			
			if (lights[i].directional) {
				colour += CalcDirLight(lights[i], viewDir);
                shadowVal += ShadowCalculation(FragPosLightSpace, lights[i]);
			} else {
				colour += CalcPointLight(lights[i], Normal, Position, viewDir);
			}
		}
		
		FragColour = clamp(vec4(colour * (1.0 - (shadowVal * 0.4)), 1), 0, 1);
	} else {
		FragColour = clamp(
            vec4(material.diffuse * texture(texture_diffuse1, TexCoords).xyz, 1), 0, 1);
	}

    // debug shadow
    // FragColour = 1.0 - vec4(shadowVal, shadowVal, shadowVal, 1.0);
    // FragColour = vec4(texture(shadow, TexCoords).x, shadowVal, 0, 1);

        float brightness = dot(FragColour.rgb, vec3(0.35, 0.35, 0.35));
    if(brightness > 100.0)
        BloomColour = vec4(FragColour.rgb * 0.8,  1.0);
	else
		BloomColour = vec4(0.0, 0.0, 0.0, 1.0);
}

// calculates the color when using a point light.
vec3 CalcPointLight(AppLight light, vec3 normal, vec3 position, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - position);
    // diffuse shading
    float diff = max(dot(lightDir, normal), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDirection = normalize(lightDir + viewDir);

    // better spec with halfway dir
    float spec = pow(max(dot(normal, halfwayDirection), 0.0), material.shininess);
    // without halfway
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), 50);

    // attenuation
    float dist = length(light.position - position);
    float attenuation = 1.0 / (light.attenuation.x + light.attenuation.y * dist + light.attenuation.z * (dist * dist));

    // combine results
    vec3 diffuse = light.diffuse * diff * material.diffuse * texture(texture_diffuse1, TexCoords).xyz;
    vec3 specular = light.specular * spec * material.specular * texture(texture_specular1, TexCoords).xyz;
    vec3 refl = vec3(0,0,0);

    if (material.reflective > 0)
    {
        vec3 I = normalize(Position - cameraPos);
        vec3 R = reflect(I, normalize(Normal));
        refl =  texture(skybox, R).xyz * material.reflective * diff;
        // refl = clamp(refl, 0, material.reflective);
    }

    diffuse *= attenuation;
    specular *= attenuation;

    return diffuse + specular + refl;
}


vec3 CalcDirLight(AppLight light, vec3 viewDir)
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-light.position);

    // diffuse shading
    float diff = max(dot(norm, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // combine results
    vec3 diffuse = light.diffuse * diff * material.diffuse * texture(texture_diffuse1, TexCoords).xyz;
    vec3 specular = light.specular * spec * material.specular * texture(texture_specular1, TexCoords).xyz;
    vec3 refl = vec3(0,0,0);

    if (material.reflective > 0)
    {
        vec3 I = normalize(Position - cameraPos);
        vec3 R = reflect(I, normalize(Normal));
        refl =  texture(skybox, R).xyz * material.reflective * diff;
        // refl = clamp(refl, 0, material.reflective);
    }

    return diffuse + specular + refl;
}

float ShadowCalculation(vec4 fragPosLightSpace, AppLight light)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadow, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(-light.position - Position);

    float bias = max(0.001 * (1.0 - dot(normal, lightDir)), 0.0001);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadowVal = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadow, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadow, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadowVal += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadowVal /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadowVal = 0.0;
        
    return shadowVal;
}