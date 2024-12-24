#include "states.hpp"
#include <ncurses.h>
#include <cstring>
#include "limits_constants.hpp"

inline constexpr int NUMBER_OF_MENU_OPTIONS = 5;

StateCoursesMenu::StateCoursesMenu()
{
	current_menu = menu_t::ADD_THEORETICAL_COURSE;
	next_state = State_list_t::State_Courses_Menu;
}

StateCoursesMenu::~StateCoursesMenu()
{
}

void StateCoursesMenu::inputHandler(int input)
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
		if (current_menu == menu_t::ADD_THEORETICAL_COURSE)
		{
			next_state = State_list_t::State_Add_Theoretical_Course;
			current_menu = menu_t::ADD_THEORETICAL_COURSE;
		}
		else if (current_menu == menu_t::ADD_PRACTICAL_COURSE)
		{
			next_state = State_list_t::State_Add_Practical_Course;
			current_menu = menu_t::ADD_PRACTICAL_COURSE;
		}
		else if (current_menu == menu_t::REMOVE_COURSE)
		{
			next_state = State_list_t::State_Remove_Course;
			current_menu = menu_t::REMOVE_COURSE;
		}
		else if (current_menu == menu_t::SEARCH_COURSE)
		{
			next_state = State_list_t::State_Search_Course;
			current_menu = menu_t::SEARCH_COURSE;
		}
		else if (current_menu == menu_t::BACK)
		{
			current_menu = menu_t::ADD_THEORETICAL_COURSE;
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

void StateCoursesMenu::mainWindowHandler()
{
	next_state = State_list_t::State_Courses_Menu;

	if (screen_update_flag)
	{
		if (window != nullptr)
		{
			if (current_menu == menu_t::ADD_THEORETICAL_COURSE)
			{
				wattron(window, A_BOLD | COLOR_PAIR(MENU_HIGHLIGHT_COLOR));
				mvwprintw(window, 1, 1, "%s", (char *)" ADD THEORETICAL COURSE ");
				wattroff(window, A_BOLD | COLOR_PAIR(MENU_HIGHLIGHT_COLOR));

				wattron(window, A_DIM);
				mvwprintw(window, 2, 1, "%s", (char *)" ADD PRACTICAL COURSE ");
				mvwprintw(window, 3, 1, "%s", (char *)" REMOVE A COURSE ");
				mvwprintw(window, 4, 1, "%s", (char *)" SEARCH A COURSE ");
				mvwprintw(window, 5, 1, "%s", (char *)" BACK ");
				wattroff(window, A_DIM);
			}
			else if (current_menu == menu_t::ADD_PRACTICAL_COURSE)
			{
				wattron(window, A_BOLD | COLOR_PAIR(MENU_HIGHLIGHT_COLOR));
				mvwprintw(window, 2, 1, "%s", (char *)" ADD PRACTICAL COURSE ");
				wattroff(window, A_BOLD | COLOR_PAIR(MENU_HIGHLIGHT_COLOR));

				wattron(window, A_DIM);
				mvwprintw(window, 1, 1, "%s", (char *)" ADD THEORETICAL COURSE ");
				mvwprintw(window, 3, 1, "%s", (char *)" REMOVE A COURSE ");
				mvwprintw(window, 4, 1, "%s", (char *)" SEARCH A COURSE ");
				mvwprintw(window, 5, 1, "%s", (char *)" BACK ");
				wattroff(window, A_DIM);
			}
			else if (current_menu == menu_t::REMOVE_COURSE)
			{
				wattron(window, A_BOLD | COLOR_PAIR(MENU_HIGHLIGHT_COLOR));
				mvwprintw(window, 3, 1, "%s", (char *)" REMOVE A COURSE ");
				wattroff(window, A_BOLD | COLOR_PAIR(MENU_HIGHLIGHT_COLOR));

				wattron(window, A_DIM);
				mvwprintw(window, 1, 1, "%s", (char *)" ADD THEORETICAL COURSE ");
				mvwprintw(window, 2, 1, "%s", (char *)" ADD PRACTICAL COURSE ");
				mvwprintw(window, 4, 1, "%s", (char *)" SEARCH A COURSE ");
				mvwprintw(window, 5, 1, "%s", (char *)" BACK ");
				wattroff(window, A_DIM);
			}
			else if (current_menu == menu_t::SEARCH_COURSE)
			{
				wattron(window, A_BOLD | COLOR_PAIR(MENU_HIGHLIGHT_COLOR));
				mvwprintw(window, 4, 1, "%s", (char *)" SEARCH A COURSE ");
				wattroff(window, A_BOLD | COLOR_PAIR(MENU_HIGHLIGHT_COLOR));

				wattron(window, A_DIM);
				mvwprintw(window, 1, 1, "%s", (char *)" ADD THEORETICAL COURSE ");
				mvwprintw(window, 2, 1, "%s", (char *)" ADD PRACTICAL COURSE ");
				mvwprintw(window, 3, 1, "%s", (char *)" REMOVE A COURSE ");
				mvwprintw(window, 5, 1, "%s", (char *)" BACK ");
				wattroff(window, A_DIM);
			}
			else if (current_menu == menu_t::BACK)
			{
				wattron(window, A_BOLD | COLOR_PAIR(MENU_HIGHLIGHT_COLOR));
				mvwprintw(window, 5, 1, "%s", (char *)" BACK ");
				wattroff(window, A_BOLD | COLOR_PAIR(MENU_HIGHLIGHT_COLOR));

				wattron(window, A_DIM);
				mvwprintw(window, 1, 1, "%s", (char *)" ADD THEORETICAL COURSE ");
				mvwprintw(window, 2, 1, "%s", (char *)" ADD PRACTICAL COURSE ");
				mvwprintw(window, 3, 1, "%s", (char *)" REMOVE A COURSE ");
				mvwprintw(window, 4, 1, "%s", (char *)" SEARCH A COURSE ");
				wattroff(window, A_DIM);
			}
		}
	}
}

void StateCoursesMenu::formatHandler()
{
	getmaxyx(stdscr, max_rows, max_columns);

	if (!title_window_is_rendered)
	{
		titleWindowRender((char *)"COURSES", TITLE_BAR_WIDTH, MENU_COLS, 0, 0, MENU_TITLE_BAR_COLOR_ON);
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