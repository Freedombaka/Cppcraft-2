#version 330 core

// Input vertex attributes
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in uint aBlockType;

// Uniforms
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

// Output to fragment shader
out vec3 vPosition;
out vec3 vNormal;
out vec2 vTexCoord;
out flat uint vBlockType;
out float vLighting;

void main()
{
    // Calculate world position
    vPosition = vec3(uModel * vec4(aPosition, 1.0));
    
    // Transform normal to world space
    vNormal = normalize(mat3(transpose(inverse(uModel))) * aNormal);
    
    // Pass texture coordinates
    vTexCoord = aTexCoord;
    
    // Pass block type as flat (uninterpolated)
    vBlockType = aBlockType;
    
    // Calculate basic lighting based on normal direction
    // Simple ambient + directional light
    vec3 lightDir = normalize(vec3(0.5, 1.0, 0.5));
    float diffuse = max(dot(vNormal, lightDir), 0.0);
    vLighting = 0.3 + 0.7 * diffuse;  // 30% ambient + 70% diffuse
    
    // Calculate final position
    gl_Position = uProjection * uView * vec4(vPosition, 1.0);
}
