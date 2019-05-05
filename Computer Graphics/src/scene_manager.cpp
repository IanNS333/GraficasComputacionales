#include "scene_manager.h"

#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <IL/il.h>

#include "scene.h"
#include "scene_compatibility.h"
#include "scene_primitives.h"
#include "scene_chaikin.h"
#include "scene_conchoid.h"
#include "time_utils.h"
#include "scene_vertex.h"
#include "scene_fragment.h"
#include "scene_circle_grid.h"
#include "scene_sphere.h"
#include "scene_circle.h"
#include "scene_cube.h"
#include "scene_texture.h"
#include "scene_shading.h"

std::vector<std::unique_ptr<scene>> scene_manager::sceneList;
int scene_manager::currentScene = -1;

void scene_manager::start(int argc, char* argv[], const std::string& name, int width, int height)
{
	// Time init
	time::init();

	// Freeglut init
	glutInit(&argc, argv);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutInitWindowSize(width, height);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutCreateWindow(name.c_str());
	glutDisplayFunc(mainLoop);
	glutIdleFunc(idle);
	glutReshapeFunc(resize);
	glutCloseFunc(cleanup);
	glutKeyboardFunc(normalKeysDown);
	glutKeyboardUpFunc(normalKeysUp);
	glutSpecialFunc(specialKeys);
	glutPassiveMotionFunc(passiveMotion);

	// Glew init
	glewInit();

	// OpenGL init
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	// GL version
	std::cout << glGetString(GL_VERSION) << std::endl;

	ilInit();
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

	// Scene init
	initialize();

	// Run main loop
	glutMainLoop();
}

void scene_manager::next()
{
	int nScenes = (int)sceneList.size();
	if (nScenes > 0)
	{
		sceneList.at(currentScene)->sleep();
		currentScene = (currentScene + 1) % nScenes;
		sceneList.at(currentScene)->awake();
	}
}

void scene_manager::prev()
{
	int nScenes = (int)sceneList.size();
	if (nScenes > 0)
	{
		sceneList.at(currentScene)->sleep();
		currentScene--;
		if (currentScene < 0)
			currentScene = (nScenes - 1);
		sceneList.at(currentScene)->awake();
	}
}

void scene_manager::initialize()
{
	// Ejemplo de como agregar escenas al proyecto
	//std::unique_ptr<scene> somescene(new scene_project);
	//sceneList.push_back(std::move(somescene));
	
	std::unique_ptr<scene> scene1(new scene_compatibility);
	std::unique_ptr<scene> scene2(new scene_primitives);
	std::unique_ptr<scene> scene3(new scene_chaikin);
	std::unique_ptr<scene> scene4(new scene_conchoid);
	std::unique_ptr<scene> scene5(new scene_vertex);
	std::unique_ptr<scene> scene6(new scene_fragment);
	std::unique_ptr<scene> scene7(new scene_circle_grid);
	std::unique_ptr<scene> scene8(new scene_sphere);
	std::unique_ptr<scene> scene9(new scene_circle);
	std::unique_ptr<scene> scene10(new scene_cube);
	std::unique_ptr<scene> scene11(new scene_texture);
	std::unique_ptr<scene> scene12(new scene_shading);

	sceneList.push_back(std::move(scene12));
	sceneList.push_back(std::move(scene11));
	sceneList.push_back(std::move(scene10));
	sceneList.push_back(std::move(scene9));
	sceneList.push_back(std::move(scene8));
	sceneList.push_back(std::move(scene7));
	sceneList.push_back(std::move(scene6));
	sceneList.push_back(std::move(scene5));
	sceneList.push_back(std::move(scene4));
	sceneList.push_back(std::move(scene3));
	sceneList.push_back(std::move(scene2));
	sceneList.push_back(std::move(scene1));

	for (auto& s : sceneList)
		s->init();

	if (sceneList.size() > 0)
	{
		currentScene = 0;
		sceneList.at(currentScene)->awake();
	}
}

void scene_manager::mainLoop()
{
	time::tick();
	std::cout << '\r';
	std::cout << (1.0f / time::delta_time().count());
	if (currentScene >= 0)
		sceneList.at(currentScene)->mainLoop();

	glutSwapBuffers();
}

void scene_manager::idle()
{
	glutPostRedisplay();
}

void scene_manager::cleanup()
{
	sceneList.clear();
	currentScene = -1;
}

void scene_manager::resize(int width, int height)
{
	if (currentScene >= 0)
		sceneList.at(currentScene)->resize(width, height);
}

void scene_manager::normalKeysDown(unsigned char key, int x, int y)
{
	if (key == '+' || key == '.')
		next();

	if (key == '-' || key == ',')
		prev();

	if (key == 'r')
		if (currentScene >= 0)
			sceneList.at(currentScene)->reset();

	if (currentScene >= 0)
		sceneList.at(currentScene)->normalKeysDown(key);
}

void scene_manager::normalKeysUp(unsigned char key, int x, int y)
{
	if (currentScene >= 0)
		sceneList.at(currentScene)->normalKeysUp(key);
}

void scene_manager::specialKeys(int key, int x, int y)
{
	if (currentScene >= 0)
		sceneList.at(currentScene)->specialKeys(key);
}

void scene_manager::passiveMotion(int x, int y)
{
	if (currentScene >= 0)
		sceneList.at(currentScene)->passiveMotion(x, y);
}