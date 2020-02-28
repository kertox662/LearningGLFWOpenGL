#version 330 core
out vec4 FragColor;

in vec3 v_Normal;
in vec3 FragPos;

uniform vec3 u_ObjectColor;
uniform vec3 u_LightColor;
uniform vec3 u_LightPos;
uniform vec3 u_ViewPos;

void main()
{
    //Intensity Values
    float ambientStrength = 0.1;
    float specularStrength = 0.5;

    //Ambient Light because darkness is overrated
    vec3 ambient = ambientStrength * u_LightColor;

    //Normal vector and light dir
    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(u_LightPos - FragPos);

    //Calculate diffuse value
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_LightColor;

    //Get the reflection
    vec3 viewDir = normalize(u_ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    //Specular from reflection
    float spec = pow(max(dot(viewDir, reflectDir),0.0),32);
    vec3 specular = specularStrength * spec * u_LightColor;

    vec3 colorResult = u_ObjectColor * (ambient + diffuse + specular);

    FragColor = vec4(colorResult, 1.0);
}