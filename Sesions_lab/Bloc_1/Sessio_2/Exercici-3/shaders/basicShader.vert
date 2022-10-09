#version 330 core

in vec3 vertex;

void main()  {
    gl_Position = vec4 (0.5*vertex, 1.0); //Multipliquem els vèrtex per 0.5
}
