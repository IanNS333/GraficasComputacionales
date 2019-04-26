#pragma once

#include <IL/il.h>
#include <glad/glad.h>
#include <string>
#include <vector>
#include "mat4.h"
#include "renderable.h"
#include "vec2.h"
#include "vec3.h"

class phong_cube {
    static const std::vector<cgmath::vec3> vertices, normals;
    static const std::vector<cgmath::vec2> textureCoords;
    static const std::vector<unsigned int> indices;

    GLuint vao, texBo, vertBo, idxBo, normBo;
    GLuint shader;

    cgmath::mat4 model, view, projection;
    uniform_variable<cgmath::vec3> lightColor, lightPosition, cameraPosition;
    uniform_variable<cgmath::mat3> norm;
    uniform_variable<cgmath::mat4> modelV, vp;

  public:
    phong_cube(const std::string& textureFilename,
         const cgmath::mat4& model,
         const cgmath::mat4& view,
         const cgmath::mat4& projection,
         GLuint shader,
         GLint modelLocation,
         GLint vpLocation,
         GLint normLocation,
         uniform_variable<cgmath::vec3> lightColor,
         uniform_variable<cgmath::vec3> lightPosition,
         uniform_variable<cgmath::vec3> cameraPosition);
    void render();
    void setModel(const cgmath::mat4& mat);
    void setView(const cgmath::mat4& mat);
    void setProyection(const cgmath::mat4& mat);
};
