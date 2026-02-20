#version 330

// Input vertex attributes (from vertex shader)
in vec3 fragPosition;
in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragNormal;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;

// NOTE: Add your custom variables here

#define     MAX_LIGHTS              4
#define     LIGHT_DIRECTIONAL       0
#define     LIGHT_POINT             1

struct Light {
  int enabled;
  int type;
  vec3 position;
  vec3 target;
  vec4 color;
  float intensity;
};

// Input lighting values
uniform Light lights[MAX_LIGHTS];
uniform vec4 ambient;
uniform vec3 viewPos;
uniform int tiling;

float constant = 1.0;
float linear = 0.09;
float quadratic = 0.032;

void main()
{
  // Texel color fetching from texture sampler
  vec4 texelColor = texture(texture0, fragTexCoord * tiling);
  vec3 lightDot = vec3(0.0);
  vec3 normal = normalize(fragNormal);
  vec3 viewD = normalize(viewPos - fragPosition);
  vec3 specular = vec3(0.0);

  vec4 tint = colDiffuse * fragColor;

  // NOTE: Implement here your fragment shader code

  for (int i = 0; i < MAX_LIGHTS; i++)
  {
    if (lights[i].enabled == 1)
    {
      vec3 light = vec3(0.0);

      if (lights[i].type == LIGHT_DIRECTIONAL)
      {
        light = normalize(-lights[i].position);
      }

      if (lights[i].type == LIGHT_POINT)
      {
        light = normalize(lights[i].position - fragPosition);
      }

      float NdotL = max(dot(normal, light), 0.0);

      float distance = length(lights[i].position - fragPosition);
      float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

      if (lights[i].type == LIGHT_POINT) {
        lightDot += lights[i].color.rgb * NdotL * lights[i].intensity * attenuation;
      } else {
        lightDot += lights[i].color.rgb * NdotL * lights[i].intensity;
      }

      float specCo = 0.0;
      if (NdotL > 0.0) {
        vec3 halfwayDir = normalize(light + viewD);
        specCo = pow(max(0.0, dot(viewD, halfwayDir)), 16.0); // 16 refers to shine
      }

      if (lights[i].type == LIGHT_POINT) {
        specCo *= attenuation;
      }

      specular += specCo * lights[i].intensity;
    }
  }

  finalColor = (texelColor * ((tint + vec4(specular, 1.0)) * vec4(lightDot, 1.0)));
  finalColor += texelColor * (ambient / 10.0) * tint;

  // Gamma correction
  finalColor = pow(finalColor, vec4(1.0 / 2.2));
}
