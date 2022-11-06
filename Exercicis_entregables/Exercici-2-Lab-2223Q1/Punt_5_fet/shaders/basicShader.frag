#version 330 core

in vec3 fcolor;
uniform float alternatiu;
out vec4 FragColor;


void main() {
    FragColor = vec4(fcolor*vec3(1.0,1.0-alternatiu,1.0-alternatiu),1);
}

