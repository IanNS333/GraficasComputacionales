#include "scenes/scene_final.h"

#include <IL/il.h>
#include "_time.h"

namespace cube {
    const std::vector<cgmath::vec3> vertices{
        {-1, 1, 1},   {1, 1, 1},   {1, -1, 1},  {-1, -1, 1}, {1, 1, 1},
        {1, 1, -1},   {1, -1, -1}, {1, -1, 1},  {1, 1, -1},  {-1, 1, -1},
        {-1, -1, -1}, {1, -1, -1}, {-1, 1, -1}, {-1, 1, 1},  {-1, -1, 1},
        {-1, -1, -1}, {-1, 1, -1}, {1, 1, -1},  {1, 1, 1},   {-1, 1, 1},
        {-1, -1, 1},  {1, -1, 1},  {1, -1, -1}, {-1, -1, -1}};
    const std::vector<cgmath::vec3> normals{
        {0, 0, 1},  {0, 0, 1},  {0, 0, 1},  {0, 0, 1},  {1, 0, 0},  {1, 0, 0},
        {1, 0, 0},  {1, 0, 0},  {0, 0, -1}, {0, 0, -1}, {0, 0, -1}, {0, 0, -1},
        {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {0, 1, 0},  {0, 1, 0},
        {0, 1, 0},  {0, 1, 0},  {0, -1, 0}, {0, -1, 0}, {0, -1, 0}, {0, -1, 0}};
    const std::vector<cgmath::vec2> textureCoords{
        {0, 1}, {1, 1}, {1, 0}, {0, 0}, {0, 1}, {1, 1}, {1, 0}, {0, 0},
        {0, 1}, {1, 1}, {1, 0}, {0, 0}, {0, 1}, {1, 1}, {1, 0}, {0, 0},
        {0, 1}, {1, 1}, {1, 0}, {0, 0}, {0, 1}, {1, 1}, {1, 0}, {0, 0},
    };
    const std::vector<unsigned int> indices{
        3,  2,  0,  0,  2,  1,  7,  6,  4,  4,  6,  5,  11, 10, 8,  8,  10, 9,
        15, 14, 12, 12, 14, 13, 19, 18, 16, 16, 18, 17, 23, 22, 20, 20, 22, 21};
};  // namespace cube

void scene_final::init() {
    // cgmath::vec3 cameraPosition{0, 10, 20};
    // view = cgmath::mat4::inverse(
    //     cgmath::mat4::modelMatrix(cameraPosition, {}, {-30, 0, 0}));

    camera.init();
    view = camera.lookAt();

    fov = 60;
    _near = 1;
    _far = 1000;
    projection = cgmath::mat4::perspective(
        float(screen::width) / screen::height, fov, _near, _far);

    shader_program = createShaderProgram(
        {{"shaders/final.vert", GL_VERTEX_SHADER},
         {"shaders/final.frag", GL_FRAGMENT_SHADER}},
        {"position", "texture_pos", "normal", "offset", "texture_offset"});

    vp = {glGetUniformLocation(shader_program, "vp"), projection * view};
    lightColor = {glGetUniformLocation(shader_program, "lightColor"), {1}};
    lightPosition = {glGetUniformLocation(shader_program, "lightPosition"),
                     {-10, 10, 10}};

    glGenVertexArrays(1, &vao);
    addBuffer(vao, positions, cube::vertices);
    addBuffer(vao, texPos, cube::textureCoords, 1);
    addBuffer(vao, normals, cube::normals, 2);
    std::vector<cgmath::vec3> tmp{{0}, {5}, {-5}};
    addBuffer(vao, offsets, tmp, 3, 1);
    std::vector<float> tmp2{0, 0, 0};
    addBuffer(vao, texOffsets, tmp2, 4, 1);

    addIndexBuffer(vao, indices, cube::indices);
    addTexture("images/final.png", texture);
    glUniform1i(glGetUniformLocation(shader_program, "tex"), 0);
}

void scene_final::awake() {
    scene_mvp::awake();
    glfwSetInputMode(screen::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    camera.init();
}

void scene_final::sleep() {
    scene_mvp::sleep();
    glfwSetInputMode(screen::window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void scene_final::mainLoop() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shader_program);
    glBindVertexArray(vao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    if (camera.move()) {
        vp.setValue(projection * camera.lookAt());
    }

    vp.send();
    lightColor.send();
    lightPosition.send();

    glDrawElementsInstanced(GL_TRIANGLES, cube::indices.size(), GL_UNSIGNED_INT,
                            nullptr, 3);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindVertexArray(0);
    glUseProgram(0);
}

void scene_final::resize(int width, int height) {
    scene_mvp::resize(width, height);
    vp.setValue(projection * view);
}

void scene_final::keysDown(int key) {
    switch (key) {
        case GLFW_KEY_W:
            camera.dz -= camera.speed;
            break;
        case GLFW_KEY_S:
            camera.dz += camera.speed;
            break;
        case GLFW_KEY_A:
            camera.dx -= camera.speed;
            break;
        case GLFW_KEY_D:
            camera.dx += camera.speed;
            break;
        default:
            break;
    }
}

void scene_final::keysUp(int key) {
    switch (key) {
        case GLFW_KEY_W:
            camera.dz += camera.speed;
            break;
        case GLFW_KEY_S:
            camera.dz -= camera.speed;
            break;
        case GLFW_KEY_A:
            camera.dx += camera.speed;
            break;
        case GLFW_KEY_D:
            camera.dx -= camera.speed;
            break;
        default:
            break;
    }
}

void scene_final::passiveMotion(float x, float y) {
    camera.rotate(x, y);
}
