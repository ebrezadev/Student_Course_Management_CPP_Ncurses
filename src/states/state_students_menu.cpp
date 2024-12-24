#include "states.hpp"
#include <ncurses.h>
#include <cstring>
#include "limits_constants.hpp"

inline constexpr int NUMBER_OF_MENU_OPTIONS = 4;

StateStudentsMenu::StateStudentsMenu()
{
	current_menu = menu_t::ADD_STUDENT;
	next_state = State_list_t::State_Students_Menu;
}

StateStudentsMenu::~StateStudentsMenu()
{
}

void StateStudentsMenu::inputHandler(int input)
{
	switch (input)
	{
	case KEY_UP:
		current_menu = (menu_t)(((int)current_menu + NUMBER_OF_MENU_OPTIONS - 1) % NUMBER_OF_MENU_OPTIONS);
		screen_update_flag = true;
		break;
	case KEY_DOWN:
		current_menu = (menu_t)(((int)current_menu + 1) % NUMBER_OF_MENU_OPTIONS);
		screen_update_flag = true;
		break;
	case (int)13: // ENTER
		if (current_menu == menu_t::ADD_STUDENT)
		{
			next_state = State_list_t::State_Add_Student;
			current_menu = menu_t::ADD_STUDENT;
		}
		else if (current_menu == menu_t::REMOVE_STUDENT)
		{
			current_menu = menu_t::REMOVE_STUDENT;
			next_state = State_list_t::State_Remove_Student;
		}
		else if (current_menu == menu_t::SEARCH_STUDENT)
		{
			current_menu = menu_t::SEARCH_STUDENT;
			next_state = State_list_t::State_Search_Student;
		}
		else if (current_menu == menu_t::BACK)
		{
			current_menu = menu_t::ADD_STUDENT;
			next_state = State_list_t::State_Main_Menu;
		}
		title_window_is_rendered = false;
		footer_window_is_rendered = false;
		main_window_is_rendered = false;

		screen_update_flag = true;
		wbkgd(title_bar_window, COLOR_PAIR(MENU_TITLE_BAR_COLOR_OFF));

		update_window = title_bar_window;
		break;
	case KEY_LEFT:
		screen_update_flag = false;
		break;
	case KEY_RIGHT:
		screen_update_flag = false;
		break;
	case ' ':
		screen_update_flag = false;
		break;
	default:
		screen_update_flag = true;
		break;
	}
}

void StateStudentsMenu::mainWindowHandler()
{
	next_state = State_list_t::State_Students_Menu;

	if (screen_update_flag)
	{
		if (window != nullptr)
		{
			if (current_menu == menu_t::ADD_STUDENT)
			{
				wattron(window, A_BOLD | COLOR_PAIR(MENU_HIGHLIGHT_COLOR));
				mvwprintw(window, 1, 1, "%s", (char *)" ADD A STUDENT ");
				wattroff(window, A_BOLD | COLOR_PAIR(MENU_HIGHLIGHT_COLOR));

				wattron(window, A_DIM);
				mvwprintw(window, 2, 1, "%s", (char *)" REMOVE A STUDENT ");
				mvwprintw(window, 3, 1, "%s", (char *)" SEARCH A STUDENT ");
				mvwprintw(window, 4, 1, "%s", (char *)" BACK ");
				wattroff(window, A_DIM);
			}
			else if (current_menu == menu_t::REMOVE_STUDENT)
			{
				wattron(window, A_BOLD | COLOR_PAIR(MENU_HIGHLIGHT_COLOR));
				mvwprintw(window, 2, 1, "%s", (char *)" REMOVE A STUDENT ");
				wattroff(window, A_BOLD | COLOR_PAIR(MENU_HIGHLIGHT_COLOR));

				wattron(window, A_DIM);
				mvwprintw(window, 1, 1, "%s", (char *)" ADD A STUDENT ");
				mvwprintw(window, 3, 1, "%s", (char *)" SEARCH A STUDENT ");
				mvwprintw(window, 4, 1, "%s", (char *)" BACK ");
				wattroff(window, A_DIM);
			}
			else if (current_menu == menu_t::SEARCH_STUDENT)
			{
				wattron(window, A_BOLD | COLOR_PAIR(MENU_HIGHLIGHT_COLOR));
				mvwprintw(window, 3, 1, "%s", (char *)" SEARCH A STUDENT ");
				wattroff(window, A_BOLD | COLOR_PAIR(MENU_HIGHLIGHT_COLOR));

				wattron(window, A_DIM);
				mvwprintw(window, 1, 1, "%s", (char *)" ADD A STUDENT ");
				mvwprintw(window, 2, 1, "%s", (char *)" REMOVE A STUDENT ");
				mvwprintw(window, 4, 1, "%s", (char *)" BACK ");
				wattroff(window, A_DIM);
			}
			else if (current_menu == menu_t::BACK)
			{
				wattron(window, A_BOLD | COLOR_PAIR(MENU_HIGHLIGHT_COLOR));
				mvwprintw(window, 4, 1, "%s", (char *)" BACK ");
				wattroff(window, A_BOLD | COLOR_PAIR(MENU_HIGHLIGHT_COLOR));

				wattron(window, A_DIM);
				mvwprintw(window, 1, 1, "%s", (char *)" ADD A STUDENT ");
				mvwprintw(window, 2, 1, "%s", (char *)" REMOVE A STUDENT ");
				mvwprintw(window, 3, 1, "%s", (char *)" SEARCH A STUDENT ");
				wattroff(window, A_DIM);
			}
		}
	}
}

void StateStudentsMenu::formatHandler()
{
	getmaxyx(stdscr, max_rows, max_columns);

	if (!title_window_is_rendered)
	{
		titleWindowRender((char *)"STUDENTS", TITLE_BAR_WIDTH, MENU_COLS, 0, 0, MENU_TITLE_BAR_COLOR_ON);
	}
	else if (!footer_window_is_rendered)
	{
		footerWindowRender((char *)"UP/DOWN: NAVIGATE | ENTER: SELECT", FOOTER_BAR_WIDTH, max_columns, max_rows - FOOTER_BAR_WIDTH, 0, MENU_FOOTER_BAR_COLOR);
	}
	else if (!main_window_is_rendered)
	{
		mainWindowRender(max_rows - FOOTER_BAR_WIDTH - TITLE_BAR_WIDTH, MENU_COLS, TITLE_BAR_WIDTH, 0, MENU_COLOR);
	}
	else
	{
		update_window = window;
	}
}