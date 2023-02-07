#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};
#define NR_POINT_LIGHTS 4

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;

// function prototypes
//vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
//vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
//vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
uniform float DeltaTime;
uniform vec2 iResolution;

vec3 drawCircle(vec2 pos, float radius, float width, float power, vec4 color)
{
    vec2 mousePos = vec2(0.5);
    float dist1 = length(pos);
    dist1 = fract((dist1 * 5.0) - fract(DeltaTime));
    float dist2 = dist1 - radius;
    float intensity = pow(radius / abs(dist2), width); 
    vec3 col = color.rgb * intensity * power * max((0.8- abs(dist2)), 0.0);
    return col;
}

vec3 hsv2rgb(float h, float s, float v)
{
    vec4 t = vec4(1.0, 2.0/3.0, 1.0/3.0, 3.0);
    vec3 p = abs(fract(vec3(h) + t.xyz) * 6.0 - vec3(t.w));
    return v * mix(vec3(t.x), clamp(p - vec3(t.x), 0.0, 1.0), s);
}

void main()
{
    // // -1.0 ~ 1.0
    //vec2 fragCoord = vec2(0, 0);
    vec2 pos = (gl_FragCoord.xy * 2.0 - iResolution.xy) / min(iResolution.x, iResolution.y);
    
    float h = mix(0.5, 0.65, length(pos));
    vec4 color = vec4(hsv2rgb(h, 1.0, 1.0), 1.0);
    float radius = 0.5;
    float width = 0.8;
    float power = 0.1;
    vec3 finalColor = drawCircle(pos, radius, width, power, color);

    pos = abs(pos);
    // vec3 finalColor = vec3(pos.x, 0.0, pos.y);

    FragColor = vec4(finalColor, 1.0);
}
