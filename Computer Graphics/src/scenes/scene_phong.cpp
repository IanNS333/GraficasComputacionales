#include "scenes/scene_phong.h"

#include <IL/il.h>
#include "_time.h"

#include "phong_cube.h"

void scene_phong::init() {
    cgmath::vec3 cameraPosition{0, 10, 20};
    view = cgmath::mat4::inverse(
        cgmath::mat4::modelMatrix(cameraPosition, {}, {-30, 0, 0}));

    fov = 60;
    _near = 1;
    _far = 1000;
    projection = cgmath::mat4::perspective(
        float(screen::width) / screen::height, fov, _near, _far);

    shader_program =
        createShaderProgram({{"shaders/phong.vert", GL_VERTEX_SHADER},
                             {"shaders/phong.frag", GL_FRAGMENT_SHADER}},
                            {"position", "texture_pos", "normal"});

    vp_location = glGetUniformLocation(shader_program, "vp");
    model_location = glGetUniformLocation(shader_program, "model");
    normMat_location = glGetUniformLocation(shader_program, "norm");
    lightColor_location = glGetUniformLocation(shader_program, "lightColor");
    lightPosition_location =
        glGetUniformLocation(shader_program, "lightPosition");
    cameraPosition_location =
        glGetUniformLocation(shader_program, "cameraPosition");

    glUseProgram(shader_program);

    cgmath::vec3 lightColor{1};
    cgmath::vec3 lightPosition{-2, 5, 5};
    c = std::unique_ptr<phong_cube>(
        new phong_cube("images/crate.png", {1}, view, projection, shader_program,
                 model_location, vp_location, normMat_location,
                 {lightColor_location, lightColor},
                 {lightPosition_location, lightPosition},
                 {cameraPosition_location, cameraPosition}));

    glUseProgram(0);
}

void scene_phong::mainLoop() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float time = time::elapsed_time().count();
    c->setModel(
        cgmath::mat4::modelMatrix({}, {3}, {30 * time, 60 * time, 30 * time}));
    c->render();
}

void scene_phong::resize(int width, int height) {
    scene_mvp::resize(width, height);
    c->setProyection(projection);
}

scene_phong::~scene_phong() {}
