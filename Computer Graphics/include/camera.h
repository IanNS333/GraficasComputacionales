#pragma once

#include <cmath>
#include <limits>
#include "mat4.h"
#include "vec3.h"

struct Camera {
    static constexpr float maxPitch = 89.0f, minPitch = -89.0f;
    cgmath::vec3 position{0}, front{0, 0, 1}, right{1, 0, 0}, up{0, 1, 0};
    float speed = 1.0f;
    float dx = 0, dz = 0;
    float pitch = 0, yaw = 0;
    float sensitivity = 0.05f, lastX = 0, lastY = 0;
    bool dirty = 0;

    void init() {
        double x, y;
        glfwGetCursorPos(screen::window, &x, &y);
        lastX = x;
        lastY = y;
    }

    cgmath::mat4 lookAt() {
        cgmath::mat4 pos{1};
        pos[3].x = -position.x;
        pos[3].y = -position.y;
        pos[3].z = -position.z;

        cgmath::mat4 turn{1};
        turn[0].x = right.x;
        turn[1].x = right.y;
        turn[2].x = right.z;

        turn[0].y = up.x;
        turn[1].y = up.y;
        turn[2].y = up.z;

        turn[0].z = front.x;
        turn[1].z = front.y;
        turn[2].z = front.z;

        dirty = false;
        return turn * pos;
    }

    bool rotate(float x, float y) {
        float xOff = x - lastX;
        float yOff = y - lastY;

        lastX = x;
        lastY = y;

        yaw -= xOff * sensitivity;
        pitch += yOff * sensitivity;

        if (pitch > maxPitch) {
            pitch = maxPitch;
        } else if (pitch < minPitch) {
            pitch = minPitch;
        }

        front.x = cos(rad(pitch)) * sin(rad(yaw));
        front.y = sin(rad(pitch));
        front.z = cos(rad(pitch)) * cos(rad(yaw));

        front = cgmath::vec3::normalize(front);
        right = cgmath::vec3::normalize(cgmath::vec3::cross({0, 1, 0}, front));
        up = cgmath::vec3::cross(front, right);

        return dirty = true;
    }

    bool move() {
        if (!dx && !dz) {
            return dirty;
        }

        position += dz * front;
        position += dx * right;

        return dirty = true;
    }
};
