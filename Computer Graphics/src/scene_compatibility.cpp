#include "scene_compatibility.h"
#include "time_utils.h"

void scene_compatibility::awake()
{
	glClearColor(1.0f, 1.0f, 0.5f, 1.0f);
}

void scene_compatibility::mainLoop()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	angle += 45.0f * time::delta_time().count();

	glRotatef(angle, 0.0f, 0.0f, 1.0f);
	glBegin(GL_TRIANGLES);
	
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(-1.0f, -1.0f);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2f(1.0f, -1.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(0.0f, 1.0f);
	
	glEnd();

	glPopMatrix();
}