#include "scenes/scene_texture.h"

#include <IL/il.h>
#include "_time.h"

void scene_texture::init() {
    std::vector<cgmath::vec3> positions{
        {-3, 3, 3},  {3, 3, 3},   {3, -3, 3},   {-3, -3, 3},

        {3, 3, 3},   {3, 3, -3},  {3, -3, -3},  {3, -3, 3},

        {3, 3, -3},  {-3, 3, -3}, {-3, -3, -3}, {3, -3, -3},

        {-3, 3, -3}, {-3, 3, 3},  {-3, -3, 3},  {-3, -3, -3},

        {-3, 3, -3}, {3, 3, -3},  {3, 3, 3},    {-3, 3, 3},

        {-3, -3, 3}, {3, -3, 3},  {3, -3, -3},  {-3, -3, -3}};

    std::vector<unsigned int> order{3, 2, 0, 0, 2, 1};
    std::vector<unsigned int> indices;
    indices.reserve(n_points);
    std::vector<cgmath::vec2> single_side{{0, 1}, {1, 1}, {1, 0}, {0, 0}};
    std::vector<cgmath::vec2> texture_positions;
    texture_positions.reserve(positions.size());
    for (int i = 0; i < 6; i++) {
        for (auto o : order) {
            indices.push_back(4 * i + o);
        }
        for (auto& point : single_side) {
            texture_positions.emplace_back(point);
        }
    }

    glGenVertexArrays(1, &vao);
    addBuffer(vao, positionsBuffer, positions);
    addBuffer(vao, textureBuffer, texture_positions, 1);
    addIndexBuffer(vao, indicesBuffer, indices);

    shader_program =
        createShaderProgram({{"shaders/cube_texture.vert", GL_VERTEX_SHADER},
                             {"shaders/cube_texture.frag", GL_FRAGMENT_SHADER}},
                            {"position", "texture_pos"});

    mvp_location = glGetUniformLocation(shader_program, "mvpMat");

    glUseProgram(shader_program);

    addTexture("images/crate.png", crate_texture);
    addTexture("images/pig.png", pig_texture);
    glUniform1i(glGetUniformLocation(shader_program, "crate_texture"), 0);
    glUniform1i(glGetUniformLocation(shader_program, "pig_texture"), 1);

    glUseProgram(0);

    view = cgmath::mat4::inverse(
        cgmath::mat4::modelMatrix({0, 10, 20}, {}, {-30, 0, 0}));

    fov = 60;
    _near = 1;
    _far = 1000;
    projection = cgmath::mat4::perspective(
        float(screen::width) / screen::height, fov, _near, _far);

    // floor
    // todo: change this to a model thing and create a new model matrix
    std::vector<cgmath::vec3> floorPos{
        {-20, -10, 20}, {20, -10, 20}, {-20, -10, -20}, {20, -10, -20}};
    std::vector<cgmath::vec2> texPos{{0, 0}, {1, 0}, {0, 1}, {1, 1}};
    glGenVertexArrays(1, &floor_vao);
    addBuffer(floor_vao, floor_pos, floorPos);
    addBuffer(floor_vao, floorTextureBuffer, texPos, 1);
    floorShader = createShaderProgram(
        {{"shaders/single_texture.vert", GL_VERTEX_SHADER},
         {"shaders/single_texture.frag", GL_FRAGMENT_SHADER}},
        {"position", "texture_pos"});

    mvp_location = glGetUniformLocation(floorShader, "mvpMat");

    glUseProgram(floorShader);

    addTexture("images/floor.jpg", floor_texture);
    glUniform1i(glGetUniformLocation(floorShader, "tex"), 0);

    glUseProgram(0);
}

void scene_texture::mainLoop() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shader_program);
    glBindVertexArray(vao);

    float time = time::elapsed_time().count();
    model =
        cgmath::mat4::modelMatrix({}, {}, {30 * time, 60 * time, 30 * time});
    glUniformMatrix4fv(mvp_location, 1, GL_FALSE,
                       &cgmath::mat4::mvpMatrix(model, view, projection)[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, crate_texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, pig_texture);

    glDrawElements(GL_TRIANGLES, n_points, GL_UNSIGNED_INT, nullptr);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindVertexArray(0);
    glUseProgram(0);

    glUseProgram(floorShader);
    glBindVertexArray(floor_vao);

    glUniformMatrix4fv(
        mvp_location, 1, GL_FALSE,
        &cgmath::mat4::mvpMatrix(cgmath::mat4(1), view, projection)[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, floor_texture);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindVertexArray(0);
    glUseProgram(0);
}

scene_texture::~scene_texture() {}
