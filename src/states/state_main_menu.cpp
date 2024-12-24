#include "states.hpp"
#include <ncurses.h>
#include <cstring>
#include "limits_constants.hpp"

inline constexpr int NUMBER_OF_MENU_OPTIONS = 5;

StateMainMenu::StateMainMenu()
{
	current_menu = menu_t::STUDENTS;
	next_state = State_list_t::State_Main_Menu;
}

StateMainMenu::~StateMainMenu()
{
}

void StateMainMenu::inputHandler(int input)
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
		if (current_menu == menu_t::STUDENTS)
		{
			next_state = State_list_t::State_Students_Menu;
		}
		else if (current_menu == menu_t::COURSES)
		{
			next_state = State_list_t::State_Courses_Menu;
		}
		else if (current_menu == menu_t::COURSE_REGISTRATION)
		{
			next_state = State_list_t::State_Registeration_Menu;
		}
		else if (current_menu == menu_t::REPORTS)
		{
			next_state = State_list_t::State_Reports_Menu;
		}
		else if (current_menu == menu_t::EXIT)
		{
			next_state = State_list_t::State_Save_To_File;
		}

		current_menu = menu_t::STUDENTS;

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

void StateMainMenu::mainWindowHandler()
{
	next_state = State_list_t::State_Main_Menu;

	if (screen_update_flag)
	{
		werase(window);

		if (window != nullptr)
		{
			if (current_menu == menu_t::STUDENTS)
			{
				wattron(window, A_BOLD | COLOR_PAIR(MENU_HIGHLIGHT_COLOR));
				mvwprintw(window, 1, 1, "%s", (char *)" STUDENTS ");
				wattroff(window, A_BOLD | COLOR_PAIR(MENU_HIGHLIGHT_COLOR));

				wattron(window, A_DIM);
				mvwprintw(window, 2, 1, "%s", (char *)" COURSES ");
				mvwprintw(window, 3, 1, "%s", (char *)" COURSE REGISTRATION ");
				mvwprintw(window, 4, 1, "%s", (char *)" REPORTS ");
				mvwprintw(window, 5, 1, "%s", (char *)" EXIT ");
				wattroff(window, A_DIM);
			}
			else if (current_menu == menu_t::COURSES)
			{
				wattron(window, A_BOLD | COLOR_PAIR(MENU_HIGHLIGHT_COLOR));
				mvwprintw(window, 2, 1, "%s", (char *)" COURSES ");
				wattroff(window, A_BOLD | COLOR_PAIR(MENU_HIGHLIGHT_COLOR));

				wattron(window, A_DIM);
				mvwprintw(window, 1, 1, "%s", (char *)" STUDENTS ");
				mvwprintw(window, 3, 1, "%s", (char *)" COURSE REGISTRATION ");
				mvwprintw(window, 4, 1, "%s", (char *)" REPORTS ");
				mvwprintw(window, 5, 1, "%s", (char *)" EXIT ");
				wattroff(window, A_DIM);
			}
			else if (current_menu == menu_t::COURSE_REGISTRATION)
			{
				wattron(window, A_BOLD | COLOR_PAIR(MENU_HIGHLIGHT_COLOR));
				mvwprintw(window, 3, 1, "%s", (char *)" COURSE REGISTRATION ");
				wattroff(window, A_BOLD | COLOR_PAIR(MENU_HIGHLIGHT_COLOR));

				wattron(window, A_DIM);
				mvwprintw(window, 1, 1, "%s", (char *)" STUDENTS ");
				mvwprintw(window, 2, 1, "%s", (char *)" COURSES ");
				mvwprintw(window, 4, 1, "%s", (char *)" REPORTS ");
				mvwprintw(window, 5, 1, "%s", (char *)" EXIT ");
				wattroff(window, A_DIM);
			}
			else if (current_menu == menu_t::REPORTS)
			{
				wattron(window, A_BOLD | COLOR_PAIR(MENU_HIGHLIGHT_COLOR));
				mvwprintw(window, 4, 1, "%s", (char *)" REPORTS ");
				wattroff(window, A_BOLD | COLOR_PAIR(MENU_HIGHLIGHT_COLOR));

				wattron(window, A_DIM);
				mvwprintw(window, 1, 1, "%s", (char *)" STUDENTS ");
				mvwprintw(window, 2, 1, "%s", (char *)" COURSES ");
				mvwprintw(window, 3, 1, "%s", (char *)" COURSE REGISTRATION ");
				mvwprintw(window, 5, 1, "%s", (char *)" EXIT ");
				wattroff(window, A_DIM);
			}
			else if (current_menu == menu_t::EXIT)
			{
				wattron(window, A_BOLD | COLOR_PAIR(MENU_HIGHLIGHT_COLOR));
				mvwprintw(window, 5, 1, "%s", (char *)" EXIT ");
				wattroff(window, A_BOLD | COLOR_PAIR(MENU_HIGHLIGHT_COLOR));

				wattron(window, A_DIM);
				mvwprintw(window, 1, 1, "%s", (char *)" STUDENTS ");
				mvwprintw(window, 2, 1, "%s", (char *)" COURSES ");
				mvwprintw(window, 3, 1, "%s", (char *)" COURSE REGISTRATION ");
				mvwprintw(window, 4, 1, "%s", (char *)" REPORTS ");
				wattroff(window, A_DIM);
			}
		}
	}
}

void StateMainMenu::formatHandler()
{
	getmaxyx(stdscr, max_rows, max_columns);

	if (!title_window_is_rendered)
	{
		titleWindowRender((char *)"MAIN MENU", TITLE_BAR_WIDTH, MENU_COLS, 0, 0, MENU_TITLE_BAR_COLOR_ON);
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