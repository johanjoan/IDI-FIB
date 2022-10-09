#version 330 core

in vec3 vertex;

in vec3 color;

uniform mat4 TG;

uniform float scl; //Escalarà el triangle

out vec3 fcolor;

void main()  {
    fcolor = color;
    gl_Position = TG*vec4 (vertex*scl, 1.0); //Multipliquem el vertex per scl (escalar) i per la matriu de transformació
}
