#include "scene_manager.h"

#include <iostream>

#include <IL/il.h>

#include "scene.h"
#include "scene_shading.h"
#include "time_utils.h"

std::vector<std::unique_ptr<scene>> scene_manager::sceneList;
int scene_manager::currentScene = -1;
std::atomic<bool> scene_manager::program_is_running = true;
std::vector<std::thread> scene_manager::threads;

void scene_manager::start(int argc,
                          char* argv[],
                          const std::string& name,
                          int width,
                          int height) {
    // GLFW init
    if (!glfwInit()) {
        std::cout << "Error initializing GLFW" << std::endl;
        return;
    }
    glfwSetErrorCallback(error);

/*
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
*/
    GLFWwindow* window =
        glfwCreateWindow(width, height, name.c_str(), NULL, NULL);

    if (!window) {
        std::cout << "Error creating window" << std::endl;
        return;
    }
    glfwMakeContextCurrent(window);

    //glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(window, resize);

    glfwSetKeyCallback(window, keys);
    glfwSetCursorPosCallback(window, passiveMotion);
	glfwSetMouseButtonCallback(window, mouseButton);

    // Glad init
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    // OpenGL init
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    // GL version
    std::cout << glGetString(GL_VERSION) << std::endl;

    // init devIL with settings matching openGl's
    ilInit();
    ilEnable(IL_ORIGIN_SET);
    ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

	program_is_running = true;

    // Scene init
    initialize(window);

    // Run main loop
    mainLoop(window);
}

void scene_manager::next() {
    int nScenes = (int)sceneList.size();
    if (nScenes > 0) {
        sceneList.at(currentScene)->sleep();
        currentScene = (currentScene + 1) % nScenes;
        sceneList.at(currentScene)->awake();
    }
}

void scene_manager::prev() {
    int nScenes = (int)sceneList.size();
    if (nScenes > 0) {
        sceneList.at(currentScene)->sleep();
        currentScene--;
        if (currentScene < 0)
            currentScene = (nScenes - 1);
        sceneList.at(currentScene)->awake();
    }
}

void update_map(std::atomic<bool>& program_is_running) {
	while (program_is_running)
	{
		(reinterpret_cast<scene_shading *>(scene_manager::sceneList[0].get()))->update_map(program_is_running);
	}
}

void scene_manager::initialize(GLFWwindow *window) {
    // Ejemplo de como agregar escenas al proyecto
    // std::unique_ptr<scene> somescene(new scene_project);
    // sceneList.push_back(std::move(somescene));

    std::unique_ptr<scene_shading> scene1(new scene_shading(window));
	
    sceneList.push_back(std::move(scene1));

	for (auto& s : sceneList) {
		s->init();
	}
	
	threads.push_back(thread(update_map, std::ref(program_is_running)));

    if (sceneList.size() > 0) {
        currentScene = 0;
        sceneList.at(currentScene)->awake();
    }
}

void scene_manager::mainLoop(GLFWwindow* window) {
    while (!glfwWindowShouldClose(window)) {
        time::tick();

        std::cout << '\r';
        std::cout << (1.0f / time::delta_time().count());
        if (currentScene >= 0)
            sceneList.at(currentScene)->mainLoop();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
	program_is_running = false;
	for (thread &t : threads) {
		t.join();
	}
    sceneList.clear();
    currentScene = -1;
    glfwDestroyWindow(window);
    glfwTerminate();
}

void scene_manager::resize(GLFWwindow* window, int width, int height) {
    if (currentScene >= 0)
        sceneList.at(currentScene)->resize(width, height);
}

void scene_manager::error(int error, const char* description) {
    std::cout << description << std::endl;
}

void scene_manager::keys(GLFWwindow* window,
                         int key,
                         int scancode,
                         int action,
                         int mods) {
    if (action == GLFW_PRESS) {
        keysDown(key);
    } else if (action == GLFW_RELEASE) {
        keysUp(key);
    }
}

void scene_manager::keysDown(int key) {
    if (key == GLFW_KEY_EQUAL)
        next();

    if (key == GLFW_KEY_MINUS)
        prev();

    if (key == GLFW_KEY_R)
        if (currentScene >= 0)
            sceneList.at(currentScene)->reset();

    if (currentScene >= 0)
        sceneList.at(currentScene)->keysDown(key);
}

void scene_manager::keysUp(int key) {
    if (currentScene >= 0)
        sceneList.at(currentScene)->keysUp(key);
}

void scene_manager::passiveMotion(GLFWwindow* window, double x, double y) {
    if (currentScene >= 0)
        sceneList.at(currentScene)->passiveMotion(x, y);
}

void scene_manager::mouseButton(GLFWwindow* window, int button, int action, int modifiers) {
	if (currentScene >= 0) {
		sceneList.at(currentScene)->mouseButton(button, action);
	}
}