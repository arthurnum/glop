#version 120

varying float noise;

void main()
{
    float a = 0.5;
    if (noise > 0.95) { a = pow(1.0 - (noise - 0.95) * 10.0, 2.0); }
    else { a = pow(noise / 0.95, 8.0); }
    gl_FragColor = vec4(gl_Color.rgb, a);
}
