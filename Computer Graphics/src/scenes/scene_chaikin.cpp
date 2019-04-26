#include "scenes/scene_chaikin.h"

#include <list>
#include "_util.h"

void scene_chaikin::init() {
    std::vector<cgmath::vec2> p_points;
    for (auto& poligon : poligons) {
        int size = 0;
        p_starts.push_back(p_points.size());
        for (unsigned int i = 0; i + 1 < poligon.size(); ++i) {
            p_points.push_back(poligon[i]);
            size += makeDashedLine(p_points, poligon[i + 1]);
        }
        p_sizes.push_back(size);
    }

    glGenVertexArrays(1, &p_vao);
    addBuffer(p_vao, p_vbo, p_points);

    std::vector<cgmath::vec2> c_points;
    for (auto& poligon : poligons) {
        auto v = chaikin(poligon, chaikin_steps);
        c_starts.push_back(c_points.size());
        c_sizes.push_back(v.size());
        c_points.insert(c_points.end(), v.begin(), v.end());
    }

    glGenVertexArrays(1, &c_vao);
    addBuffer(c_vao, c_vbo, c_points);
}

void scene_chaikin::awake() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void scene_chaikin::reset() {
    chaikin_steps = 4;
    c_starts.clear();
    c_sizes.clear();
    p_starts.clear();
    p_sizes.clear();
}

void scene_chaikin::mainLoop() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (show_poligons) {
        glBindVertexArray(p_vao);
        glMultiDrawArrays(GL_LINES, p_starts.data(), p_sizes.data(),
                          p_starts.size());
    }

    if (show_curves) {
        glBindVertexArray(c_vao);
        glMultiDrawArrays(GL_LINE_STRIP, c_starts.data(), c_sizes.data(),
                          c_starts.size());
    }

    glBindVertexArray(0);
}

void scene_chaikin::keysDown(int key) {
    switch (key) {
        case GLFW_KEY_0:
        case GLFW_KEY_1:
        case GLFW_KEY_2:
        case GLFW_KEY_3:
        case GLFW_KEY_4:
        case GLFW_KEY_5:
        case GLFW_KEY_6:
            reset();
            chaikin_steps = key - GLFW_KEY_0;
            init();
            break;
        case GLFW_KEY_P:
            show_poligons = !show_poligons;
            break;
        case GLFW_KEY_C:
            show_curves = !show_curves;
            break;
        default:
            break;
    }
}

int scene_chaikin::makeDashedLine(std::vector<cgmath::vec2>& buffer,
                                  cgmath::vec2 b) {
    auto a = buffer.back();

    cgmath::vec2 v = b - a;
    v.normalize();
    v *= dashed_line_length;

    cgmath::vec2 p(a);
    int size = 2;
    while ((b - p).magnitude() > dashed_line_length) {
        p += v;
        buffer.emplace_back(p);
        ++size;
    }

    buffer.push_back(b);
    return size;
}

std::vector<cgmath::vec2> scene_chaikin::chaikin(
    std::vector<cgmath::vec2>& poligon,
    int steps) {
    std::list<cgmath::vec2> points(poligon.begin(), poligon.end());
    auto first = poligon.front(), last = poligon.back();
    if (first == last) {
        points.push_back(*++points.begin());
    }
    for (int i = 0; i < steps; ++i) {
        for (auto it = points.begin(), next = ++points.begin();
             next != points.end(); it = next++) {
            points.emplace(next, (3 * (*it) + (*next)) / 4);
            points.emplace(next, ((*it) + 3 * (*next)) / 4);
            points.erase(it);
        }
        points.pop_back();
    }
    if (!(first == last)) {
        points.push_front(first);
        points.push_back(last);
    }
    return std::vector<cgmath::vec2>(points.begin(), points.end());
}
