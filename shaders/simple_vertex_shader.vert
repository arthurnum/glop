#version 120

varying float noise;

uniform vec3 center;

void main()
{
    float x = gl_Vertex.x - center.x;
    float z = gl_Vertex.z - center.z;
    noise = sqrt( pow(x, 2.0) + pow(z, 2.0) );
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    gl_FrontColor = gl_Color;
}
