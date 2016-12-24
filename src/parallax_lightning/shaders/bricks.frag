#version 330 core
out vec4 FragColor;

in VertexInfo {
	vec3 FragPos;
	vec2 TexCoords;
	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangentFragPos;
} fragIn;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;

vec2 parallaxMapping(vec2 texCoords, vec3 viewDir)
{
	const float MIN_LAYERS = 10;
	const float MAX_LAYERS = 20;
	float numLayers = mix(MAX_LAYERS, MIN_LAYERS, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));

	float layerDepth = 1.0 / numLayers;
	float currentLayerDepth = 0.0;
	vec2 P = viewDir.xy / viewDir.z * 0.1;
	vec2 deltaTexCoords = P / numLayers;

	vec2 currentTexCoords = texCoords;
	float depth = texture(depthMap, currentTexCoords).r;

	while (currentLayerDepth < depth) {
		currentTexCoords -= deltaTexCoords;
		depth = texture(depthMap, currentTexCoords).r;
		currentLayerDepth += layerDepth;
	}

	vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

	float afterDepth  = depth - currentLayerDepth;
	float beforeDepth = texture(depthMap, prevTexCoords).r - currentLayerDepth + layerDepth;

	float weight = afterDepth / (afterDepth - beforeDepth);
	vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

	return finalTexCoords;
}

void main()
{
	vec3 viewDir = normalize(fragIn.TangentViewPos - fragIn.TangentFragPos);
	vec2 texCoords = parallaxMapping(fragIn.TexCoords, viewDir);

	if (texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
		discard;

	vec3 color = texture(diffuseMap, texCoords).rgb;
	vec3 ambient = 0.1 * color;

	vec3 lightDir = normalize(fragIn.TangentLightPos - fragIn.TangentFragPos);
	vec3 normal = normalize(texture(normalMap, texCoords).rgb * 2.0 - 1.0);
	vec3 diffuse = color * max(dot(lightDir, normal), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	vec3 specular = vec3(0.2) * pow(max(dot(normal, halfwayDir), 0.0), 32.0);

	FragColor = vec4(ambient + diffuse + specular, 1.0f);
}
