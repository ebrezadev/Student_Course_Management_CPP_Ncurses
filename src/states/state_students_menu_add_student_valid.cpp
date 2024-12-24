#include "states.hpp"
#include <ncurses.h>
#include <cstring>
#include "portal.hpp"
#include "limits_constants.hpp"

inline constexpr int NUMBER_OF_MENU_OPTIONS = 1;

StateNotificationStudentAddSuccess::StateNotificationStudentAddSuccess()
{
	current_menu = menu_t::OKAY;
	next_state = State_list_t::State_Notification_Student_Add_Success;
}

StateNotificationStudentAddSuccess::~StateNotificationStudentAddSuccess()
{
}

void StateNotificationStudentAddSuccess::formatHandler()
{
	getmaxyx(stdscr, max_rows, max_columns);

	if (!title_window_is_rendered)
	{
		titleWindowRender((char *)"SUCCESS", TITLE_BAR_WIDTH, NOTIFICATION_COLS, (max_rows - NOTIFICATION_ROWS) / 2, MENU_COLS + (max_columns - MENU_COLS - NOTIFICATION_COLS) / 2, NOTIFICATION_TITLE_BAR_COLOR_ON);
	}
	else if (!footer_window_is_rendered)
	{
		footerWindowRender((char *)"OKAY: BACK TO STUDENTS MENU", 1, max_columns, max_rows - 1, 0, NOTIFICATION_FOOTER_BAR_COLOR);
	}
	else if (!main_window_is_rendered)
	{
		mainWindowRender(NOTIFICATION_ROWS - TITLE_BAR_WIDTH, NOTIFICATION_COLS, ((max_rows - NOTIFICATION_ROWS) / 2) + TITLE_BAR_WIDTH, MENU_COLS + (max_columns - MENU_COLS - NOTIFICATION_COLS) / 2, NOTIFICATION_COLOR);
	}
	else
	{
		update_window = window;
	}
}

void StateNotificationStudentAddSuccess::mainWindowHandler()
{
	next_state = State_list_t::State_Notification_Student_Add_Success;

	if (screen_update_flag)
	{
		if (window != nullptr)
		{
			getmaxyx(window, max_rows, max_columns);
			mvwprintw(window, NOTIFICATION_ROWS - 7, (NOTIFICATION_COLS - strlen("Student added successfuly.")) / 2, "Student added successfuly.");
			wattron(window, A_BOLD | COLOR_PAIR(NOTIFICATION_BUTTON_COLOR_ON));
			mvwprintw(window, NOTIFICATION_ROWS - 5, (NOTIFICATION_COLS - strlen("    OK    ")) / 2, "    OK    ");
			wattroff(window, A_BOLD | COLOR_PAIR(NOTIFICATION_BUTTON_COLOR_ON));
		}
	}
}

void StateNotificationStudentAddSuccess::inputHandler(int input)
{
	switch (input)
	{
	case KEY_UP:
		break;
	case KEY_DOWN:
		break;
	case (int)13: // ENTER
		title_window_is_rendered = false;
		main_window_is_rendered = false;
		footer_window_is_rendered = false;

		Default_Workspace.destination_state = State_list_t::State_Students_Menu;
		next_state = State_list_t::State_Default_Workspace;
		screen_update_flag = true;

		wbkgd(title_bar_window, NOTIFICATION_TITLE_BAR_COLOR_OFF);
		update_window = title_bar_window;

		break;
	default:
		screen_update_flag = true;
		break;
	}
}