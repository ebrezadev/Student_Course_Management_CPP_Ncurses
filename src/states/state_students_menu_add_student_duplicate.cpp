#include "states.hpp"
#include <ncurses.h>
#include <cstring>
#include "portal.hpp"
#include "limits_constants.hpp"

inline constexpr int NUMBER_OF_MENU_OPTIONS = 1;

StateNotificationStudentAddDuplicate::StateNotificationStudentAddDuplicate()
{
	current_menu = menu_t::BACK;
	next_state = State_list_t::State_Notification_Student_Add_Duplicate;
}

StateNotificationStudentAddDuplicate::~StateNotificationStudentAddDuplicate()
{
}

void StateNotificationStudentAddDuplicate::formatHandler()
{
	getmaxyx(stdscr, max_rows, max_columns);

	if (!title_window_is_rendered)
	{
		titleWindowRender((char *)"FAILED", TITLE_BAR_WIDTH, NOTIFICATION_COLS, (max_rows - NOTIFICATION_ROWS) / 2, MENU_COLS + (max_columns - MENU_COLS - NOTIFICATION_COLS) / 2, NOTIFICATION_FAIL_TITLE_BAR_COLOR_ON);
	}
	else if (!footer_window_is_rendered)
	{
		footerWindowRender((char *)"OKAY: BACK", 1, max_columns, max_rows - 1, 0, NOTIFICATION_FAIL_FOOTER_BAR_COLOR);
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

void StateNotificationStudentAddDuplicate::mainWindowHandler()
{
	next_state = State_list_t::State_Notification_Student_Add_Duplicate;

	if (screen_update_flag)
	{
		if (window != nullptr)
		{
			getmaxyx(window, max_rows, max_columns);
			mvwprintw(window, NOTIFICATION_ROWS - 7, (NOTIFICATION_COLS - strlen("ID already exists.")) / 2, "ID already exists.");
			wattron(window, A_BOLD | COLOR_PAIR(NOTIFICATION_FAIL_BUTTON_COLOR_ON));
			mvwprintw(window, NOTIFICATION_ROWS - 5, (NOTIFICATION_COLS - strlen("   BACK   ")) / 2, "   BACK   ");
			wattroff(window, A_BOLD | COLOR_PAIR(NOTIFICATION_FAIL_BUTTON_COLOR_ON));
		}
	}
}

void StateNotificationStudentAddDuplicate::inputHandler(int input)
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

		next_state = State_list_t::State_Add_Student;
		screen_update_flag = true;

		wbkgd(title_bar_window, NOTIFICATION_TITLE_BAR_COLOR_OFF);
		update_window = title_bar_window;
		break;
	default:
		screen_update_flag = true;
		break;
	}
}