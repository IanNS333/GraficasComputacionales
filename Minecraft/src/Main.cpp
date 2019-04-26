#include <iostream>

#include "scene_manager.h"

int main(int argc, char* argv[])
{
	scene_manager::start(argc, argv, "Hello, World!", 800, 800);

	return 0;
}
