#version 330 core
out FragColor;
uniform vec3 ObjectColor;
void main(){
    FragColor = vec4(ObjectColor,1);
}