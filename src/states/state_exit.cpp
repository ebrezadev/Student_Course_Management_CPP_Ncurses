#include "states.hpp"

StateExit::StateExit()
{
	window = nullptr;
}

StateExit::~StateExit()
{

}

StateExit::state_return_t StateExit::stateFunction(int input)
{
	return {State_list_t::State_Exit, window, true};
}
