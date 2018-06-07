#ifndef PLANET_H
#define PLANET_H

#include <glm/vec3.hpp>
#include <cmath>
#include <vector>

#include "Shader.h"
#include "Camera.h"

class Planet
{
    public:
        Planet(float radius);
        virtual ~Planet();
        void Update(Camera& camera);
        void Render(Camera& camera);

        bool update;
    private:
        void CreateBaseMesh();
        void Subdivide(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, Camera& camera, int recursionDepth = 0);

        glm::vec3 position;
        float radius;
        int baseRecursionDepth;
        int maxRecursionDepth;
        //double* vertices;
        std::vector<float> vertices;
        std::vector<int> indices;

        unsigned int vao, vbo, ebo;

        static char* VertexSrc;
        static char* FragmentSrc;
};

#endif // PLANET_H
