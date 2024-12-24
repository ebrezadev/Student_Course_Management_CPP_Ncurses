#ifndef __STATE_INTERFACE_H__
#define __STATE_INTERFACE_H__

#include <ncurses.h>
#include "states_list.hpp"
#include "limits_constants.hpp"
#include <string>
#include "error.hpp"

// Virtual State class is the parent of all the derived states classes of a specific app
class State
{
public:
	State();
	~State() {};

	typedef struct // The return type of every state function. contains the next state in case of state transition
	{
		State_list_t next_state;
		WINDOW *state_window;
		bool update;
	} state_return_t;

	virtual state_return_t stateFunction(int input); // The main functionality of every derived state
	void titleWindowRender(char *str, int rows, int cols, int start_y, int start_x, int title_bar_color);
	void footerWindowRender(char *str, int rows, int cols, int start_y, int start_x, int footer_bar_color);
	void mainWindowRender(int rows, int cols, int start_y, int start_x, int main_window_color);
	Error_code_t digitsToInt(std::string number_string, int &number);
	virtual void inputHandler(int input){};
	virtual void mainWindowHandler(){};
	virtual void formatHandler(){};

protected:
	State_list_t next_state;
	bool screen_update_flag; // In order to check if the screen has changed
	bool title_window_is_rendered;
	bool footer_window_is_rendered;
	bool main_window_is_rendered;
	int max_rows;	 // Maximum possible number of rows in a terminal
	int max_columns; // Maximum possible number of columns in a terminal
	WINDOW *window;
	WINDOW *title_bar_window;
	WINDOW *footer_bar_window;
	WINDOW *update_window;
};

#endif
