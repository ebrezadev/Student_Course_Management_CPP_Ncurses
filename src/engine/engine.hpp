/* 
This file contains 'engine' class. App begins with instantiating and running an engine object.
engine class is a singleton class to stop duplicate engine objects.

Platform/App/IO independent.
*/
#ifndef __ENGINE_H__
#define __ENGINE_H__

#include "io.hpp"
#include "state_machine.hpp"

// Engine is responsible for high level app loop
class Engine
{
public:
	static Engine &getInstance();
	Engine(Engine &other) = delete;				
	void operator=(const Engine &) = delete;
	void execute();									// Runs the app loop

private:
	Engine();
	~Engine();
	void update(int to);						// Passes the input to state machine
	void render();								// Updates the screen 
	bool is_running;							// Finished app condition
	bool should_update;							// Determines if engine should render
	int input;
	WINDOW *update_window;
	IO *io;										// pointer to IO object
	StateMachine app_state_machine;				// State Machine object
};

#endif
