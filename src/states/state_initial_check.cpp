#include "states.hpp"
#include <ncurses.h>
#include <cstring>
#include "limits_constants.hpp"
#include "portal.hpp"

StateInitialCheck::StateInitialCheck()
{
}

StateInitialCheck::~StateInitialCheck()
{
}

StateInitialCheck::state_return_t StateInitialCheck::stateFunction(int input)
{
	State_list_t next_state = State_list_t::State_Load_From_File;

	getmaxyx(stdscr, max_rows, max_columns);

	if(max_columns < 75 || max_rows < 15)
	{
		mvwprintw(stdscr, 0, 0, "PLEASE EXPAND YOUR TERMINAL WINDOW");
		next_state = State_list_t::State_Initial_Check;
	}

	if(!has_colors())
	{
		mvwprintw(stdscr, 0, 0, "YOUR TERMINAL DOES NOT SUPPORT COLORS");
		next_state = State_list_t::State_Initial_Check;
	}

	return {next_state, stdscr, true};
}

