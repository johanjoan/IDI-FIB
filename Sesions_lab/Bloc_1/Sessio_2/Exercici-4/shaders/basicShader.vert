#version 330 core

in vec3 vertex;

in vec3 colors;

out vec3 fcolors;

void main()  {
    gl_Position = vec4 (vertex, 1.0); //Multipliquem els vèrtex per 0.5
    fcolors = colors;
}
