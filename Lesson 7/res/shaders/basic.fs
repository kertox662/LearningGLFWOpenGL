//FRAGMENT SHADER
#version 330 core
        
layout(location = 0) out vec4 color;
in vec4 col;

uniform vec4 u_Color;

void main(){
    float cX = col.x;
    float cY = col.y;
    // color = vec4((cX+1)/2, 0, (cY+1)/2, 1.0);
    color = u_Color;
}