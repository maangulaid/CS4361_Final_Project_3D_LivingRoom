#version 330 core
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 viewPos;

// ----- lighting controls -----
uniform vec3 lightPos0;     // point light 0 (ceiling-ish)
uniform vec3 lightColor0;

uniform vec3 lightPos1;     // point light 1 (lamp near TV)
uniform vec3 lightColor1;

uniform int  useFlashlight; // 0/1
uniform vec3 flashDir;      // camera front
uniform float flashCutoff;      // cos(innerAngle)
uniform float flashOuterCutoff; // cos(outerAngle)
uniform vec3 flashColor;        // flashlight color

uniform float ambientScale; // 0.0..1.0  (night mode dims this)

// ----- fog controls -----
uniform int   useFog;       // 0/1
uniform vec3  fogColor;
uniform float fogDensity;   // e.g., 0.03

// --------------------------------------
vec3 phongPointLight(vec3 lp, vec3 lc, vec3 fragPos, vec3 N, vec3 V)
{
    vec3 L = normalize(lp - fragPos);
    float dist = length(lp - fragPos);
    float att = 1.0 / (1.0 + 0.22*dist + 0.20*dist*dist); // moderate falloff

    float diff = max(dot(N, L), 0.0);

    vec3 R   = reflect(-L, N);
    float spec = pow(max(dot(V, R), 0.0), 32.0);

    float specularStrength = 0.5;
    vec3 ambient  = ambientScale * lc;
    vec3 diffuse  = diff * lc;
    vec3 specular = specularStrength * spec * lc;

    return (ambient + att * (diffuse + specular)) * objectColor;
}

vec3 flashlight(vec3 fragPos, vec3 N, vec3 V)
{
    // flashlight originates at camera (viewPos), points along flashDir
    vec3 Ldir   = normalize(fragPos - viewPos);
    float theta = dot(normalize(-Ldir), normalize(flashDir)); // how aligned with cone

    float epsilon = flashCutoff - flashOuterCutoff;
    float intensity = clamp((theta - flashOuterCutoff) / epsilon, 0.0, 1.0);

    if (intensity <= 0.0) return vec3(0.0);

    // Phong within the spotlight cone
    vec3 L = normalize(viewPos - fragPos);
    float diff = max(dot(N, L), 0.0);
    vec3 R = reflect(-L, N);
    float spec = pow(max(dot(V, R), 0.0), 32.0);

    float specularStrength = 0.6;
    vec3 ambient  = ambientScale * flashColor * 0.2;
    vec3 diffuse  = diff * flashColor;
    vec3 specular = specularStrength * spec * flashColor;

    // no distance attenuation here (flashlight is close & cone-limited)
    return (ambient + intensity * (diffuse + specular)) * objectColor;
}

void main()
{
    vec3 N = normalize(Normal);
    vec3 V = normalize(viewPos - FragPos);

    // two point lights
    vec3 c0 = phongPointLight(lightPos0, lightColor0, FragPos, N, V);
    vec3 c1 = phongPointLight(lightPos1, lightColor1, FragPos, N, V);

    vec3 total = c0 + c1;

    if (useFlashlight == 1) {
        total += flashlight(FragPos, N, V);
    }

    // fog (exp2)
    if (useFog == 1) {
        float dist = length(viewPos - FragPos);
        float f = 1.0 - exp(-pow(fogDensity * dist, 2.0));
        total = mix(total, fogColor, clamp(f, 0.0, 1.0));
    }

    FragColor = vec4(total, 1.0);
}
