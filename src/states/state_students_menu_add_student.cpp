#include "states.hpp"
#include <ncurses.h>
#include <cstring>
#include "portal.hpp"
#include "limits_constants.hpp"

inline constexpr int NUMBER_OF_MENU_OPTIONS = 5;

StateAddStudent::StateAddStudent()
{
	current_menu = menu_t::FIRST_NAME;

	first_name_cursor = 0;
	last_name_cursor = 0;
	id_cursor = 0;

	invalid_input = false;

	next_state = State_list_t::State_Add_Student;
}

StateAddStudent::~StateAddStudent()
{
}

void StateAddStudent::inputHandler(int input)
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
		if (current_menu == menu_t::OKAY)
		{
			if (first_name.length() != 0 && last_name.length() != 0 && id.length() != 0)
			{
				if (!portal.exists())
				{
					portal.newPortal();
				}

				size_t dummy_index;
				if (portal.searchStudentByID(id, dummy_index) != Error_code_t::ERROR_OK)
				{
					portal.addStudent(first_name, last_name, id);

					first_name.clear();
					last_name.clear();
					id.clear();

					first_name_cursor = last_name_cursor = id_cursor = 0;

					current_menu = menu_t::FIRST_NAME;

					title_window_is_rendered = false;
					main_window_is_rendered = false;
					footer_window_is_rendered = false;

					invalid_input = false;

					wbkgd(title_bar_window, COLOR_PAIR(WORKSPACE_TITLE_BAR_COLOR_OFF));

					update_window = title_bar_window;
					next_state = State_list_t::State_Notification_Student_Add_Success;
				}
				else
				{
					id.clear();
					id_cursor = 0;

					title_window_is_rendered = false;
					main_window_is_rendered = false;
					footer_window_is_rendered = false;

					invalid_input = true;
					current_menu = menu_t::ID;

					wbkgd(title_bar_window, COLOR_PAIR(WORKSPACE_TITLE_BAR_COLOR_OFF));

					update_window = title_bar_window;
					next_state = State_list_t::State_Notification_Student_Add_Duplicate;
				}
			}
			else
			{

				title_window_is_rendered = false;
				main_window_is_rendered = false;
				footer_window_is_rendered = false;

				invalid_input = true;

				current_menu = menu_t::FIRST_NAME;

				wbkgd(title_bar_window, COLOR_PAIR(WORKSPACE_TITLE_BAR_COLOR_OFF));

				update_window = title_bar_window;
				next_state = State_list_t::State_Notification_Student_Add_Fail;
			}
		}
		else if (current_menu == menu_t::CANCEL)
		{
			first_name.clear();
			last_name.clear();
			id.clear();

			first_name_cursor = last_name_cursor = id_cursor = 0;

			current_menu = menu_t::FIRST_NAME;

			title_window_is_rendered = false;
			main_window_is_rendered = false;
			footer_window_is_rendered = false;

			invalid_input = false;

			wbkgd(title_bar_window, COLOR_PAIR(WORKSPACE_TITLE_BAR_COLOR_OFF));

			update_window = title_bar_window;
			Default_Workspace.destination_state = State_list_t::State_Students_Menu;
			next_state = State_list_t::State_Default_Workspace;
		}

		screen_update_flag = true;
		break;
	case KEY_LEFT:
	case KEY_RIGHT:
		if (current_menu == menu_t::CANCEL)
		{
			current_menu = menu_t::OKAY;
		}
		else if (current_menu == menu_t::OKAY)
		{
			current_menu = menu_t::CANCEL;
		}
		screen_update_flag = true;
		break;
	case KEY_BACKSPACE:
		if (current_menu == menu_t::FIRST_NAME)
		{
			if (first_name.length() > 0)
			{
				mvwaddch(window, 1, strlen(" First Name: ") + 2 + first_name_cursor, ' ');
				first_name.pop_back();
				first_name_cursor--;
			}
		}
		else if (current_menu == menu_t::LAST_NAME)
		{
			if (last_name.length() > 0)
			{
				mvwaddch(window, 2, strlen(" Last Name: ") + 2 + last_name_cursor, ' ');
				last_name.pop_back();
				last_name_cursor--;
			}
		}
		else if (current_menu == menu_t::ID)
		{
			if (id.length() > 0)
			{
				mvwaddch(window, 3, strlen(" ID: ") + 2 + id_cursor, ' ');
				id.pop_back();
				id_cursor--;
			}
		}
		break;
	case -1:
		break;
	default:
		if ((input >= 32) && (input <= 126))
		{
			if (current_menu == menu_t::FIRST_NAME && first_name_cursor < MAXIMUM_FIRST_NAME_LENGTH)
			{
				first_name.append((char *)&input);
				mvwprintw(window, 1, strlen(" First Name: ") + 2 + first_name_cursor, "%c", input);
				first_name_cursor++;
			}
			else if (current_menu == menu_t::LAST_NAME && last_name_cursor < MAXIMUM_LAST_NAME_LENGTH)
			{
				last_name.append((char *)&input);
				mvwprintw(window, 2, strlen(" Last Name: ") + 2 + last_name_cursor, "%c", input);
				last_name_cursor++;
			}
			else if (current_menu == menu_t::ID && id_cursor < MAXIMUM_STUDENT_ID_LENGTH)
			{
				id.append((char *)&input);
				mvwprintw(window, 3, strlen(" ID: ") + 2 + id_cursor, "%c", input);
				id_cursor++;
			}
		}

		screen_update_flag = true;
		break;
	}
}

void StateAddStudent::mainWindowHandler()
{
	next_state = State_list_t::State_Add_Student;

	if (screen_update_flag)
	{
		if (window != nullptr)
		{
			if (invalid_input)
			{
				mvwprintw(window, 1, 0, " ");
				mvwprintw(window, 2, 0, " ");
				mvwprintw(window, 3, 0, " ");

				if (first_name.length() == 0)
				{
					wattron(window, A_BOLD | COLOR_PAIR(WORKSPACE_X_COLOR));
					mvwprintw(window, 1, 0, "X");
					wattroff(window, A_BOLD | COLOR_PAIR(WORKSPACE_X_COLOR));
				}

				if (last_name.length() == 0)
				{
					wattron(window, A_BOLD | COLOR_PAIR(WORKSPACE_X_COLOR));
					mvwprintw(window, 2, 0, "X");
					wattroff(window, A_BOLD | COLOR_PAIR(WORKSPACE_X_COLOR));
				}

				if (id.length() == 0)
				{
					wattron(window, A_BOLD | COLOR_PAIR(WORKSPACE_X_COLOR));
					mvwprintw(window, 3, 0, "X");
					wattroff(window, A_BOLD | COLOR_PAIR(WORKSPACE_X_COLOR));
				}

				mvwprintw(window, 1, strlen(" First Name: ") + 2, "%s", first_name.c_str());
				mvwprintw(window, 2, strlen(" Last Name: ") + 2, "%s", last_name.c_str());
				mvwprintw(window, 3, strlen(" ID: ") + 2, "%s", id.c_str());
			}

			if (current_menu == menu_t::FIRST_NAME)
			{
				wattron(window, A_BOLD | COLOR_PAIR(WORKSPACE_HIGHLIGHT_COLOR));
				mvwprintw(window, 1, 1, "%s", (char *)" First Name: ");
				wattroff(window, A_BOLD | COLOR_PAIR(WORKSPACE_HIGHLIGHT_COLOR));

				wattron(window, A_DIM);
				mvwprintw(window, 2, 1, "%s", (char *)" Last Name: ");
				mvwprintw(window, 3, 1, "%s", (char *)" ID: ");
				wattroff(window, A_DIM);
				wattron(window, COLOR_PAIR(WORKSPACE_BUTTON_COLOR_OFF));
				mvwprintw(window, 5, 1, "%s", (char *)"  Cancel  ");
				mvwprintw(window, 5, 12, "%s", (char *)"    OK    ");
				wattroff(window, COLOR_PAIR(WORKSPACE_BUTTON_COLOR_OFF));

				wattron(window, A_BLINK);
				mvwaddch(window, 1, strlen(" First Name: ") + 2 + first_name_cursor, ACS_CKBOARD);
				wattroff(window, A_BLINK);
				mvwaddch(window, 2, strlen(" Last Name: ") + 2 + last_name_cursor, ' ');
				mvwaddch(window, 3, strlen(" ID: ") + 2 + id_cursor, ' ');
			}
			else if (current_menu == menu_t::LAST_NAME)
			{
				wattron(window, A_BOLD | COLOR_PAIR(WORKSPACE_HIGHLIGHT_COLOR));
				mvwprintw(window, 2, 1, "%s", (char *)" Last Name: ");
				wattroff(window, A_BOLD | COLOR_PAIR(WORKSPACE_HIGHLIGHT_COLOR));

				wattron(window, A_DIM);
				mvwprintw(window, 1, 1, "%s", (char *)" First Name: ");
				mvwprintw(window, 3, 1, "%s", (char *)" ID: ");
				wattroff(window, A_DIM);
				wattron(window, COLOR_PAIR(WORKSPACE_BUTTON_COLOR_OFF));
				mvwprintw(window, 5, 1, "%s", (char *)"  Cancel  ");
				mvwprintw(window, 5, 12, "%s", (char *)"    OK    ");
				wattroff(window, COLOR_PAIR(WORKSPACE_BUTTON_COLOR_OFF));

				wattron(window, A_BLINK);
				mvwaddch(window, 2, strlen(" Last Name: ") + 2 + last_name_cursor, ACS_CKBOARD);
				wattroff(window, A_BLINK);
				mvwaddch(window, 1, strlen(" First Name: ") + 2 + first_name_cursor, ' ');
				mvwaddch(window, 3, strlen(" ID: ") + 2 + id_cursor, ' ');
			}
			else if (current_menu == menu_t::ID)
			{
				wattron(window, A_BOLD | COLOR_PAIR(WORKSPACE_HIGHLIGHT_COLOR));
				mvwprintw(window, 3, 1, "%s", (char *)" ID: ");
				wattroff(window, A_BOLD | COLOR_PAIR(WORKSPACE_HIGHLIGHT_COLOR));

				wattron(window, A_DIM);
				mvwprintw(window, 1, 1, "%s", (char *)" First Name: ");
				mvwprintw(window, 2, 1, "%s", (char *)" Last Name: ");
				wattroff(window, A_DIM);
				wattron(window, COLOR_PAIR(WORKSPACE_BUTTON_COLOR_OFF));
				mvwprintw(window, 5, 1, "%s", (char *)"  Cancel  ");
				mvwprintw(window, 5, 12, "%s", (char *)"    OK    ");
				wattroff(window, COLOR_PAIR(WORKSPACE_BUTTON_COLOR_OFF));

				wattron(window, A_BLINK);
				mvwaddch(window, 3, strlen(" ID: ") + 2 + id_cursor, ACS_CKBOARD);
				wattroff(window, A_BLINK);
				mvwaddch(window, 1, strlen(" First Name: ") + 2 + first_name_cursor, ' ');
				mvwaddch(window, 2, strlen(" Last Name: ") + 2 + last_name_cursor, ' ');
			}
			else if (current_menu == menu_t::CANCEL)
			{
				wattron(window, A_BOLD | COLOR_PAIR(WORKSPACE_BUTTON_COLOR_ON));
				mvwprintw(window, 5, 1, "%s", (char *)"  Cancel  ");
				wattroff(window, A_BOLD | COLOR_PAIR(WORKSPACE_BUTTON_COLOR_ON));

				wattron(window, A_DIM);
				mvwprintw(window, 1, 1, "%s", (char *)" First Name: ");
				mvwprintw(window, 2, 1, "%s", (char *)" Last Name: ");
				mvwprintw(window, 3, 1, "%s", (char *)" ID: ");
				wattroff(window, A_DIM);
				wattron(window, COLOR_PAIR(WORKSPACE_BUTTON_COLOR_OFF));
				mvwprintw(window, 5, 12, "%s", (char *)"    OK    ");
				wattroff(window, COLOR_PAIR(WORKSPACE_BUTTON_COLOR_OFF));

				mvwaddch(window, 1, strlen(" First Name: ") + 2 + first_name_cursor, ' ');
				mvwaddch(window, 2, strlen(" Last Name: ") + 2 + last_name_cursor, ' ');
				mvwaddch(window, 3, strlen(" ID: ") + 2 + id_cursor, ' ');
			}
			else if (current_menu == menu_t::OKAY)
			{
				wattron(window, A_BOLD | COLOR_PAIR(WORKSPACE_BUTTON_COLOR_ON));
				mvwprintw(window, 5, 12, "%s", (char *)"    OK    ");
				wattroff(window, A_BOLD | COLOR_PAIR(WORKSPACE_BUTTON_COLOR_ON));

				wattron(window, A_DIM);
				mvwprintw(window, 1, 1, "%s", (char *)" First Name: ");
				mvwprintw(window, 2, 1, "%s", (char *)" Last Name: ");
				mvwprintw(window, 3, 1, "%s", (char *)" ID: ");
				wattroff(window, A_DIM);
				wattron(window, COLOR_PAIR(WORKSPACE_BUTTON_COLOR_OFF));
				mvwprintw(window, 5, 1, "%s", (char *)"  Cancel  ");
				wattroff(window, COLOR_PAIR(WORKSPACE_BUTTON_COLOR_OFF));

				mvwaddch(window, 1, strlen(" First Name: ") + 2 + first_name_cursor, ' ');
				mvwaddch(window, 2, strlen(" Last Name: ") + 2 + last_name_cursor, ' ');
				mvwaddch(window, 3, strlen(" ID: ") + 2 + id_cursor, ' ');
			}
		}
	}
}

void StateAddStudent::formatHandler()
{
	getmaxyx(stdscr, max_rows, max_columns);

	if (!title_window_is_rendered)
	{
		titleWindowRender((char *)"WORKSPACE: ADD STUDENT", TITLE_BAR_WIDTH, max_columns - MENU_COLS, 0, MENU_COLS, WORKSPACE_TITLE_BAR_COLOR_ON);
	}
	else if (!footer_window_is_rendered)
	{
		footerWindowRender((char *)"UP/DOWN: NAVIGATE | CANCEL: CANCEL ADD STUDENT | OKAY: ADD STUDENT", FOOTER_BAR_WIDTH, max_columns, max_rows - FOOTER_BAR_WIDTH, 0, WORKSPACE_FOOTER_BAR_COLOR);
	}
	else if (!main_window_is_rendered)
	{
		mainWindowRender(max_rows - FOOTER_BAR_WIDTH - TITLE_BAR_WIDTH, max_columns - MENU_COLS, TITLE_BAR_WIDTH, MENU_COLS, WORKSPACE_COLOR);
	}
	else
	{
		update_window = window;
	}
}