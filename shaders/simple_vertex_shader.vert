#version 130

varying float noise;

void main()
{
    noise = abs(gl_Vertex.x * gl_Vertex.z);
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    gl_FrontColor = gl_Color;
}
