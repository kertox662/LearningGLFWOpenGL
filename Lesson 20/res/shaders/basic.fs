//FRAGMENT SHADER
#version 330 core
        
layout(location = 0) out vec4 color;

// uniform sampler2D u_Texture;
uniform vec4 inColor;
// in vec2 v_TexCoord;

void main(){
    // vec4 texColor = texture(u_Texture, v_TexCoord);
    // color = texColor;
    color = vec4(1.0f);
}