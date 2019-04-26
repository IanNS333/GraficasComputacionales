#include "scenes/scene_sphere.h"

#include "mat4.h"
#include "_time.h"
#include "_util.h"

void scene_sphere::init() {
    shader_program =
        createShaderProgram({{"shaders/sphere.vert", GL_VERTEX_SHADER},
                             {"shaders/solid_color.frag", GL_FRAGMENT_SHADER}});
}

void scene_sphere::awake() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_PROGRAM_POINT_SIZE);
}

void scene_sphere::sleep() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glDisable(GL_PROGRAM_POINT_SIZE);
}

void scene_sphere::reset() {
    angle = 0;
}

void scene_sphere::mainLoop() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shader_program);
    glBindVertexArray(1);

    GLuint rotation_location = glGetUniformLocation(shader_program, "rotation");
    glUniformMatrix4fv(
        rotation_location, 1, GL_FALSE,
        &(cgmath::mat4::rotateZ(angle) * cgmath::mat4::rotateX(angle))[0][0]);

    glDrawArrays(GL_POINTS, 0, 35 * 34);

	glBindVertexArray(0);
    glUseProgram(0);
    angle += time::delta_time().count() * 60;
}

scene_sphere::~scene_sphere() {
    glDeleteProgram(shader_program);
}
