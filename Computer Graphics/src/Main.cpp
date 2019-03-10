#include <iostream>

#include "scene_manager.h"

int main(int argc, char* argv[])
{
	scene_manager::start(argc, argv, "Hello, World!", 400, 400);

	return 0;
}
