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
out vec3 L;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 TG;

uniform vec3 posFocus;  // en SCO

void main()
{	
  mat3 NormalMatrix = inverse(transpose(mat3(view*TG))); // Matriu per transformal les normals dels vertexs a SCO. Es pot moure a l'aplicació ja que sempre es la mateixa
  normalSCO = NormalMatrix*normal; //Normals en SCO
  normalSCO = normalize(normalSCO); //Tornem a normalitzar

  vec4 posFocusSCO = vec4(posFocus,1.0); //Ara ho deixem ja en les coordenades que es donen

  vertexSCO = view*TG*vec4(vertex,1.0);
  L = posFocusSCO.xyz - vertexSCO.xyz;
  L = normalize(L); //Tornem a normalitzar

  frag_matspec = matspec;
  frag_matamb = matamb;
  frag_matdiff = matdiff;
  frag_matshin = matshin;

  gl_Position = proj * view * TG * vec4 (vertex, 1.0);
}
