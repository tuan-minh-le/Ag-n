#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

// Simple lighting parameters
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

// Texture sampling
uniform sampler2D diffuseTexture;
uniform bool hasTexture;

void main()
{
    // Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // Combined result
    vec3 result;
    if (hasTexture) {
        vec3 textureColor = texture(diffuseTexture, TexCoords).rgb;
        result = (ambient + diffuse + specular) * textureColor;
    } else {
        result = (ambient + diffuse + specular) * objectColor;
    }

    FragColor = vec4(result, 1.0);
}
