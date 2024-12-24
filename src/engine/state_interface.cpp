#include "state_interface.hpp"
#include <cstring>

State::State()
{
	screen_update_flag = true;
	title_window_is_rendered = false;
	main_window_is_rendered = false;
	footer_window_is_rendered = false;
	window = nullptr;
	title_bar_window = nullptr;
	footer_bar_window = nullptr;
}

State::state_return_t State::stateFunction(int input)
{
	formatHandler();

	mainWindowHandler();

	inputHandler(input);

	return {next_state, update_window, screen_update_flag};
}

void State::titleWindowRender(char *str, int rows, int cols, int start_y, int start_x, int title_bar_color)
{
	if (title_bar_window == nullptr)
	{
		title_bar_window = newwin(rows, cols, start_y, start_x);
	}
	wbkgd(title_bar_window, COLOR_PAIR(title_bar_color));
	werase(title_bar_window);
	wattron(title_bar_window, A_BOLD);
	getmaxyx(title_bar_window, max_rows, max_columns);
	wborder(title_bar_window, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
	mvwprintw(title_bar_window, 1, (max_columns - strlen(str)) / 2, "%s", str);
	wattroff(title_bar_window, A_BOLD);
	update_window = title_bar_window;
	title_window_is_rendered = true;
}

void State::footerWindowRender(char *str, int rows, int cols, int start_y, int start_x, int footer_bar_color)
{
	if (footer_bar_window == nullptr)
	{
		getmaxyx(stdscr, max_rows, max_columns);
		footer_bar_window = newwin(rows, cols, start_y, start_x);
		wbkgd(footer_bar_window, COLOR_PAIR(footer_bar_color));
	}
	werase(footer_bar_window);
	getmaxyx(footer_bar_window, max_rows, max_columns);
	mvwprintw(footer_bar_window, 0, 1, "%s", str);
	update_window = footer_bar_window;
	footer_window_is_rendered = true;
}

void State::mainWindowRender(int rows, int cols, int start_y, int start_x, int main_window_color)
{
	if (window == nullptr)
	{
		window = newwin(rows, cols, start_y, start_x);
		keypad(window, TRUE);
		wbkgd(window, COLOR_PAIR(main_window_color));
	}
	update_window = window;
	main_window_is_rendered = true;
	werase(window);
}

Error_code_t State::digitsToInt(std::string number_string, int &number)
{
	for (size_t index = 0; index < number_string.length(); index++)
	{
		if (number_string.c_str()[index] < '0' || number_string.c_str()[index] > '9')
		{
			number = 0;

			return Error_code_t::ERROR_CONVERSION;
		}

		if (index == (number_string.length() - 1))
		{
			number = stoi(number_string);
			break;
		}
	}

	return Error_code_t::ERROR_OK;
}