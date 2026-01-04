#version 330 core

// Input from vertex shader
in vec3 fragColor;
in vec3 fragNormal;
in vec2 fragTexCoord;
in float fragLightLevel;

// Uniforms
uniform sampler2D texture_sampler;
uniform vec3 lightDir;
uniform vec3 viewPos;

// Output
out vec4 FragColor;

void main()
{
    // Sample texture color
    vec4 texColor = texture(texture_sampler, fragTexCoord);
    
    // Discard transparent pixels
    if (texColor.a < 0.1)
        discard;
    
    // Normalize the normal vector
    vec3 norm = normalize(fragNormal);
    
    // Calculate ambient lighting
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * fragColor;
    
    // Calculate diffuse lighting
    vec3 lightDirection = normalize(lightDir);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = diff * fragColor;
    
    // Calculate specular lighting
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - fragColor);
    vec3 reflectDir = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = specularStrength * spec * vec3(1.0);
    
    // Apply light level (day/night cycle)
    float lightIntensity = mix(0.2, 1.0, fragLightLevel);
    
    // Combine all lighting components
    vec3 result = (ambient + diffuse + specular) * lightIntensity;
    
    // Apply texture color
    result *= texColor.rgb;
    
    // Output final color
    FragColor = vec4(result, texColor.a);
}
