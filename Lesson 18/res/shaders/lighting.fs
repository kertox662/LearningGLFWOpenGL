#version 330 core
out vec4 FragColor;

struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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

uniform vec3 u_ObjectColor;
uniform vec3 u_ViewPos;

uniform Material u_Mat;
uniform Light u_Light;

void main()
{
    //Ambient Light because darkness is overrated
    vec3 ambient = u_Light.ambient * u_Mat.ambient;

    //Normal vector and light dir
    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(u_Light.pos - FragPos);

    //Calculate diffuse value
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse =  u_Light.diffuse * (diff * u_Mat.diffuse);

    //Get the reflection
    vec3 viewDir = normalize(u_ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    //Specular from reflection
    float spec = pow(max(dot(viewDir, reflectDir),0.0),u_Mat.shininess);
    vec3 specular =  (spec * u_Mat.specular) * u_Light.specular;

    vec3 colorResult = (ambient + diffuse + specular);

    FragColor = vec4(colorResult, 1.0);
}