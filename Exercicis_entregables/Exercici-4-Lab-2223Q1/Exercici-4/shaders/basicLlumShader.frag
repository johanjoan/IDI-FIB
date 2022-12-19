#version 330 core

in vec3  fmatamb;
in vec3  fmatdiff;
in vec3  fmatspec;
in float fmatshin;
in vec3  fvertex;
in vec3  fnormal;

const vec3 llumAmbient = vec3(0.1, 0.1, 0.1);

uniform mat4 TG;
uniform mat4 Proj;
uniform mat4 View;

uniform vec3 colorFocus; //Color dels focus d'escena
uniform mat4 posFocus; //Posicions dels 4 focus d'escena
uniform mat4 TG_llum; //TG que cal aplicar al focus del Morty per a que aparegui on toca
uniform vec3 colorFocusMorty;

out vec4 FragColor;


vec3 Ambient() {
  return llumAmbient*fmatamb;
}

vec3 Difus (vec3 NormSCO, vec3 L, vec3 colFocus)
{
  // Fixeu-vos que SOLS es retorna el terme de Lambert!
  // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats
  vec3 colRes = vec3(0);
  if (dot (L, NormSCO) > 0)
    colRes = colFocus * fmatdiff * dot (L, NormSCO);
  return (colRes);
}

vec3 Especular (vec3 NormSCO, vec3 L, vec3 vertSCO, vec3 colFocus)
{
  // Fixeu-vos que SOLS es retorna el terme especular!
  // Assumim que els vectors estan normalitzats
  vec3 colRes = vec3 (0);
  // Si la llum ve de darrera o el material és mate no fem res
  if ((dot(NormSCO,L) < 0) || (fmatshin == 0))
    return colRes;  // no hi ha component especular

  vec3 R = reflect(-L, NormSCO); // equival a: 2.0*dot(NormSCO,L)*NormSCO - L;
  vec3 V = normalize(-vertSCO); // perquè la càmera està a (0,0,0) en SCO

  if (dot(R, V) < 0)
    return colRes;  // no hi ha component especular

  float shine = pow(max(0.0, dot(R, V)), fmatshin);
  return (colRes + fmatspec * colFocus * shine);
}

void main()
{
  vec3 aux = Ambient(); //Llum Ambient

  //Col·loquem cadascun dels 4 focus d'escena
  for (int i = 0; i < 4; ++i) {
    vec4 posFocusSCO = View*vec4(posFocus[i].xyz,1.0);
    vec3 L = posFocusSCO.xyz - fvertex.xyz;
    L = normalize(L);
    aux += Difus(fnormal,L,colorFocus) + Especular(fnormal,L,fvertex,colorFocus);
  }

  //Afegim el focus de la mà del Morty
  vec4 posFocusSCO = View*TG_llum*vec4(vec3(60,-90,7),1.0); //Passar a vec3
  vec3 L = posFocusSCO.xyz - fvertex.xyz;
  L = normalize(L);
  aux += Difus(fnormal,L,colorFocusMorty) + Especular(fnormal,L,fvertex,colorFocusMorty);


  FragColor = vec4(aux,1.0); //Enviem tot

}
