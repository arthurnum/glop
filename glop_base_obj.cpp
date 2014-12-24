#include "glop_base_obj.h"

unsigned char GLOPBaseObj::_Id = 0;

void GLOPBaseObj::Load(const char* filename)
{
    std::ifstream file;
    file.open(filename);
    file.seekg (0, std::ios::end);
    long fileSize = file.tellg();
    file.seekg (0, std::ios::beg);

    vertex_v *vertices;
    vertices = new vertex_v[fileSize];
    unsigned int vertices_count = 0;

    vertex_v *normals;
    normals = new vertex_v[fileSize];
    unsigned int normals_count = 0;

    unsigned int * vertex_indices;
    vertex_indices = new unsigned int [fileSize];
    unsigned int * normal_indices;
    normal_indices = new unsigned int [fileSize];
    unsigned int indices_count = 0;

    faces = 0;

    std::string line; 
    while (getline(file, line))
    {
        if (line[0] == 'v' && line[1] == ' ')
        {
            line[0] = ' ';
            float x, y, z;
            sscanf(line.c_str(), "%f %f %f", &x, &y, &z);
            vertices[vertices_count].x = x;
            vertices[vertices_count].y = y;
            vertices[vertices_count].z = z;
            vertices_count++;
        }
        if (line[0] == 'v' && line[1] == 'n')
        {
            line[0] = ' ';
            line[1] = ' ';
            float x, y, z;
            sscanf(line.c_str(), "%f %f %f", &x, &y, &z);
            normals[normals_count].x = x;
            normals[normals_count].y = y;
            normals[normals_count].z = z;
            normals_count++;
        }
        if (line[0] == 'f')
        {
            line[0] = ' ';
            unsigned int v1, vn1, v2, vn2, v3, vn3;
            sscanf(line.c_str(), "%d//%d %d//%d %d//%d", &v1, &vn1, &v2, &vn2, &v3, &vn3);
            vertex_indices[indices_count] = v1 - 1;
            vertex_indices[indices_count+1] = v2 - 1;
            vertex_indices[indices_count+2] = v3 - 1;
            normal_indices[indices_count] = vn1 - 1;
            normal_indices[indices_count+1] = vn2 - 1;
            normal_indices[indices_count+2] = vn3 - 1;
            indices_count += 3;
            faces++;
        }
    }

    vertex_v *super_vertices;
    vertex_v *super_normals;
    super_vertices = new vertex_v [indices_count];
    super_normals = new vertex_v [indices_count];
    for (unsigned int i = 0; i < indices_count; i++)
    {
        super_vertices[i] = vertices[vertex_indices[i]];
        super_normals[i] = normals[normal_indices[i]];
    }

    glGenBuffers(1, &verticesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
    glBufferData(GL_ARRAY_BUFFER, indices_count*sizeof(vertex_v), super_vertices, GL_STATIC_DRAW);
    glVertexPointer(3, GL_FLOAT, 0, NULL);


    glGenBuffers(1, &normalsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
    glBufferData(GL_ARRAY_BUFFER, indices_count*sizeof(vertex_v), super_normals, GL_STATIC_DRAW);
    glNormalPointer(GL_FLOAT, 0, 0);

    file.close();
    delete[] vertices;
    delete[] normals;
    delete[] vertex_indices;
    delete[] normal_indices;
    delete[] super_vertices;
    delete[] super_normals;
    printf("Create BaseObj with VBO %d\n", verticesVBO);
}


void GLOPBaseObj::Draw()
{
    glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
    glVertexPointer(3, GL_FLOAT, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
    glNormalPointer(GL_FLOAT, 0, 0);

    float colorAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f }; glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, colorAmbient);
    float colorDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f }; glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, colorDiffuse);
    float colorSpecular[] = { 0.0f, 0.0f, 0.0f, 1.0f }; glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, colorSpecular);
    glMateriali(GL_FRONT, GL_SHININESS, 0);

	if (selected > 0)
	{
		float colorAmbientGold[] = { 0.24f, 0.19f, 0.07f, 1.0f }; glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, colorAmbientGold);
		float colorDiffuseGold[] = { 0.75f, 0.60f, 0.22f, 1.0f }; glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, colorDiffuseGold);
		float colorSpecularGold[] = { 0.62f, 0.55f, 0.36f, 1.0f }; glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, colorSpecularGold);
		glMaterialf(GL_FRONT, GL_SHININESS, 0.4f * 128.0f);
	}

    glDrawArrays(GL_TRIANGLES, 0, faces*3);
}


void GLOPBaseObj::DrawPick()
{
    glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
    glVertexPointer(3, GL_FLOAT, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
    glNormalPointer(GL_FLOAT, 0, 0);
    glColor3ub(id, id, id);
    glDrawArrays(GL_TRIANGLES, 0, faces*3);
}


GLOPBaseObj::GLOPBaseObj()
{
    _Id++;
    id = _Id;
    selected = 0;
}


void GLOPBaseObj::MouseOver(unsigned char* pixel)
{
    selected = 0;
    if (pixel[0] == id && pixel[1] == id && pixel[2] == id) { selected = 1; }
}
