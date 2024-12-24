#include <iostream>
#include <cstring>
#include "engine.hpp"
#include "error.hpp"

int main()
{
	Engine &engine = Engine::getInstance();

	engine.execute();

	return 0;
}

