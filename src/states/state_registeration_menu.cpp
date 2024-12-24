#include "states.hpp"
#include <ncurses.h>
#include <cstring>
#include "limits_constants.hpp"

inline constexpr int NUMBER_OF_MENU_OPTIONS = 4;

StateRegisterationMenu::StateRegisterationMenu()
{
	current_menu = menu_t::REGISTER;
	next_state = State_list_t::State_Registeration_Menu;
}

StateRegisterationMenu::~StateRegisterationMenu()
{
}

void StateRegisterationMenu::formatHandler()
{
	getmaxyx(stdscr, max_rows, max_columns);

	if (!title_window_is_rendered)
	{
		titleWindowRender((char *)"REGISTERATION", TITLE_BAR_WIDTH, MENU_COLS, 0, 0, MENU_TITLE_BAR_COLOR_ON);
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

void StateRegisterationMenu::mainWindowHandler()
{
	next_state = State_list_t::State_Registeration_Menu;

	if (screen_update_flag)
	{
		if (window != nullptr)
		{
			if (current_menu == menu_t::REGISTER)
			{
				wattron(window, A_BOLD | COLOR_PAIR(MENU_HIGHLIGHT_COLOR));
				mvwprintw(window, 1, 1, "%s", (char *)" REGISTER A COURSE ");
				wattroff(window, A_BOLD | COLOR_PAIR(MENU_HIGHLIGHT_COLOR));

				wattron(window, A_DIM);
				mvwprintw(window, 2, 1, "%s", (char *)" UNREGISTER A COURSE ");
				mvwprintw(window, 3, 1, "%s", (char *)" ENTER GRADES ");
				mvwprintw(window, 4, 1, "%s", (char *)" BACK ");
				wattroff(window, A_DIM);
			}
			else if (current_menu == menu_t::UNREGISTER)
			{
				wattron(window, A_BOLD | COLOR_PAIR(MENU_HIGHLIGHT_COLOR));
				mvwprintw(window, 2, 1, "%s", (char *)" UNREGISTER A COURSE ");
				wattroff(window, A_BOLD | COLOR_PAIR(MENU_HIGHLIGHT_COLOR));

				wattron(window, A_DIM);
				mvwprintw(window, 1, 1, "%s", (char *)" REGISTER A COURSE ");
				mvwprintw(window, 3, 1, "%s", (char *)" ENTER GRADES ");
				mvwprintw(window, 4, 1, "%s", (char *)" BACK ");
				wattroff(window, A_DIM);
			}
			else if (current_menu == menu_t::ENTER_GRADES)
			{
				wattron(window, A_BOLD | COLOR_PAIR(MENU_HIGHLIGHT_COLOR));
				mvwprintw(window, 3, 1, "%s", (char *)" ENTER GRADES ");
				wattroff(window, A_BOLD | COLOR_PAIR(MENU_HIGHLIGHT_COLOR));

				wattron(window, A_DIM);
				mvwprintw(window, 1, 1, "%s", (char *)" REGISTER A COURSE ");
				mvwprintw(window, 2, 1, "%s", (char *)" UNREGISTER A COURSE ");
				mvwprintw(window, 4, 1, "%s", (char *)" BACK ");
				wattroff(window, A_DIM);
			}
			else if (current_menu == menu_t::BACK)
			{
				wattron(window, A_BOLD | COLOR_PAIR(MENU_HIGHLIGHT_COLOR));
				mvwprintw(window, 4, 1, "%s", (char *)" BACK ");
				wattroff(window, A_BOLD | COLOR_PAIR(MENU_HIGHLIGHT_COLOR));

				wattron(window, A_DIM);
				mvwprintw(window, 1, 1, "%s", (char *)" REGISTER A COURSE ");
				mvwprintw(window, 2, 1, "%s", (char *)" UNREGISTER A COURSE ");
				mvwprintw(window, 3, 1, "%s", (char *)" ENTER GRADES ");
				wattroff(window, A_DIM);
			}
		}
	}
}

void StateRegisterationMenu::inputHandler(int input)
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
		if (current_menu == menu_t::REGISTER)
		{
			next_state = State_list_t::State_Course_Register;
			current_menu = menu_t::REGISTER;
		}
		else if (current_menu == menu_t::UNREGISTER)
		{
			current_menu = menu_t::UNREGISTER;
			next_state = State_list_t::State_Course_Unregister;
		}
		else if (current_menu == menu_t::ENTER_GRADES)
		{
			current_menu = menu_t::ENTER_GRADES;
			next_state = State_list_t::State_Grade_Entry;
		}
		else if (current_menu == menu_t::BACK)
		{
			current_menu = menu_t::REGISTER;
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
	default:
		screen_update_flag = true;
		break;
	}
}
