#pragma once

#include <memory>
#include <vector>
#include <string>
#include <thread>

#include "scene.h"

class scene_manager
{
  public:
    static void start(int argc,
                      char* argv[],
                      const std::string& name,
                      int width,
                      int height);

    static void next();
    static void prev();

    static std::vector<std::unique_ptr<scene>> sceneList;

  private:
    static void initialize(GLFWwindow* window);
    static void mainLoop(GLFWwindow* window);
    static void resize(GLFWwindow* window, int width, int height);
    static void error(int error, const char* description);

    static void keys(GLFWwindow* window,
                     int key,
                     int scancode,
                     int action,
                     int mods);
    static void keysDown(int key);
    static void keysUp(int key);
    static void passiveMotion(GLFWwindow* window, double x, double y);
    static void mouseButton(GLFWwindow* window, int button, int action, int modifiers);

    static int currentScene;
	static std::atomic<bool> program_is_running;
	static std::vector<std::thread> threads;
};