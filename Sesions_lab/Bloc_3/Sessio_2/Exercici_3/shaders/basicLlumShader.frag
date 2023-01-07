#version 330 core

in vec3 fcolor;
in vec3 frag_matspec;
in vec3 frag_matamb;
in vec3 frag_matdiff;
in float frag_matshin;
out vec4 FragColor;

in vec3 normalSCO;
in vec3 L;
in vec4 vertexSCO;
uniform int focus;

// Valors per als components que necessitem del focus de llum
uniform vec3 colorFocus;
uniform vec3 posFocus;  // en SCO
vec3 llumAmbient = vec3(0.2, 0.2, 0.2);
uniform mat4 view;

vec3 Ambient() {
    return llumAmbient * frag_matamb;
}

vec3 Difus (vec3 NormSCO, vec3 L, vec3 colFocus) 
{
    // Tant sols retorna el terme difús
    // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats
    vec3 colRes = vec3(0);
    // Càlcul component difusa, si n'hi ha
    if (dot (L, NormSCO) > 0)
      colRes = colFocus * frag_matdiff * dot (L, NormSCO);
    return (colRes);
}

vec3 Especular (vec3 NormSCO, vec3 L, vec4 vertSCO, vec3 colFocus) 
{
    // Tant sols retorna el terme especular!
    // Els vectors rebuts com a paràmetres (NormSCO i L) estan normalitzats
    vec3 colRes = vec3 (0);
    // Si la llum ve de darrera o el material és mate no fem res
    if ((dot(NormSCO,L) < 0) || (frag_matshin == 0))
      return colRes;  // no hi ha component especular

    // Calculem R i V
    vec3 R = reflect(-L, NormSCO); // equival a: 2.0*dot(NormSCO,L)*NormSCO - L;
    vec3 V = normalize(-vertSCO.xyz); // perquè la càmera està a (0,0,0) en SCO

    if (dot(R, V) < 0)
      return colRes;  // no hi ha component especular
    
    // Calculem i retornem la component especular
    float shine = pow(max(0.0, dot(R, V)), frag_matshin);   
    return (frag_matspec * colFocus * shine);     
}


void main()
{	

  vec3 L;

  vec4 posFocusSCO;
  if (focus == 1) posFocusSCO = view*vec4(posFocus,1.0); 
  else posFocusSCO = vec4(posFocus,1.0);

  L = normalize((posFocusSCO - vertexSCO).xyz);

	FragColor = vec4(Ambient() + Difus(normalSCO,L,colorFocus) + Especular(normalSCO,L,vertexSCO,colorFocus),1);	
}
