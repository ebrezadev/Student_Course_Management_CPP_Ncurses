#include "states.hpp"
#include <ncurses.h>
#include <cstring>
#include "portal.hpp"
#include "limits_constants.hpp"

inline constexpr int NUMBER_OF_MENU_OPTIONS = 3;

StateSearchStudent::StateSearchStudent()
{
	current_menu = menu_t::STUDENT_ID;
	id_cursor = 0;
	next_state = State_list_t::State_Search_Student;
}

StateSearchStudent::~StateSearchStudent()
{
}

void StateSearchStudent::formatHandler()
{
	getmaxyx(stdscr, max_rows, max_columns);

	if (!title_window_is_rendered)
	{
		titleWindowRender((char *)"WORKSPACE: SEARCH STUDENT", TITLE_BAR_WIDTH, max_columns - MENU_COLS, 0, MENU_COLS, WORKSPACE_TITLE_BAR_COLOR_ON);
	}
	else if (!footer_window_is_rendered)
	{
		footerWindowRender((char *)"UP/DOWN: NAVIGATE | CANCEL: STUDENT MENU | SEARCH: SEARCH STUDENT", FOOTER_BAR_WIDTH, max_columns, max_rows - FOOTER_BAR_WIDTH, 0, WORKSPACE_FOOTER_BAR_COLOR);
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

void StateSearchStudent::mainWindowHandler()
{
	next_state = State_list_t::State_Search_Student;

	if (screen_update_flag)
	{
		if (window != nullptr)
		{
			if (current_menu == menu_t::STUDENT_ID)
			{
				wattron(window, A_BOLD | COLOR_PAIR(WORKSPACE_HIGHLIGHT_COLOR));
				mvwprintw(window, 1, 1, "%s", (char *)" Student ID: ");
				wattroff(window, A_BOLD | COLOR_PAIR(WORKSPACE_HIGHLIGHT_COLOR));

				wattron(window, COLOR_PAIR(WORKSPACE_BUTTON_COLOR_OFF));
				mvwprintw(window, 3, 1, "%s", (char *)"  Cancel  ");
				mvwprintw(window, 3, 12, "%s", (char *)"  Search  ");
				wattroff(window, COLOR_PAIR(WORKSPACE_BUTTON_COLOR_OFF));

				wattron(window, A_BLINK);
				mvwaddch(window, 1, strlen(" Student ID: ") + 2 + id_cursor, ACS_CKBOARD);
				wattroff(window, A_BLINK);
			}
			else if (current_menu == menu_t::CANCEL)
			{
				wattron(window, A_BOLD | COLOR_PAIR(WORKSPACE_BUTTON_COLOR_ON));
				mvwprintw(window, 3, 1, "%s", (char *)"  Cancel  ");
				wattroff(window, A_BOLD | COLOR_PAIR(WORKSPACE_BUTTON_COLOR_ON));

				wattron(window, A_DIM);
				mvwprintw(window, 1, 1, "%s", (char *)" Student ID: ");
				wattroff(window, A_DIM);
				wattron(window, COLOR_PAIR(WORKSPACE_BUTTON_COLOR_OFF));
				mvwprintw(window, 3, 12, "%s", (char *)"  Search  ");
				wattroff(window, COLOR_PAIR(WORKSPACE_BUTTON_COLOR_OFF));

				mvwaddch(window, 1, strlen(" Student ID: ") + 2 + id_cursor, ' ');
			}
			else if (current_menu == menu_t::SEARCH)
			{
				wattron(window, A_BOLD | COLOR_PAIR(WORKSPACE_BUTTON_COLOR_ON));
				mvwprintw(window, 3, 12, "%s", (char *)"  Search  ");
				wattroff(window, A_BOLD | COLOR_PAIR(WORKSPACE_BUTTON_COLOR_ON));

				wattron(window, A_DIM);
				mvwprintw(window, 1, 1, "%s", (char *)" Student ID: ");
				wattroff(window, A_DIM);
				wattron(window, COLOR_PAIR(WORKSPACE_BUTTON_COLOR_OFF));
				mvwprintw(window, 3, 1, "%s", (char *)"  Cancel  ");
				wattroff(window, COLOR_PAIR(WORKSPACE_BUTTON_COLOR_OFF));

				mvwaddch(window, 1, strlen(" Student ID: ") + 2 + id_cursor, ' ');
			}
		}
	}
}

void StateSearchStudent::inputHandler(int input)
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
		if (current_menu == menu_t::SEARCH)
		{
			getmaxyx(window, max_rows, max_columns);

			for (int index = 0; index < max_columns; index++)
			{
				mvwaddch(window, 1, index, ' ');
				mvwaddch(window, 5, index, ' ');
				mvwaddch(window, 6, index, ' ');
			}

			if (id.length() != 0)
			{
				if (portal.exists())
				{
					size_t index = 0;

					if (portal.searchStudentByID(id, index) == Error_code_t::ERROR_OK)
					{
						getmaxyx(window, max_rows, max_columns);

						int list_cols = max_columns - 9;
						int last_name_cols = (LAST_NAME_COLS_PERCENT * list_cols) / 100;
						int first_name_cols = (FIRST_NAME_COLS_PERCENT * list_cols) / 100;
						int id_cols = (ID_COLS_PERCENT * list_cols) / 100;

						wattron(window, COLOR_PAIR(WORKSPACE_LIST_GUIDE));

						mvwaddch(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP, 1, ACS_VLINE);
						mvwprintw(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP, 2, "Index");
						mvwaddch(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP, 2 + strlen("Index"), ACS_VLINE);

						for (size_t index = 0; index < (last_name_cols - strlen("Last Name")) / 2; index++)
						{
							mvwaddch(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + index, ' ');
						}

						mvwprintw(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + (last_name_cols - strlen("Last Name")) / 2, "Last Name");

						for (size_t index = 0; index < (last_name_cols - strlen("Last Name")) / 2; index++)
						{
							mvwaddch(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + (last_name_cols + strlen("Last Name")) / 2 + index, ' ');
						}

						mvwaddch(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP, 2 + strlen("Index") + last_name_cols, ACS_VLINE);

						for (size_t index = 0; index < (first_name_cols - strlen("First Name")) / 2; index++)
						{
							mvwaddch(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + last_name_cols + index, ' ');
						}

						mvwprintw(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + last_name_cols + (first_name_cols - strlen("First Name")) / 2, "First Name");

						for (size_t index = 0; index < (first_name_cols - strlen("First Name")) / 2; index++)
						{
							mvwaddch(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + last_name_cols + (first_name_cols + strlen("First Name")) / 2 + index, ' ');
						}

						mvwaddch(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP, 2 + strlen("Index") + last_name_cols + first_name_cols, ACS_VLINE);

						for (size_t index = 0; index < (id_cols - strlen("ID")) / 2; index++)
						{
							mvwaddch(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + last_name_cols + first_name_cols + index, ' ');
						}

						mvwprintw(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + last_name_cols + first_name_cols + (id_cols - strlen("ID")) / 2, "ID");

						for (size_t index = 0; index < (id_cols - strlen("ID")) / 2; index++)
						{
							mvwaddch(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + last_name_cols + first_name_cols + (id_cols + strlen("ID")) / 2 + index, ' ');
						}

						mvwaddch(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP, 2 + strlen("Index") + last_name_cols + first_name_cols + id_cols, ACS_VLINE);

						wattroff(window, COLOR_PAIR(WORKSPACE_LIST_GUIDE));

						portal.getStudent(index, first_name, last_name, id);

						mvwaddch(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP + 1, 1, ACS_VLINE);
						mvwprintw(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP + 1, 2, "%d", (int)index);
						mvwaddch(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP + 1, 2 + strlen("Index"), ACS_VLINE);
						mvwprintw(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP + 1, 3 + strlen("Index"), "%s", last_name.substr(0, last_name_cols).c_str());
						mvwaddch(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP + 1, 2 + strlen("Index") + last_name_cols, ACS_VLINE);
						mvwprintw(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP + 1, 3 + strlen("Index") + last_name_cols, "%s", first_name.substr(0, first_name_cols).c_str());
						mvwaddch(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP + 1, 2 + strlen("Index") + last_name_cols + first_name_cols, ACS_VLINE);
						mvwprintw(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP + 1, 3 + strlen("Index") + last_name_cols + first_name_cols, "%s", id.substr(0, id_cols).c_str());
						mvwaddch(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP + 1, 2 + strlen("Index") + last_name_cols + first_name_cols + id_cols, ACS_VLINE);
					}
					else
					{

						mvwprintw(window, 5, 1, "No student found.");
						id_cursor = 0;
					}
				}
				else
				{
					mvwprintw(window, 5, 1, "No student list.");
				}
				first_name.clear();
				last_name.clear();
				id.clear();

				id_cursor = 0;
			}
			else
			{
				mvwprintw(window, 5, 1, "Please enter a valid value.");

				current_menu = menu_t::STUDENT_ID;
			}
		}
		else if (current_menu == menu_t::CANCEL)
		{
			first_name.clear();
			last_name.clear();
			id.clear();

			id_cursor = 0;

			current_menu = menu_t::STUDENT_ID;

			title_window_is_rendered = false;
			main_window_is_rendered = false;
			footer_window_is_rendered = false;

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
			current_menu = menu_t::SEARCH;
		}
		else if (current_menu == menu_t::SEARCH)
		{
			current_menu = menu_t::CANCEL;
		}
		screen_update_flag = true;
		break;
	case KEY_BACKSPACE:
		if ((current_menu == menu_t::STUDENT_ID) && (id.length() > 0))
		{
			mvwaddch(window, 1, strlen(" Student ID: ") + 2 + id_cursor, ' ');
			id.pop_back();
			id_cursor--;
		}
		break;
	case -1:
		break;
	default:
		if ((input >= 32) && (input <= 126))
		{
			if (current_menu == menu_t::STUDENT_ID && id_cursor < MAXIMUM_STUDENT_ID_LENGTH)
			{
				id.append((char *)&input);
				mvwprintw(window, 1, strlen(" Student ID: ") + 2 + id_cursor, "%c", input);
				id_cursor++;
			}
		}

		screen_update_flag = true;
		break;
	}
}