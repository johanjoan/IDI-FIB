#version 330 core

in vec3 vertex;
in vec3 normal;

in vec3 matamb;
in vec3 matdiff;
in vec3 matspec;
in float matshin;

out vec3 frag_matspec;
out vec3 frag_matamb;
out vec3 frag_matdiff;
out float frag_matshin;

out vec3 normalSCO;
out vec4 vertexSCO;
out vec3 L_escena;
out vec3 L_camera;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 TG;


void main()
{	
  mat3 NormalMatrix = inverse(transpose(mat3(view*TG))); // Matriu per transformal les normals dels vertexs a SCO. Es pot moure a l'aplicació ja que sempre es la mateixa
  normalSCO = NormalMatrix*normal; //Normals en SCO
  normalSCO = normalize(normalSCO); //Tornem a normalitzar
  
  vertexSCO = view*TG*vec4(vertex,1.0);



  frag_matspec = matspec;
  frag_matamb = matamb;
  frag_matdiff = matdiff;
  frag_matshin = matshin;

  gl_Position = proj * view * TG * vec4 (vertex, 1.0);
}
