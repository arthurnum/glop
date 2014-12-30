#version 130

varying float noise;

void main()
{
    gl_FragColor = gl_Color + vec4(0.0, 0.0, 0.0, -0.4);
}
