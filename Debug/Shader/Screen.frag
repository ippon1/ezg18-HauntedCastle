#version 430 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

// Interpolated values from the vertex shaders
in vec2 UV;
in vec3 Position_worldspace;
in vec3 Normal_worldspace;
in vec3 Tangent_worldspace;
in vec3 Bitangent_worldspace;
in mat3 TBN;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 Torch1Direction_cameraspace;
in vec3 Torch2Direction_cameraspace;
in vec4 ShadowCoord;

uniform int NORMAL_MAPPING;
uniform int FIRE_AND_SHADOWS_1;
uniform int FIRE_AND_SHADOWS_2;
uniform float FIRE_AND_SHADOWS_INTENSITY_1;
uniform float FIRE_AND_SHADOWS_INTENSITY_2;
uniform int LEVEL_OF_RENDER_QUALITY;



// Values that stay constant for the whole mesh.
uniform sampler2D modelTexture; // modelTexture
uniform sampler2D modelNormalTexture; // modelTexture
uniform vec3 Torch1Position_worldspace;
uniform vec3 Torch2Position_worldspace;
uniform float flameIntensity[2];
uniform int hasTexture;
uniform int hasNormalTexture;
uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform sampler2DShadow directionalShadowsDepthMap;
uniform vec3 SunDirection_worldspace;
uniform float AmbientIntensity;

	//////////////// calculate point shadow ///////////////
	uniform samplerCube pointShadowsDepthCubeMap1;
	uniform samplerCube pointShadowsDepthCubeMap2;
	uniform vec3 viewPos;
	uniform vec3 flameCenterPosition1;
	uniform vec3 flameCenterPosition2;
	uniform float pointShadowsFarPlane;
	in vec3 FragPos;
	//////////////////////////////////////////////////////


uniform mat4 light_projection_matrix;
uniform int renderRayShafts;


vec2 disk[16] = vec2[](
   vec2( -0.94202634, -0.39907226 ),
   vec2( 0.94558508, -0.76891735 ),
   vec2( -0.094184211, -0.92938780 ),
   vec2( 0.34495848, 0.29387650 ),
   vec2( -0.91588491, 0.45771522 ),
   vec2( -0.81544152, -0.87912464 ),
   vec2( -0.38277463, 0.27676836 ),
   vec2( 0.97484401, 0.75648380 ),
   vec2( 0.44323319, -0.97511562 ),
   vec2( 0.53742979, -0.47373419 ),
   vec2( -0.26496815, -0.41893133 ),
   vec2( 0.79197508, 0.19090191 ),
   vec2( -0.24188838, 0.99706499 ),
   vec2( -0.81409960, 0.91437588 ),
   vec2( 0.19984118, 0.78641372 ),
   vec2( 0.14383159, -0.14100800 )
);

// array of offset direction for sampling
vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1),
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

// TODO aufraumen
float ShadowCalculation(vec3 fragPos, vec3 flameCenterPosition, samplerCube pointShadowsDepthCubeMap)
{
    // get vector between fragment position and light position
    vec3 fragToLight = fragPos - flameCenterPosition;
    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    // test for shadows
    // PCF
    float shadow = 0.0;
    float bias = 0.25;
    int samples = 20;
    float viewDistance = length(viewPos - fragPos);
    float diskRadius = (1.0 + (viewDistance / pointShadowsFarPlane)) / 25.0;

	float currentDepthFactor = diskRadius * min(1, 1 / sqrt(currentDepth));

	float currentDepthMinusBias = currentDepth - bias;

    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(pointShadowsDepthCubeMap, fragToLight + gridSamplingDisk[i] * currentDepthFactor).r;
        closestDepth *= pointShadowsFarPlane;   // undo mapping [0;1]
        if(currentDepthMinusBias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);

    return shadow;
}

mat4 biasMatrix = mat4(0.5, 0.0, 0.0, 0.0,
	0.0, 0.5, 0.0, 0.0,
	0.0, 0.0, 0.5, 0.0,
	0.5, 0.5, 0.5, 1.0);

float PI = 3.14159265359;
float PI_RCP = 0.31830988618;
float tau = 0.02; // Thickness of medium
float phi = 200.0; // Light strength

vec3 light_position = vec3(0, 12, 20) + 10.0 * vec3(0, 1, 1);

float lightShaft(vec3 FragPos_worldspace, vec3 Camera_worldspace) {

	float raymarch_distance_worldspace = length(Camera_worldspace - FragPos_worldspace);
	vec3 delta_lightview = normalize(Camera_worldspace - FragPos_worldspace);
	vec3 delta_worldspace = delta_lightview;

	float level_of_render_quality_factor = (LEVEL_OF_RENDER_QUALITY == 1 ? 1.0 : 2.0);

	float step_size = level_of_render_quality_factor * 0.025 * raymarch_distance_worldspace;

	vec3 ray_position_lightview = FragPos_worldspace;

	
	float light_contribution = 0;

	int previosLuminated = 0;

	for (float l = raymarch_distance_worldspace; l > step_size; l -= step_size) {

		vec4 ray_position_lightclipspace = biasMatrix * light_projection_matrix * vec4(ray_position_lightview, 1);

		float shadow_term = texture( directionalShadowsDepthMap, ray_position_lightclipspace.xyz );
		
		if (shadow_term > 0.0) {
			
			for (float i = previosLuminated == 1 ? 0 : -0.9; i <= 0.9; i += 0.1) {
			
				vec3 ray_position_lightview_detail = ray_position_lightview + i * step_size * delta_lightview;
				
				vec4 ray_position_lightclipspace_detail = biasMatrix * light_projection_matrix * vec4(ray_position_lightview_detail, 1);

				shadow_term = texture( directionalShadowsDepthMap, ray_position_lightclipspace_detail.xyz );

				// get distance in world space
				float d = length(vec3(0, ray_position_lightview_detail.y - light_position.y, ray_position_lightview_detail.z - light_position.z));
				// multiplication is faster than division, so do it once
				float d_rcp = 1.0/d;
		
				light_contribution += level_of_render_quality_factor * 0.1 * shadow_term * (phi * 0.25 * PI_RCP) * d_rcp * d_rcp * 
					exp(-d*tau) *
					exp(-l*tau);
			}
			previosLuminated = 1;
		} else {
			previosLuminated = 0;
		}

		ray_position_lightview += step_size * delta_lightview;
	}

	return light_contribution;
}


void main(){

	// Use Texture or Material Color
	vec3 MaterialDiffuseColor;

	if(hasTexture == 1)
	{
		MaterialDiffuseColor = texture( modelTexture, UV ).rgb;
	}
	else
	{
		MaterialDiffuseColor = diffuseColor;
	}

	//////////////// calculate point shadow ///////////////
	float shadow1 = ShadowCalculation(FragPos, flameCenterPosition1, pointShadowsDepthCubeMap1);
	float shadow2 = ShadowCalculation(FragPos, flameCenterPosition2, pointShadowsDepthCubeMap2);
	//////////////////////////////////////////////////////

	vec3 MaterialAmbientColor = AmbientIntensity * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = specularColor;


	vec3 normal;
	vec3 torch1Dir;
	vec3 torch2Dir;
	vec3 viewDir;

	if(NORMAL_MAPPING == 1 && hasNormalTexture == 1)
	{
		// obtain normal from normal map in range [0,1]
		normal = texture(modelNormalTexture, UV).rgb;
		// transform normal vector to range [-1,1]
		normal = normalize(normal * 2.0 - 1.0);
		normal = normalize(TBN * normal);

		torch1Dir = normalize(Torch1Position_worldspace - Position_worldspace);
		torch2Dir = normalize(Torch2Position_worldspace - Position_worldspace);
		viewDir  = normalize(viewPos - Position_worldspace);

	}
	else
	{
		normal = Normal_cameraspace;
		torch1Dir = Torch1Direction_cameraspace;
		torch2Dir = Torch2Direction_cameraspace;
		viewDir = EyeDirection_cameraspace;
	}





	vec3 n1 = normalize( normal );
	vec3 l1 = normalize( torch1Dir );
	float cosTheta1 = clamp( dot( n1, l1 ), 0, 1 );

	vec3 E1 = normalize(viewDir);
	vec3 R1 = reflect( -l1, n1 );
	float cosAlpha1 = clamp( dot( E1, R1 ), 0, 1 );

	float I1 = 1 / (0.5 * distance( Torch1Position_worldspace, Position_worldspace ));



	vec3 n2 = normalize( normal );
	vec3 l2 = normalize( torch2Dir );
	float cosTheta2 = clamp( dot( n2, l2 ), 0, 1 );

	vec3 E2 = normalize(viewDir);
	vec3 R2 = reflect( -l2, n2 );
	float cosAlpha2 = clamp( dot( E2, R2 ), 0, 1 );

	float I2 = 1 / (0.5 * distance( Torch2Position_worldspace, Position_worldspace ));

	float visibility=1.0;

	// Bias to get rid of Shadow acne
	float bias = 0.002;


	// PCF
	// 4 Times Stratified Poisson Sampling
	float discDevidor = 1/700.0;
	float ShadowCoordZFactor = (ShadowCoord.z-bias)/ShadowCoord.w;
	for (int i=0;i<4;i++){
		visibility -= 0.25*(1.0-texture( directionalShadowsDepthMap, vec3(ShadowCoord.xy + disk[i] * discDevidor,   ShadowCoordZFactor)));
	}

	vec3 nV = normalize( Normal_worldspace );
	vec3 lV = normalize( SunDirection_worldspace );
	float visibilityPosible = dot( nV, lV ) >= 0 ? 1 : 0;

	visibility = visibility * visibilityPosible;

	
	float lightshaft = renderRayShafts == 1 ? lightShaft(FragPos, viewPos) : 0;
	
	vec3 result = vec3(
	//FragColor = vec4(
		// Ambient
		vec3(MaterialAmbientColor) +

		FIRE_AND_SHADOWS_1 * FIRE_AND_SHADOWS_INTENSITY_1 *
		(1.0-shadow1) *
		(
			flameIntensity[0]*
			(
				// Diffuse Torch 1
				MaterialDiffuseColor * I1 * cosTheta1 +
				// Specular Torch 1
				MaterialSpecularColor * I1 * pow(cosAlpha1, 5)
			)
		)
		+
		FIRE_AND_SHADOWS_2 * FIRE_AND_SHADOWS_INTENSITY_2 *
		(1.0-shadow2) *
		(
			flameIntensity[1] *
			(
				// Diffuse Torch 2
				MaterialDiffuseColor * I2 * cosTheta2 +
				// Specular Torch 2
				MaterialSpecularColor * I2 * pow(cosAlpha2, 5)
			)
		) +
		// Window
		MaterialDiffuseColor * visibility + 

		vec3(lightshaft)
	);

	//float brightness = dot(result, vec3(1.0, 1.0, 1.0));
    //if(brightness > 1.0){
	if(MaterialDiffuseColor.r == 1.1 && MaterialDiffuseColor.g == 1.1 && MaterialDiffuseColor.b == 1.1){ // workaround
        BrightColor = vec4(MaterialDiffuseColor, 1.0);
    } else {
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
	FragColor = vec4(result, 1.0);
	//FragColor = vec4(vec3(cosTheta1), 1);
}
