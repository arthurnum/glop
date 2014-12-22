#include "init.h"
#include "headers/glop_math.h"

#include <stdio.h>


bmp_image load_bmp_image(const char* filename)
{
    bmp_image image;
    // Data read from the header of the BMP file
    unsigned char header[54]; // Each BMP file begins by a 54-bytes header
    unsigned int dataPos;     // Position in the file where the actual data begins
    unsigned int imageSize;   // = width*height*3

    FILE *file = fopen(filename,"rb");
    fread(header, 1, 54, file);
    // Read ints from the byte array
    dataPos = *(int*)&(header[0x0A]);
    imageSize = *(int*)&(header[0x22]);
    image.width = *(int*)&(header[0x12]);
    image.height = *(int*)&(header[0x16]);
    // Some BMP files are misformatted, guess missing information
    if (imageSize==0) imageSize = image.width * image.height * 3; // 3 : one byte for each Red, Green and Blue component
    if (dataPos==0) dataPos = 54; // The BMP header is done that way
    // Create a buffer
    image.data = new unsigned char [imageSize];

    // Read the actual data from the file into the buffer
    fread(image.data, 1, imageSize, file);
    for (int i = 0; i < imageSize; i += 3)
    {
        unsigned char buf = image.data[i];
        image.data[i] = image.data[i+2];
        image.data[i+2] = buf;
    }

    //Everything is in memory now, the file can be closed
    fclose(file);
    return image;
}


void build_glop_data()
{
    vertex_t *glop_vertices;
    vertex_t *glop_normals;
    vertex_t *super_vertices;
    vertex_t *super_normals;

    bmp_image Hmap;
    Hmap = load_bmp_image("map128.bmp");
    int D = Hmap.width;
    glop_vertices = new vertex_t[D*D];

    float scale = 0.10f;
    for (int h = 0; h < D; h++)
    {
        for (int w = 0; w < D; w++)
        {
            int i = h*D + w;
            glop_vertices[i].x = w * scale;
            glop_vertices[i].y = Hmap.data[((h)*Hmap.width*3+w*3)] / 255.0;
            glop_vertices[i].z = -(h * scale);
        }
    }

    glop_normals = new vertex_t[D*D];

    // bottom left normal
    vertex_t n1, n2, n3, n4, n5, n6;
    normal(glop_vertices[D+1], glop_vertices[D], glop_vertices[0], &n1);
    normal(glop_vertices[1], glop_vertices[D+1], glop_vertices[0], &n2);
    glop_normals[0].x = n1.x + n2.x;
    glop_normals[0].y = n1.y + n2.y;
    glop_normals[0].z = n1.z + n2.z;
    normalize(&glop_normals[0]);

    // bottom right normal
    normal(glop_vertices[D-1], glop_vertices[D+D-1], glop_vertices[D-2], &glop_normals[D-1]);

    // top left normal
    normal(glop_vertices[D*D-D+1], glop_vertices[D*D-D], glop_vertices[D*D-D-D], &glop_normals[D*D-D]);
    
    // top right normal
    normal(glop_vertices[D*D-1], glop_vertices[D*D-2], glop_vertices[D*D-D-2], &n1);
    normal(glop_vertices[D*D-D-1], glop_vertices[D*D-1], glop_vertices[D*D-D-2], &n2);
    glop_normals[D*D-1].x = n1.x + n2.x;
    glop_normals[D*D-1].y = n1.y + n2.y;
    glop_normals[D*D-1].z = n1.z + n2.z;
    normalize(&glop_normals[D*D-1]);

    // bottom normals
    for (int i = 1; i < D-1; i++)
    {
        normal(glop_vertices[D+i+1], glop_vertices[D+i], glop_vertices[i], &n1);
        normal(glop_vertices[i+1], glop_vertices[D+i+1], glop_vertices[i], &n2);
        normal(glop_vertices[i], glop_vertices[D+i], glop_vertices[i-1], &n3);
        glop_normals[i].x = n1.x + n2.x + n3.x;
        glop_normals[i].y = n1.y + n2.y + n3.y;
        glop_normals[i].z = n1.z + n2.z + n3.z;
        normalize(&glop_normals[i]);
    }

    // top normals
    for (int i = 1; i < D-1; i++)
    {
        normal(glop_vertices[D*D-D+i], glop_vertices[D*D-D+i-1], glop_vertices[D*D-D-D+i-1], &n1);
        normal(glop_vertices[D*D-D-D+i], glop_vertices[D*D-D+i], glop_vertices[D*D-D-D+i-1], &n2);
        normal(glop_vertices[D*D-D+i+1], glop_vertices[D*D-D+i], glop_vertices[D*D-D-D+i], &n3);
        glop_normals[D*D-D+i].x = n1.x + n2.x + n3.x;
        glop_normals[D*D-D+i].y = n1.y + n2.y + n3.y;
        glop_normals[D*D-D+i].z = n1.z + n2.z + n3.z;
        normalize(&glop_normals[D*D-D+i]);
    }

    // left normals
    for (int i = 1; i < D-1; i++)
    {
        normal(glop_vertices[D*i+D+1], glop_vertices[D*i+D], glop_vertices[D*i], &n1);
        normal(glop_vertices[D*i+1], glop_vertices[D*i+D+1], glop_vertices[D*i], &n2);
        normal(glop_vertices[D*i+1], glop_vertices[D*i], glop_vertices[D*i-D], &n3);
        glop_normals[D*i].x = n1.x + n2.x + n3.x;
        glop_normals[D*i].y = n1.y + n2.y + n3.y;
        glop_normals[D*i].z = n1.z + n2.z + n3.z;
        normalize(&glop_normals[D*i]);
    }

    // right normals
    for (int i = 1; i < D-1; i++)
    {
        normal(glop_vertices[D*i+D-1], glop_vertices[D*i+D-2], glop_vertices[D*i-2], &n1);
        normal(glop_vertices[D*i-1], glop_vertices[D*i+D-1], glop_vertices[D*i-2], &n2);
        normal(glop_vertices[D*i+D-1], glop_vertices[D*i+D+D-1], glop_vertices[D*i+D-2], &n3);
        glop_normals[D*i+D-1].x = n1.x + n2.x + n3.x;
        glop_normals[D*i+D-1].y = n1.y + n2.y + n3.y;
        glop_normals[D*i+D-1].z = n1.z + n2.z + n3.z;
        normalize(&glop_normals[D*i+D-1]);
    }

    // in-side normals
    for (int h = 1; h < D-1; h++)
    {
        for (int w = 1; w < D-1; w++)
        {
            int i = h*D + w;
            normal(glop_vertices[i+D+1], glop_vertices[i+D], glop_vertices[i], &n1);
            normal(glop_vertices[i+1], glop_vertices[i+D+1], glop_vertices[i], &n2);
            normal(glop_vertices[i], glop_vertices[i-1], glop_vertices[i-D-1], &n3);
            normal(glop_vertices[i-D], glop_vertices[i], glop_vertices[i-D-1], &n4);
            normal(glop_vertices[i], glop_vertices[i+D], glop_vertices[i-1], &n5);
            normal(glop_vertices[i+1], glop_vertices[i], glop_vertices[i-D], &n6);
            glop_normals[i].x = n1.x + n2.x + n3.x + n4.x + n5.x + n6.x;
            glop_normals[i].y = n1.y + n2.y + n3.y + n4.y + n5.y + n6.y;
            glop_normals[i].z = n1.z + n2.z + n3.z + n4.z + n5.z + n6.z;
            normalize(&glop_normals[i]);
        }
    }

    unsigned int size = (D-1)*(D-1)*6;
    super_vertices = new vertex_t[size];
    super_normals = new vertex_t[size];
    for (int h = 0; h < D-1; h++)
    {
        for (int w = 0; w < D-1; w++)
        {
            int i = h*D + w;
            int x = h*(D-1)*6 + w*6;
            super_vertices[x] = glop_vertices[i+D+1];
            super_vertices[x+1] = glop_vertices[i+D];
            super_vertices[x+2] = glop_vertices[i];
            super_vertices[x+3] = glop_vertices[i+1];
            super_vertices[x+4] = glop_vertices[i+D+1];
            super_vertices[x+5] = glop_vertices[i];
            super_normals[x] = glop_normals[i+D+1];
            super_normals[x+1] = glop_normals[i+D];
            super_normals[x+2] = glop_normals[i];
            super_normals[x+3] = glop_normals[i+1];
            super_normals[x+4] = glop_normals[i+D+1];
            super_normals[x+5] = glop_normals[i];
        }
    }

    glGenBuffers(1, &triangleVBO);
    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
    glBufferData(GL_ARRAY_BUFFER, size*sizeof(vertex_t), super_vertices, GL_STATIC_DRAW);
    glVertexPointer(3, GL_FLOAT, 0, NULL);


    glGenBuffers(1, &normalsVOB);
    glBindBuffer(GL_ARRAY_BUFFER, normalsVOB);
    glBufferData(GL_ARRAY_BUFFER, size*sizeof(vertex_t), super_normals, GL_STATIC_DRAW);
    glNormalPointer(GL_FLOAT, 0, 0);



    quad_texture_uv *texcoords;
    texcoords = new quad_texture_uv[(D-1)*(D-1)];
    float dim = 1.0f / D;
    for (int h = 0; h < D-1; h++)
    {
        for (int w = 0; w < D-1; w++)
        {
            int i = h*(D-1) + w;
            texcoords[i].a = dim*(w+1);     //x1
            texcoords[i].b = dim*(h+1);     //y1
            texcoords[i].c = dim*w;         //x2
            texcoords[i].d = dim*(h+1);     //y2
            texcoords[i].e = dim*w;         //x3
            texcoords[i].f = dim*h;         //y3
            texcoords[i].a2 = dim*(w+1);    //x4
            texcoords[i].b2 = dim*h;        //y4
            texcoords[i].c2 = dim*(w+1);    //x5
            texcoords[i].d2 = dim*(h+1);    //y5
            texcoords[i].e2 = dim*w;        //x6
            texcoords[i].f2 = dim*h;        //y6
        }
    }
    glGenBuffers(1, &texture_coordVBO);
    glBindBuffer(GL_ARRAY_BUFFER, texture_coordVBO);
    glBufferData(GL_ARRAY_BUFFER, (D-1)*(D-1)*sizeof(quad_texture_uv), texcoords , GL_STATIC_DRAW);
    glTexCoordPointer(2, GL_FLOAT, 0, 0);


    delete[] glop_vertices;
    delete[] glop_normals;
    delete[] super_vertices;
    delete[] super_normals;
}
