#version 330 core
out vec4 FragColor;

struct Material{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct Light{
    vec3 pos;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 v_Normal;
in vec3 FragPos;
in vec2 v_TexCoords;

uniform vec3 u_ObjectColor;
uniform vec3 u_ViewPos;

uniform Material u_Mat;
uniform Light u_Light;

uniform float u_Time;

void main()
{
    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(u_Light.pos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 viewDir = normalize(u_ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir),0.0),u_Mat.shininess);

    vec3 diffuse =  u_Light.diffuse * diff * vec3(texture(u_Mat.diffuse, v_TexCoords));
    vec3 ambient = u_Light.ambient * vec3(texture(u_Mat.diffuse, v_TexCoords));
    vec3 specular =  (spec * vec3(texture(u_Mat.specular, v_TexCoords))) * u_Light.specular;

    vec2 modTex = v_TexCoords + vec2(0.0, u_Time);
    if(modTex.y > 1) modTex.y -= int(modTex.y);
    vec3 emission = vec3(texture(u_Mat.emission, modTex));
    vec3 specMap = vec3(texture(u_Mat.specular, v_TexCoords));
    if(specMap.r > 0 || specMap.g > 0 || specMap.b > 0) emission = vec3(0.0);

    emission *= sin(2*u_Time) * 0.5 + 0.5;

    vec3 colorResult = (ambient + diffuse + specular + emission);

    FragColor = vec4(colorResult, 1.0);
}