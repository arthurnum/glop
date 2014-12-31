#version 130

varying float noise;

void main()
{
    float a = 1.0;
    if (noise > 0.95) { a = pow(1 - (noise - 0.95) * 10, 2.0); }
    else { a = pow(noise / 0.95, 4.0); }
    gl_FragColor = vec4(gl_Color.rgb, a);
}
