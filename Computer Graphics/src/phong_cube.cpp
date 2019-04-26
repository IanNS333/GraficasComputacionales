#include "phong_cube.h"

#include "mat3.h"
#include "_util.h"

const std::vector<cgmath::vec3> phong_cube::vertices{
    {-1, 1, 1},   {1, 1, 1},   {1, -1, 1},  {-1, -1, 1}, {1, 1, 1},
    {1, 1, -1},   {1, -1, -1}, {1, -1, 1},  {1, 1, -1},  {-1, 1, -1},
    {-1, -1, -1}, {1, -1, -1}, {-1, 1, -1}, {-1, 1, 1},  {-1, -1, 1},
    {-1, -1, -1}, {-1, 1, -1}, {1, 1, -1},  {1, 1, 1},   {-1, 1, 1},
    {-1, -1, 1},  {1, -1, 1},  {1, -1, -1}, {-1, -1, -1}};
const std::vector<cgmath::vec3> phong_cube::normals{
    {0, 0, 1},  {0, 0, 1},  {0, 0, 1},  {0, 0, 1},  {1, 0, 0},  {1, 0, 0},
    {1, 0, 0},  {1, 0, 0},  {0, 0, -1}, {0, 0, -1}, {0, 0, -1}, {0, 0, -1},
    {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {0, 1, 0},  {0, 1, 0},
    {0, 1, 0},  {0, 1, 0},  {0, -1, 0}, {0, -1, 0}, {0, -1, 0}, {0, -1, 0}};
const std::vector<cgmath::vec2> phong_cube::textureCoords{
    {0, 1}, {1, 1}, {1, 0}, {0, 0}, {0, 1}, {1, 1}, {1, 0}, {0, 0},
    {0, 1}, {1, 1}, {1, 0}, {0, 0}, {0, 1}, {1, 1}, {1, 0}, {0, 0},
    {0, 1}, {1, 1}, {1, 0}, {0, 0}, {0, 1}, {1, 1}, {1, 0}, {0, 0},
};
const std::vector<unsigned int> phong_cube::indices{
    3,  2,  0,  0,  2,  1,  7,  6,  4,  4,  6,  5,  11, 10, 8,  8,  10, 9,
    15, 14, 12, 12, 14, 13, 19, 18, 16, 16, 18, 17, 23, 22, 20, 20, 22, 21};

phong_cube::phong_cube(const std::string& textureFilename,
           const cgmath::mat4& model,
           const cgmath::mat4& view,
           const cgmath::mat4& projection,
           GLuint shader,
           GLint modelLocation,
           GLint vpLocation,
           GLint normLocation,
           uniform_variable<cgmath::vec3> lightColor,
           uniform_variable<cgmath::vec3> lightPosition,
           uniform_variable<cgmath::vec3> cameraPosition)
    : model{model},
      view{view},
      projection{projection},
      shader{shader},
      modelV{modelLocation, model},
      vp{vpLocation, projection * view},
      norm{normLocation, cgmath::mat3::normalMatrix(model)},
      lightColor{lightColor},
      lightPosition{lightPosition},
      cameraPosition{cameraPosition} {
    glGenVertexArrays(1, &vao);

    addBuffer(vao, vertBo, vertices);
    addBuffer(vao, texBo, textureCoords, 1);
    addBuffer(vao, normBo, normals, 2);

    addIndexBuffer(vao, idxBo, indices);

    addTexture(textureFilename, texBo);
}

void phong_cube::render() {
    glUseProgram(shader);
    glBindVertexArray(vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texBo);

    vp.send();
    modelV.send();
    norm.send();
    lightColor.send();
    lightPosition.send();
    cameraPosition.send();

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

void phong_cube::setModel(const cgmath::mat4& mat) {
    model = mat;
    modelV.setValue(model);
    norm.setValue(cgmath::mat3::normalMatrix(model));
}

void phong_cube::setView(const cgmath::mat4& mat) {
    view = mat;
    vp.setValue(projection * view);
}

void phong_cube::setProyection(const cgmath::mat4& mat) {
    projection = mat;
    vp.setValue(projection * view);
}
