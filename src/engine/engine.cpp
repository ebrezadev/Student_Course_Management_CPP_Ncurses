#include <iostream>
#include "engine.hpp"
#include <ncurses.h>

constexpr int BLOCKING_IO = -1;
constexpr int NON_BLOCKING_IO = 0;

Engine::Engine()
{
	// Initialize singleton io object
	io = IO::getInstance();
	input = 0;
	is_running = true;
	update_window = nullptr;
}

Engine::~Engine()
{
}

Engine &Engine::getInstance()
{
	static Engine Instance;
	return Instance;
}

void Engine::update(int to)
{
	// Pass the input event to the state machine and run the state machine
	StateMachine::state_machine_return_t state_machine_return = app_state_machine.run(input);

	is_running = state_machine_return.is_running;
	should_update = state_machine_return.update;
	update_window = state_machine_return.window;

	// Read the input event from IO subsystem, with specified timeout
	input = io->input(to, update_window);
}

void Engine::render()
{
	if(update_window != nullptr)
		io->render(update_window);
}

void Engine::execute()
{
	for (; is_running;)
	{
		render();
		
		// Read input from IO subsystem and pass it to state machine
		update(NON_BLOCKING_IO);
	}
}
