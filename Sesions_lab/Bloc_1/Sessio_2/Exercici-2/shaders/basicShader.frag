#version 330 core

out vec4 FragColor;

void main() {
    if (gl_FragCoord.x < 354.) { //Part esquerre
        if (gl_FragCoord.y > 354.) FragColor = vec4(1.0,0.0,0.0,1); //Superior
        else FragColor = vec4(1.0,1.0,0.0,1); //Inferior
    }

    else { //Part dreta
        if (gl_FragCoord.y > 354.) FragColor = vec4(0.0,0.0,1.0,1); //Superior
        else FragColor = vec4(0.0,1.0,0.0,1);
    }

    if (int(gl_FragCoord.y) % 10 < 4) discard; //Descartem cada 4 de cada 10
}

