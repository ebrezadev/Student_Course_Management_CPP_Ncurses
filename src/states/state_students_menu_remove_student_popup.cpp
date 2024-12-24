#include "states.hpp"
#include <ncurses.h>
#include <cstring>
#include "portal.hpp"
#include "limits_constants.hpp"

inline constexpr int NUMBER_OF_MENU_OPTIONS = 2;

StateNotificationRemoveStudentPopup::StateNotificationRemoveStudentPopup()
{
	screen_update_flag = true;
	title_window_is_rendered = false;
	footer_window_is_rendered = false;
	main_window_is_rendered = false;
	current_menu = menu_t::CANCEL;
	window = nullptr;
	title_bar_window = nullptr;
	footer_bar_window = nullptr;
}

StateNotificationRemoveStudentPopup::~StateNotificationRemoveStudentPopup()
{
}

StateNotificationRemoveStudentPopup::state_return_t StateNotificationRemoveStudentPopup::stateFunction(int input)
{
	if (!title_window_is_rendered)
	{
		if (title_bar_window == nullptr)
		{
			getmaxyx(stdscr, max_rows, max_columns);
			title_bar_window = newwin(TITLE_BAR_WIDTH, NOTIFICATION_COLS, (max_rows - NOTIFICATION_ROWS) / 2, MENU_COLS + (max_columns - MENU_COLS - NOTIFICATION_COLS) / 2);
		}
		wbkgd(title_bar_window, COLOR_PAIR(NOTIFICATION_TITLE_BAR_COLOR_ON));
		werase(title_bar_window);
		wattron(title_bar_window, A_BOLD);
		getmaxyx(title_bar_window, max_rows, max_columns);
		wborder(title_bar_window, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
		mvwprintw(title_bar_window, 1, (max_columns - strlen("WARNING")) / 2, "%s", (char *)"WARNING");
		wattroff(title_bar_window, A_BOLD);
		update_window = title_bar_window;
		title_window_is_rendered = true;
	}
	else if (!footer_window_is_rendered)
	{
		if (footer_bar_window == nullptr)
		{
			getmaxyx(stdscr, max_rows, max_columns);
			footer_bar_window = newwin(1, max_columns, max_rows - 1, 0);
			wbkgd(footer_bar_window, COLOR_PAIR(NOTIFICATION_FOOTER_BAR_COLOR));
		}
		werase(footer_bar_window);
		mvwprintw(footer_bar_window, 0, 1, "CANCEL: CANCEL REMOVE | OKAY: REMOVE STUDENT");
		update_window = footer_bar_window;
		footer_window_is_rendered = true;
	}
	else if (!main_window_is_rendered)
	{
		if (window == nullptr)
		{
			getmaxyx(stdscr, max_rows, max_columns);
			window = newwin(NOTIFICATION_ROWS - TITLE_BAR_WIDTH, NOTIFICATION_COLS, ((max_rows - NOTIFICATION_ROWS) / 2) + TITLE_BAR_WIDTH, MENU_COLS + (max_columns - MENU_COLS - NOTIFICATION_COLS) / 2);
			keypad(window, TRUE);
			wbkgd(window, COLOR_PAIR(NOTIFICATION_COLOR));
		}
		update_window = window;
		main_window_is_rendered = true;
		werase(window);
	}
	else
	{
		update_window = window;
	}

	State_list_t next_state = State_list_t::State_Notification_Remove_Student_Popup;

	if (screen_update_flag)
	{
		if (window != nullptr)
		{
			getmaxyx(window, max_rows, max_columns);
			mvwprintw(window, NOTIFICATION_ROWS - 7, (NOTIFICATION_COLS - strlen("Remove student?")) / 2, "Remove student?");

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

		next_state = State_list_t::State_Remove_Student;
		screen_update_flag = true;

		wbkgd(title_bar_window, NOTIFICATION_TITLE_BAR_COLOR_OFF);
		update_window = title_bar_window;

		if(current_menu == menu_t::OKAY)
		{
			Remove_Student.remove = true;
		}
		else
		{
			Remove_Student.remove = false;
		}

		current_menu = menu_t::CANCEL;
		break;
	default:
		screen_update_flag = true;
		break;
	}

	return {next_state, update_window, screen_update_flag};
}