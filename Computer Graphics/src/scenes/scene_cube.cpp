#include "scenes/scene_cube.h"

#include "_time.h"
#include "vec3.h"

void scene_cube::init() {
    std::vector<cgmath::vec3> positions{
        {-3, 3, 3},  {3, 3, 3},   {3, -3, 3},   {-3, -3, 3},

        {3, 3, 3},   {3, 3, -3},  {3, -3, -3},  {3, -3, 3},

        {3, 3, -3},  {-3, 3, -3}, {-3, -3, -3}, {3, -3, -3},

        {-3, 3, -3}, {-3, 3, 3},  {-3, -3, 3},  {-3, -3, -3},

        {-3, 3, -3}, {3, 3, -3},  {3, 3, 3},    {-3, 3, 3},

        {-3, -3, 3}, {3, -3, 3},  {3, -3, -3},  {-3, -3, -3}};

    std::vector<cgmath::vec3> u_colors{
        {98, 182, 203}, {75, 163, 145},  {165, 66, 129},
        {61, 59, 107},  {140, 139, 142}, {247, 125, 76},
    };
    std::vector<cgmath::vec3> colors;
    colors.reserve(positions.size());
    for (auto& c : u_colors) {
        for (int i = 0; i < 4; ++i) {
            colors.emplace_back(c / 255.0f);
        }
    }

    std::vector<unsigned int> order{3, 2, 0, 0, 2, 1};
    std::vector<unsigned int> indices;
    indices.reserve(n_points);
    for (int i = 0; i < u_colors.size(); i++) {
        for (auto o : order) {
            indices.push_back(4 * i + o);
        }
    }
    glGenVertexArrays(1, &vao);
    addBuffer(vao, positionsBuffer, positions);
    addBuffer(vao, colorsBuffer, colors, 1);
    addIndexBuffer(vao, indicesBuffer, indices);

    shader_program =
        createShaderProgram({{"shaders/mvp.vert", GL_VERTEX_SHADER},
                             {"shaders/solid_color.frag", GL_FRAGMENT_SHADER}},
                            {"position", "color"});

    mvp_location = glGetUniformLocation(shader_program, "mvpMat");

    view = cgmath::mat4::inverse(cgmath::mat4::translate({0, 0, 10}));

    fov = 60;
    _near = 1;
    _far = 1000;
    projection = cgmath::mat4::perspective(
        float(screen::width) / screen::height, fov, _near, _far);
}

void scene_cube::mainLoop() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shader_program);
    glBindVertexArray(vao);

    float time = time::elapsed_time().count();
    model =
        cgmath::mat4::modelMatrix({}, {}, {30 * time, 60 * time, 30 * time});
    glUniformMatrix4fv(mvp_location, 1, GL_FALSE,
                       &cgmath::mat4::mvpMatrix(model, view, projection)[0][0]);
    glDrawElements(GL_TRIANGLES, n_points, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
    glUseProgram(0);
}
