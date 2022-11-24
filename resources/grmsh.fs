#version 330 core
out vec4 FragColor;
in vec3 ourColor;

struct Material {
    float shininess;
}; 

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D ourTexture;


uniform vec3 viewPos;
uniform DirLight dirLight;
uniform Material material;

// function prototypes

void main()
{    
    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    
    FragColor = texture(ourTexture, TexCoords);
}

// calculates the color when using a directional light.

// calculates the color when using a point light.
