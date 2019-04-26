#include "scene_shading.h"

#include <GL/freeglut.h>
#include <iostream>

#include "cgmath.h"
#include "mat3.h"
#include "mat4.h"
#include "perlin_noise.h"
#include "time_utils.h"
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"

using cgmath::lookAt;
using cgmath::mat3;
using cgmath::mat4;
using cgmath::radians;
using cgmath::rotate_around;
using cgmath::rotation_matrix;
using cgmath::translation_matrix;
using cgmath::vec2;
using cgmath::vec3;
using cgmath::vec4;

scene_shading::~scene_shading() {
    glDeleteProgram(shader_program);
    glDeleteTextures(1, &textureId);
}

void scene_shading::compile_shaders() {
    shader_program = generateShaderProgram(
        {
            {"shaders/cube_shading.vert", GL_VERTEX_SHADER},
            {"shaders/lambertian.frag", GL_FRAGMENT_SHADER},
            //{"shaders/cube_shading.frag", GL_FRAGMENT_SHADER },
        },
        {"position_in", "texture_coord_in", "normal_in", "offset_in"});

    glUseProgram(shader_program);

    time_location = glGetUniformLocation(shader_program, "u_time");
    model_matrix_location =
        glGetUniformLocation(shader_program, "u_model_matrix");
    proyection_matrix_location =
        glGetUniformLocation(shader_program, "u_proyection_matrix");
    view_matrix_location =
        glGetUniformLocation(shader_program, "u_view_matrix");
    normal_matrix_location =
        glGetUniformLocation(shader_program, "u_normal_matrix");

    texture_location = glGetUniformLocation(shader_program, "u_texture1");

    light_position_location =
        glGetUniformLocation(shader_program, "u_light_position");
    light_color_location =
        glGetUniformLocation(shader_program, "u_light_color");
    camera_position_location =
        glGetUniformLocation(shader_program, "u_camera_position");

    glUniform3f(light_color_location, 1.0f, 1.0f, 1.0f);
    glUniform3f(light_position_location, 0, 10.0f, 0.0f);

    glUseProgram(0);
}

// mat4 scene_shading::get_camera_model_matrix(float time) {
//
//	//mat4 camera_model = lookAt(camera_position, { 0.0f, 0.0f, 0.0f }, {
//0.0f, 1.0f, 0.0f });
//
//	mat4 translation = mat4::transpose({
//		{ 1.0f, 0.0f, 0.0f, camera_position.x },
//		{ 0.0f, 1.0f, 0.0f, camera_position.y },
//		{ 0.0f, 0.0f, 1.0f, camera_position.z },
//		{ 0.0f, 0.0f, 0.0f, 1.0f }
//		});
//
//	mat4 camera_model = /*rotation_matrix(0.0f, 10*radians(time), 0.0f) * */
//translation * rotation_matrix(radians(-45.0f), 0.0f, 0.0f);
//
//	return camera_model;
//}

mat4 scene_shading::calculate_model_matrix(float time) {
    float deltaX = 30.0f;
    float deltaY = 60.0f;
    float deltaZ = 30.0f;

    float alpha = radians(time * deltaX);
    float beta = radians(time * deltaY);
    float gamma = radians(time * deltaZ);

    mat4 scale = {
        {0.5f, 0.0f, 0.0f, 0.0f},
        {0.0f, 0.5f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.5f, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f},
    };

    return rotation_matrix(0, 0, 0) * scale;
    // return rotation_matrix(alpha, beta, gamma) * scale;
}

vector<vec2> scene_shading::get_texture_coords(int x, int y) {
    x--;
    y--;
    vec2 lower_left = {x / 16.0f, (y / 23.0f)};
    vec2 lower_right = {(x + 1.0f) / 16.0f, (y / 23.0f)};
    vec2 upper_left = {x / 16.0f, (y + 1.0f) / 23.0f};
    vec2 upper_right = {(x + 1.0f) / 16.0f, (y + 1.0f) / 23.0f};

    vector<vec2> texture_coords = {
        lower_left,
        lower_right,
        upper_right,
        upper_left,
    };
    return texture_coords;
}

vector<vec3> scene_shading::cube_positions() {
    vec3 back_lower_left = {-1.0f, -1.0f, -1.0f};
    vec3 back_lower_right = {1.0f, -1.0f, -1.0f};
    vec3 back_upper_right = {1.0f, 1.0f, -1.0f};
    vec3 back_upper_left = {-1.0f, 1.0f, -1.0f};
    vec3 front_lower_left = {-1.0f, -1.0f, 1.0f};
    vec3 front_lower_right = {1.0f, -1.0f, 1.0f};
    vec3 front_upper_right = {1.0f, 1.0f, 1.0f};
    vec3 front_upper_left = {-1.0f, 1.0f, 1.0f};

    vector<vec3> positions = {
        // front
        front_lower_left,
        front_lower_right,
        front_upper_right,
        front_upper_left,
        // left
        back_lower_left,
        front_lower_left,
        front_upper_left,
        back_upper_left,
        // right
        front_lower_right,
        back_lower_right,
        back_upper_right,
        front_upper_right,
        // up
        front_upper_left,
        front_upper_right,
        back_upper_right,
        back_upper_left,
        // down
        back_lower_left,
        back_lower_right,
        front_lower_right,
        front_lower_left,
        // back
        back_lower_right,
        back_lower_left,
        back_upper_left,
        back_upper_right,
    };
    return positions;
}

vector<vec2> scene_shading::cube_texture_coords(int id) {
    vector<vec2> texture_coords_up =
        get_texture_coords(textures[id].x_up, textures[id].y_up);
    vector<vec2> texture_coords_side =
        get_texture_coords(textures[id].x_side, textures[id].y_side);
    vector<vec2> texture_coords_down =
        get_texture_coords(textures[id].x_down, textures[id].y_down);
    vector<vec2> texture_coords = {
        // front
        texture_coords_side[0],
        texture_coords_side[1],
        texture_coords_side[2],
        texture_coords_side[3],
        // left
        texture_coords_side[0],
        texture_coords_side[1],
        texture_coords_side[2],
        texture_coords_side[3],
        // right
        texture_coords_side[0],
        texture_coords_side[1],
        texture_coords_side[2],
        texture_coords_side[3],
        // up
        texture_coords_up[0],
        texture_coords_up[1],
        texture_coords_up[2],
        texture_coords_up[3],
        // down
        texture_coords_down[0],
        texture_coords_down[1],
        texture_coords_down[2],
        texture_coords_down[3],
        // back
        texture_coords_side[0],
        texture_coords_side[1],
        texture_coords_side[2],
        texture_coords_side[3],
    };
    return texture_coords;
}

vector<vec3> scene_shading::cube_normal_vectors() {
    vec3 front = {0.0f, 0.0f, 1.0f};
    vec3 back = {0.0f, 0.0f, -1.0f};
    vec3 up = {0.0f, 1.0f, 0.0f};
    vec3 down = {0.0f, -1.0f, 0.0f};
    vec3 left = {-1.0f, 0.0f, 0.0f};
    vec3 right = {1.0f, 0.0f, 0.0f};

    vector<vec3> normal_vectors = {front, front, front, front, left,  left,
                                   left,  left,  right, right, right, right,
                                   up,    up,    up,    up,    down,  down,
                                   down,  down,  back,  back,  back,  back};

    return normal_vectors;
}

bool scene_shading::is_block_visible(int i, int j, int k) {
    int a_start = i == 0 ? 0 : -1;
    int a_end = i == 1000 ? 0 : 1;

    int b_start = j == 0 ? 0 : -1;
    int b_end = j == 1000 ? 0 : 1;

    int c_start = k == 0 ? 0 : -1;
    int c_end = k == 64 ? 0 : 1;

    for (int a = a_start; a <= a_end; a++) {
        for (int b = b_start; b <= b_end; b++) {
            for (int c = c_start; c <= c_end; c++) {
                if (blocks[i + a][j + b][k + c] == 0) {
                    return true;
                }
            }
        }
    }
    return false;
}

vector<vector<vec3>> scene_shading::generate_map() {
    for (int i = 0; i <= 1000; i++) {
        for (int j = 0; j <= 1000; j++) {
            for (int k = 0; k <= 64; k++) {
                blocks[i][j][k] = 0;
            }
        }
    }
    perlin::generate_grid(101, 101);

    for (int i = 0; i <= 1000; i++) {
        for (int j = 0; j <= 1000; j++) {
            int height = 16.0f * perlin::height_at(i / 10.0f, j / 10.0f);

            for (int k = height; k >= -16; k--) {
                if (blocks[i][j][k + 16 + 1] == 0) {
                    blocks[i][j][k + 16] = 1;
                } else if (blocks[i][j][k + 16 + 3] == 0) {
                    blocks[i][j][k + 16] = 2;
                } else {
                    blocks[i][j][k + 16] = 3;
                }
            }
        }
    }

    offsets.clear();
    for (int i = 0; i < texture_count; i++) {
        offsets.push_back(vector<vec3>());
    }

    for (int i = 0; i <= 1000; i++) {
        for (int j = 0; j <= 1000; j++) {
            for (int k = 0; k <= 64; k++) {
                if (blocks[i][j][k] != 0 && is_block_visible(i, j, k)) {
                    int type = blocks[i][j][k] - 1;

                    offsets[type].push_back(vec3(i - 500, k - 16, j - 500));
                    instance_count_per_block[type]++;
                }
                if (k > 30) {
                    bool a = true;
                }
            }
        }
    }

    return offsets;
}

void scene_shading::init() {
    vector<vec3> positions = cube_positions();
    vector<vec2> texture_coords = cube_texture_coords(0);
    vector<vec3> normal_vectors = cube_normal_vectors();
    offsets = generate_map();

    vector<unsigned int> indices{
        // front
        0,
        1,
        2,
        0,
        2,
        3,
        // left
        4,
        5,
        6,
        4,
        6,
        7,
        // right
        8,
        9,
        10,
        8,
        10,
        11,
        // up
        12,
        13,
        14,
        12,
        14,
        15,
        // down
        16,
        17,
        18,
        16,
        18,
        19,
        // back
        20,
        21,
        22,
        20,
        22,
        23,
    };

    glGenVertexArrays(1, &vao);

    indexIBO = generateIBO(vao, indices, GL_STATIC_DRAW);
    positionsVBO = generateVBO(vao, 0, positions, 3, GL_FLOAT, GL_STATIC_DRAW);
    texture_coordsVBO =
        generateVBO(vao, 1, texture_coords, 2, GL_FLOAT, GL_STATIC_DRAW);
    normal_vectorsVBO =
        generateVBO(vao, 2, normal_vectors, 3, GL_FLOAT, GL_STATIC_DRAW);
    offsets_coordsVBO = generateInstancedVBO(vao, 3, offsets[0], 3, GL_FLOAT,
                                             GL_STATIC_DRAW, 1);

    compile_shaders();

    textureId = generateTexture("spritesheet.png");
}

void scene_shading::awake() {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	mouse_x = x;
	mouse_y = y;
	glClearColor(208.0f / 255.0f, 183.0f / 255.0f, 249.0f / 255.0f, 1.0f);
}

void scene_shading::sleep() {
    glClearColor(1.0f, 1.0f, 0.5f, 1.0f);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void scene_shading::mainLoop() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    handle_movement();
    handle_rotation();
    handle_gravity();

    glBindVertexArray(vao);
    glUseProgram(shader_program);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);

    float u_time = time::elapsed_time().count();
    glUniform1f(time_location, u_time);
    glUniform1i(texture_location, 0);

    mat4 model_matrix = calculate_model_matrix(u_time);
    glUniformMatrix4fv(model_matrix_location, 1, GL_FALSE,
                       &(model_matrix[0][0]));

    mat3 normal_matrix = mat3::transpose(
        mat3::inverse(mat3(model_matrix[0], model_matrix[1], model_matrix[2])));
    glUniformMatrix3fv(normal_matrix_location, 1, GL_FALSE,
                       &(normal_matrix[0][0]));

    glUniformMatrix4fv(proyection_matrix_location, 1, GL_FALSE,
                       &(proyection_matrix[0][0]));

    // mat4 camera_model_matrix = get_camera_model_matrix(u_time);
    // mat4 view_matrix = mat4::inverse(camera_model_matrix);
    mat4 view_matrix =
        lookAt(camera_position, camera_position + forward, {0.0f, 1.0f, 0.0f});
    glUniformMatrix4fv(view_matrix_location, 1, GL_FALSE, &(view_matrix[0][0]));

    for (int i = 0; i < texture_count; i++) {
        updateVBO(vao, texture_coordsVBO, 1, cube_texture_coords(i), 2,
                  GL_FLOAT, GL_STATIC_DRAW);
        updateInstancedVBO(vao, offsets_coordsVBO, 3, offsets[i], 3, GL_FLOAT,
                           GL_STATIC_DRAW, 1);
        glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr,
                                offsets[i].size());
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
    glBindVertexArray(0);
}

void scene_shading::resize(int width, int height) {
    this->width = width;
    this->height = height;

    glViewport(0, 0, width, height);

    glUseProgram(shader_program);

    proyection_matrix = mat4::transpose({
        {1.0f /
             (((width + 0.0f) / (height + 0.0f)) * tan(radians(60.0f) / 2.0f)),
         0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f / (tan(radians(60.0f) / 2.0f)), 0.0f, 0.0f},
        {0.0f, 0.0f, -(1000.0f + 1.0f) / (1000.0f - 1.0f),
         -(2.0f * 1000.0f * 1.0f) / (1000.0f - 1.0f)},
        {0.0f, 0.0f, -1.0f, 0.0f},
    });

    glUseProgram(0);
    this->width = width;
    this->height = height;
}

void scene_shading::handle_gravity() {
    camera_position.y += 10.0f * up_input * time::delta_time().count();
}

void scene_shading::handle_rotation() {
    float delta_time = time::delta_time().count();

	yaw_input = delta_x;
	pitch_input = delta_y;

	delta_x = 0;
	delta_y = 0;

    if (abs(yaw_input) < 0.05f && abs(pitch_input) < 0.05f) {
        return;
    }

    float rotation_speed = radians(delta_time);
    vec2 rotation =
        rotation_speed * vec2(pitch_input, yaw_input);

    // mat4 rotate_camera = rotation_matrix(rotation.x, rotation.y, 0.0f);
    mat4 rotate_camera = rotate_around(rotation.y, {0.0f, 1.0f, 0.0f}) *
                         rotate_around(rotation.x, right);

	vec3 new_forward = rotate_camera * vec4(forward.x, forward.y, forward.z, 1.0f);
	vec3 front = vec3::cross({0.0f, 1.0f, 0.0f}, right);
	if (acosf(abs(vec3::dot(new_forward, front))) > radians(85.0f)) {
		float likelihood = vec3::dot({0.0f, 1.0f, 0.0f}, new_forward);
		float degrees = likelihood < 0.0f ? -85.0f: 85.0f;
		forward = /*rotate_around(rotation.x, {0.0f, 1.0f, 0.0f}) * */ rotate_around(radians(degrees), right) * vec4(front.x, front.y, front.x, 1.0f);
	}
	else {
		forward = rotate_camera * vec4(forward.x, forward.y, forward.z, 1.0f);
	}
    // mat4 pitch_rotation = rotation_matrix(0.0f, 0.0f);
    // right = rotate_camera * vec4(1.0f, 0.0f, 0.0f, 1.0f);
    // forward = rotate_camera  * vec4( 0.0, 0.0, -1.0f, 1.0f );
    // right = rotate_camera * vec4(right.x, right.y, right.z, 1.0f);
    forward = vec3::normalize(forward);
    right = vec3::cross(forward, {0.0f, 1.0f, 0.0f});
    upward = vec3::cross(right, forward);
}

void scene_shading::handle_movement() {
    float delta_time = time::delta_time().count();

    if (horizontal_input == 0 && vertical_input == 0 ) {
        return;
    }

    float speed = 10.0 * delta_time;
    vec2 movement =
        speed * vec2::normalize(vec2(horizontal_input, vertical_input));

    camera_position.x += (right * movement.x).x + (forward * movement.y).x;
    camera_position.z += (right * movement.x).z + (forward * movement.y).z;
}

void scene_shading::keysDown(int key) {
    if (key == GLFW_KEY_C) {
        compile_shaders();
    }
    if (key == GLFW_KEY_W) {
        vertical_input = 1;
    }
    if (key == GLFW_KEY_A) {
        horizontal_input = -1;
    }
    if (key == GLFW_KEY_S) {
        vertical_input = -1;
    }
    if (key == GLFW_KEY_D) {
        horizontal_input = 1;
    }
    if (key == GLFW_KEY_Z) {
        up_input = 1;
    }
    if (key == GLFW_KEY_X) {
        up_input = -1;
    }
    /*if (key == GLFW_KEY_UP) {
        pitch_input = -1;
    }
    if (key == GLFW_KEY_DOWN) {
        pitch_input = 1;
    }
    if (key == GLFW_KEY_RIGHT) {
        yaw_input = 1;
    }
    if (key == GLFW_KEY_LEFT) {
        yaw_input = -1;
    }*/
}

void scene_shading::keysUp(int key) {
    if (key == GLFW_KEY_W && vertical_input == 1) {
        vertical_input = 0;
    }
    if (key == GLFW_KEY_A && horizontal_input == -1) {
        horizontal_input = 0;
    }
    if (key == GLFW_KEY_S && vertical_input == -1) {
        vertical_input = 0;
    }
    if (key == GLFW_KEY_D && horizontal_input == 1) {
        horizontal_input = 0;
    }
    if (key == GLFW_KEY_Z && up_input == 1) {
        up_input = 0;
    }
    if (key == GLFW_KEY_X && up_input == -1) {
        up_input = 0;
    }
    /*if (key == GLFW_KEY_UP && pitch_input == -1) {
        pitch_input = 0;
    }
    if (key == GLFW_KEY_DOWN && pitch_input == 1) {
        pitch_input = 0;
    }
    if (key == GLFW_KEY_RIGHT && yaw_input == 1) {
        yaw_input = 0;
    }
    if (key == GLFW_KEY_LEFT && yaw_input == -1) {
        yaw_input = 0;
    }*/
    if (key == GLFW_KEY_LEFT_CONTROL) {
        capture_mouse = !capture_mouse;
        std::cout << capture_mouse << std::endl;
        if (capture_mouse) {
			double x;
			double y;
			glfwGetCursorPos(window, &x, &y);
			mouse_x = x;
			mouse_y = y;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
}

void scene_shading::passiveMotion(float x, float y) {

	std::cout << x << " " << y << std::endl;

	if (!capture_mouse) {
		return;
	}
	delta_x = (mouse_x - x);
	delta_y = (mouse_y - y);
	mouse_x = x;
	mouse_y = y;
}