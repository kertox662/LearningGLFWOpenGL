//VERTEX SHADER
#version 330 core

layout(location = 0) in vec4 position;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 textureCoords;
// layout(location = 1) in vec2 texCoord;

// out vec2 v_TexCoord;
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;
uniform mat3 u_NormalMatrix;
// uniform vec3 u_ViewPos;

out vec3 v_Normal;
out vec3 FragPos;
out vec2 v_TexCoords;

void main(){
    // mat3 normalMat = mat3(transpose(inverse(u_Model)));
    gl_Position = u_Proj * u_View * u_Model * position;
    FragPos = vec3(u_Model*position);
    v_Normal = u_NormalMatrix * vertexNormal;
    v_TexCoords = textureCoords;
}