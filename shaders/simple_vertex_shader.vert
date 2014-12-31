#version 130

varying float noise;

uniform vec3 center;

void main()
{
    float x = gl_Vertex.x - center.x;
    float z = gl_Vertex.z - center.z;
    noise = sqrt( pow(x, 2) + pow(z, 2) );
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    gl_FrontColor = gl_Color;
}
