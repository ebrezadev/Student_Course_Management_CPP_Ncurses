#include "states.hpp"
#include <ncurses.h>
#include <cstring>
#include "limits_constants.hpp"

StateDefaultWorkspace::StateDefaultWorkspace()
{
	destination_state = State_list_t::State_Main_Menu;
	load_success = false;
	next_state = State_list_t::State_Default_Workspace;
	main_window_is_printed = false;
}

StateDefaultWorkspace::~StateDefaultWorkspace()
{
}

void StateDefaultWorkspace::formatHandler()
{
	getmaxyx(stdscr, max_rows, max_columns);

	if (!title_window_is_rendered)
	{
		titleWindowRender((char *)"WORKSPACE", TITLE_BAR_WIDTH, max_columns - MENU_COLS, 0, MENU_COLS, WORKSPACE_TITLE_BAR_COLOR_ON);
	}
	else if (!footer_window_is_rendered)
	{
		footerWindowRender((char *)" ", FOOTER_BAR_WIDTH, max_columns, max_rows - FOOTER_BAR_WIDTH, 0, WORKSPACE_FOOTER_BAR_COLOR);
	}
	else if (!main_window_is_rendered)
	{
		mainWindowRender(max_rows - FOOTER_BAR_WIDTH - TITLE_BAR_WIDTH, max_columns - MENU_COLS, TITLE_BAR_WIDTH, MENU_COLS, WORKSPACE_COLOR);
	}
}

void StateDefaultWorkspace::mainWindowHandler()
{
	next_state = State_list_t::State_Default_Workspace;

	if (title_window_is_rendered && main_window_is_rendered && footer_window_is_rendered && main_window_is_printed)
	{
		title_window_is_rendered = false;
		footer_window_is_rendered = false;
		main_window_is_rendered = false;
		main_window_is_printed = false;

		update_window = title_bar_window;

		wbkgd(title_bar_window, COLOR_PAIR(WORKSPACE_TITLE_BAR_COLOR_OFF));

		next_state = destination_state;
	}

	if (main_window_is_rendered)
	{
		getmaxyx(window, max_rows, max_columns);
		wattron(window, A_BOLD);
		mvwprintw(window, 1, (max_columns - sizeof("Students Portal")) / 2, "%s", (char *)"Students Portal");
		wattroff(window, A_BOLD);

		wattron(window, A_ITALIC | A_DIM);
		mvwprintw(window, 3, (max_columns - sizeof("Use arrow keys to navigate the menus.")) / 2, "%s", (char *)"Use arrow keys to navigate the menus.");
		mvwprintw(window, 4, (max_columns - sizeof("Press ENTER to choose.")) / 2, "%s", (char *)"Press ENTER to choose.");
		wattroff(window, A_ITALIC | A_DIM);

		main_window_is_printed = true;
	}
}

void StateDefaultWorkspace::inputHandler(int input)
{
}
