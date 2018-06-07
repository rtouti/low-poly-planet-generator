#include <GL/gl3w.h>
#include "Planet.h"

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <algorithm>


Planet::Planet(float radius)
: radius(radius), baseRecursionDepth(15), maxRecursionDepth(10), update(true)
{
    if(Shader::Get("planet") == nullptr)
        Shader::Add("planet", Planet::VertexSrc, Planet::FragmentSrc);

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    //CreateBaseMesh();
}

Planet::~Planet()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

/*void Planet::CreateBaseMesh(){
    float t = (1.0+std::sqrt(5.0)) / 2.0;

    float verts[] = {
        -1, t, 0, 	1, t, 0, 	-1, -t, 0, 	1, -t, 0,
        0, -1, t, 	0, 1, t,	0, -1, -t, 	0, 1, -t,
        t, 0, -1, 	t, 0, 1, 	-t, 0, -1, 	-t, 0, 1
    };
    //std::for_each(&verts[0], &verts[35], [&](double& v){
    //    v *= radius;
    //});

    int indices[] = {
        0, 11, 5, 	0, 5, 1, 	0, 1, 7, 	0, 7, 10, 	0, 10, 11,
        1, 5, 9, 	5, 11, 4,	11, 10, 2,	10, 7, 6,	7, 1, 8,
        3, 9, 4, 	3, 4, 2,	3, 2, 6,	3, 6, 8,	3, 8, 9,
        4, 9, 5, 	2, 4, 11,	6, 2, 10,	8, 6, 7,	9, 8, 1
    };

    for(int i = 0; i < 20; i++){
        Subdivide(
            glm::vec3(verts[3*indices[3*i]], verts[3*indices[3*i]+1], verts[3*indices[3*i]+2]),
            glm::vec3(verts[3*indices[3*i+1]], verts[3*indices[3*i+1]+1], verts[3*indices[3*i+1]+2]),
            glm::vec3(verts[3*indices[3*i+2]], verts[3*indices[3*i+2]+2], verts[3*indices[3*i+2]+2])
        );
    }

    for(int i = 0; i < vertices.size(); i++){
        std::cout << vertices[i] << std::endl;
    }
}*/

void Planet::Update(Camera& camera){
    if(!update)
        return;

    double t = (1.0+std::sqrt(5.0)) / 2.0;
    double verts[] = {
        - 1, t, 0, 	1, t, 0, 	- 1, - t, 0, 	1, - t, 0,
		 0, - 1, t, 	0, 1, t,	0, - 1, - t, 	0, 1, - t,
		 t, 0, - 1, 	t, 0, 1, 	- t, 0, - 1, 	- t, 0, 1
    };
    int indices[] = {
        0, 11, 5, 	0, 5, 1, 	0, 1, 7, 	0, 7, 10, 	0, 10, 11,
        1, 5, 9, 	5, 11, 4,	11, 10, 2,	10, 7, 6,	7, 1, 8,
        3, 9, 4, 	3, 4, 2,	3, 2, 6,	3, 6, 8,	3, 8, 9,
        4, 9, 5, 	2, 4, 11,	6, 2, 10,	8, 6, 7,	9, 8, 1
    };

    vertices.clear();

    for(int i = 0; i < 20; i++){
        Subdivide(
            glm::vec3(verts[3*indices[3*i]], verts[3*indices[3*i]+1], verts[3*indices[3*i]+2]),
            glm::vec3(verts[3*indices[3*i+1]], verts[3*indices[3*i+1]+1], verts[3*indices[3*i+1]+2]),
            glm::vec3(verts[3*indices[3*i+2]], verts[3*indices[3*i+2]+1], verts[3*indices[3*i+2]+2]),
            camera,
            0
        );
    }
    double data[] = {

    };
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(int), indices.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Planet::Render(Camera& camera){
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

    Shader* shader = Shader::Get("planet");
    shader->Use();
    shader->SetUniformMat4fv("uProj", glm::value_ptr(camera.GetProjectionMatrix()));
    shader->SetUniformMat4fv("uView", glm::value_ptr(camera.GetViewMatrix()));
    shader->SetUniformMat4fv("uModel", glm::value_ptr(model));
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size()/3);
    glBindVertexArray(0);
}

void Planet::Subdivide(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, Camera& camera, int recursionDepth){
    glm::vec3 p11 = radius*glm::normalize(p1);
    glm::vec3 p22 = radius*glm::normalize(p2);
    glm::vec3 p33 = radius*glm::normalize(p3);

    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    glm::vec3 center = radius*glm::normalize((p11+p22+p33)/3.0f);
    center = glm::vec3(model*glm::vec4(center, 1.0));

    //glm::vec3 centerToCamera = camera.GetPosition()-center;
    //if(glm::dot(center-position, centerToCamera) < 0)
    //    return;

    float dist = glm::distance(camera.GetPosition(), center);
    if(recursionDepth >= 3 && (dist/glm::length(p11-p22) >= 10.0f || recursionDepth == baseRecursionDepth)){
        vertices.push_back(p11.x);
        vertices.push_back(p11.y);
        vertices.push_back(p11.z);
        vertices.push_back(p22.x);
        vertices.push_back(p22.y);
        vertices.push_back(p22.z);
        vertices.push_back(p33.x);
        vertices.push_back(p33.y);
        vertices.push_back(p33.z);

        return;
    }

    glm::vec3 p1p2 = glm::mix(p11, p22, 0.5);
    glm::vec3 p1p3 = glm::mix(p11, p33, 0.5);
    glm::vec3 p2p3 = glm::mix(p22, p33, 0.5);

    //glm::vec3 cameraPosition = camera.GetPosition();
    Subdivide(p11, p1p2, p1p3, camera, recursionDepth+1);
    Subdivide(p1p2, p22, p2p3, camera, recursionDepth+1);
    Subdivide(p1p3, p2p3, p33, camera, recursionDepth+1);
    Subdivide(p1p2, p2p3, p1p3, camera, recursionDepth+1);
}

char* Planet::VertexSrc =
"#version 330 core\n"
"\n"
"layout(location=0) in vec3 aPos;"
"\n"
"out float vHeight;\n"
"\n"
"uniform mat4 uProj;\n"
"uniform mat4 uView;\n"
"uniform mat4 uModel;\n"
"\n"
"float rand2d(float x, float y){\n"
"	return fract(sin(dot(vec2(x, y) ,vec2(12.9898, 78.233))) * 43758.5453);\n"
"}\n"
"\n"
"float rand3d(float x, float y, float z){\n"
"	return fract(sin(dot(vec3(x, y, z) ,vec3(12.9898, 78.233, 54.734))) * 43758.5453);\n"
"}\n"
"\n"
"float fade(float t){\n"
"	return t*t*(3.0-2.0*t);\n"
"}\n"
"\n"
"float noise2d(float x, float y){\n"
"	float xf = fade(fract(x));\n"
"	float yf = fade(fract(y));\n"
"	float xi = floor(x);\n"
"	float yi = floor(y);\n"
"\n"
"	return mix(\n"
"			mix(rand2d(xi, yi), rand2d(xi+1.0, yi), xf),\n"
"			mix(rand2d(xi, yi+1.0), rand2d(xi+1.0, yi+1.0), xf),\n"
"			yf\n"
"	);\n"
"}\n"
"\n"
"float noise3d(float x, float y, float z){\n"
"	float xf = fade(fract(x));\n"
"	float yf = fade(fract(y));\n"
"	float zf = fade(fract(z));\n"
"	float xi = floor(x);\n"
"	float yi = floor(y);\n"
"	float zi = floor(z);\n"
"\n"
"	return mix(\n"
"		mix(\n"
"			mix(rand3d(xi, yi, zi), rand3d(xi+1.0, yi, zi), xf),\n"
"			mix(rand3d(xi, yi+1.0, zi), rand3d(xi+1.0, yi+1.0, zi), xf),\n"
"			yf\n"
"		),\n"
"		mix(\n"
"			mix(rand3d(xi, yi, zi+1.0), rand3d(xi+1.0, yi, zi+1.0), xf),\n"
"			mix(rand3d(xi, yi+1.0, zi+1.0), rand3d(xi+1.0, yi+1.0, zi+1.0), xf),\n"
"			yf\n"
"		),\n"
"		zf\n"
"	);\n"
"}\n"
"\n"
"void main(){\n"
"   //vec4 position = uModel*vec4(aPos, 1.0f);\n"
"   /*float h = 0.0f;\n"
"   float amp = 20.0f;\n"
"   float freq = 0.001f;\n"
"   for(int o = 0; o < 4; o++){\n"
"       float v = amp*noise3d(freq*aPos.x, freq*aPos.y, freq*aPos.z);\n"
"       v *= 2.0f;\n"
"       v -= 1.0f;\n"
"       h += v;\n"
"       amp *= 0.5f;\n"
"       freq *= 5.0f;\n"
"   }*/\n"
"   vec3 position = (2000.0/*+h*/)*normalize(aPos);\n"
"   gl_Position = uProj*uView*uModel*vec4(position, 1.0f);\n"
"   vHeight = 1.0;//h;\n"
"}";

char* Planet::FragmentSrc =
"#version 330 core\n"
"\n"
"in float vHeight;\n"
"\n"
"out vec4 vColor;"
"\n"
"void main(){\n"
"   float a = 1.0f - vHeight/20.0f;\n"
"   if(vHeight <= 17.0f)\n"
"       vColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);\n"
"   else if(vHeight <= 18.0f)\n"
"       vColor = vec4(0.1f, 1.0f, 1.0f, a);\n"
"   else\n"
"       vColor = vec4(1.0f, 1.0f, 1.0f, a);\n"
"}";
