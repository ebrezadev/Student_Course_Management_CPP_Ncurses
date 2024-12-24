#include "states.hpp"
#include <ncurses.h>
#include <cstring>
#include "portal.hpp"
#include "limits_constants.hpp"

inline constexpr int NUMBER_OF_MENU_OPTIONS = 2;

StateNotificationRemoveCoursePopup::StateNotificationRemoveCoursePopup()
{
	current_menu = menu_t::CANCEL;
	next_state = State_list_t::State_Notification_Remove_Course_Popup;
}

StateNotificationRemoveCoursePopup::~StateNotificationRemoveCoursePopup()
{
}

void StateNotificationRemoveCoursePopup::formatHandler()
{
	getmaxyx(stdscr, max_rows, max_columns);

	if (!title_window_is_rendered)
	{
		titleWindowRender((char *)"WARNING", TITLE_BAR_WIDTH, NOTIFICATION_COLS, (max_rows - NOTIFICATION_ROWS) / 2, MENU_COLS + (max_columns - MENU_COLS - NOTIFICATION_COLS) / 2, NOTIFICATION_TITLE_BAR_COLOR_ON);
	}
	else if (!footer_window_is_rendered)
	{
		footerWindowRender((char *)"CANCEL: CANCEL REMOVE | OKAY: REMOVE COURSE", 1, max_columns, max_rows - 1, 0, NOTIFICATION_FOOTER_BAR_COLOR);
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

void StateNotificationRemoveCoursePopup::mainWindowHandler()
{
	next_state = State_list_t::State_Notification_Remove_Course_Popup;

	if (screen_update_flag)
	{
		if (window != nullptr)
		{
			getmaxyx(window, max_rows, max_columns);
			mvwprintw(window, NOTIFICATION_ROWS - 7, (NOTIFICATION_COLS - strlen("Remove course?")) / 2, "Remove course?");

			switch (current_menu)
			{
			case menu_t::CANCEL:
				wattron(window, A_BOLD | COLOR_PAIR(NOTIFICATION_BUTTON_COLOR_ON));
				mvwprintw(window, NOTIFICATION_ROWS - 5, 3, "  CANCEL  ");
				wattroff(window, A_BOLD | COLOR_PAIR(NOTIFICATION_BUTTON_COLOR_ON));
				wattron(window, A_BOLD | COLOR_PAIR(NOTIFICATION_BUTTON_COLOR_OFF));
				mvwprintw(window, NOTIFICATION_ROWS - 5, NOTIFICATION_COLS - 3 - strlen("    OK    "), "    OK    ");
				wattroff(window, A_BOLD | COLOR_PAIR(NOTIFICATION_BUTTON_COLOR_OFF));
				break;
			case menu_t::OKAY:
				wattron(window, A_BOLD | COLOR_PAIR(NOTIFICATION_BUTTON_COLOR_OFF));
				mvwprintw(window, NOTIFICATION_ROWS - 5, 3, "  CANCEL  ");
				wattroff(window, A_BOLD | COLOR_PAIR(NOTIFICATION_BUTTON_COLOR_OFF));
				wattron(window, A_BOLD | COLOR_PAIR(NOTIFICATION_BUTTON_COLOR_ON));
				mvwprintw(window, NOTIFICATION_ROWS - 5, NOTIFICATION_COLS - 3 - strlen("    OK    "), "    OK    ");
				wattroff(window, A_BOLD | COLOR_PAIR(NOTIFICATION_BUTTON_COLOR_ON));
				break;
			default:
				break;
			}
		}
	}
}

void StateNotificationRemoveCoursePopup::inputHandler(int input)
{
	switch (input)
	{
	case KEY_UP:
	case KEY_RIGHT:
		current_menu = (menu_t)(((int)current_menu + NUMBER_OF_MENU_OPTIONS - 1) % NUMBER_OF_MENU_OPTIONS);
		screen_update_flag = true;
		break;
	case KEY_DOWN:
	case KEY_LEFT:
		current_menu = (menu_t)(((int)current_menu + 1) % NUMBER_OF_MENU_OPTIONS);
		screen_update_flag = true;
		break;
	case (int)13: // ENTER
		title_window_is_rendered = false;
		main_window_is_rendered = false;
		footer_window_is_rendered = false;

		next_state = State_list_t::State_Remove_Course;
		screen_update_flag = true;

		wbkgd(title_bar_window, NOTIFICATION_TITLE_BAR_COLOR_OFF);
		update_window = title_bar_window;

		if (current_menu == menu_t::OKAY)
		{
			Remove_Course.remove = true;
		}
		else
		{
			Remove_Course.remove = false;
		}

		current_menu = menu_t::CANCEL;
		break;
	default:
		screen_update_flag = true;
		break;
	}
}
