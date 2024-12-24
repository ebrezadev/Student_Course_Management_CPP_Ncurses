#include "state_machine.hpp"
#include "states.hpp"

StateMachine::StateMachine()
{
	// Start from the top of the list of states
	current_state = State_list_t::State_Initial_Check;
}

StateMachine::~StateMachine()
{

}

StateMachine::state_machine_return_t StateMachine::run(int input)
{
	State::state_return_t state_return = states_table[(int)current_state]->stateFunction(input);

	current_state = state_return.next_state;

	if(current_state == State_list_t::State_Exit)
		return {false, state_return.update, state_return.state_window};
	else
		return {true, state_return.update, state_return.state_window};
}
