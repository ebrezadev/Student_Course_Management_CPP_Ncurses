#include "states.hpp"
#include <ncurses.h>
#include <cstring>
#include "portal.hpp"
#include "limits_constants.hpp"

StateStudentsList::StateStudentsList()
{
	current_index = 0;
	current_list_page = 1;
	total_list_pages = 1;

	done_calculations = false;

	next_state = State_list_t::State_Students_List;
}

StateStudentsList::~StateStudentsList()
{
}

void StateStudentsList::formatHandler()
{
	getmaxyx(stdscr, max_rows, max_columns);

	if (!title_window_is_rendered)
	{
		titleWindowRender((char *)"WORKSPACE: STUDENTS LIST", TITLE_BAR_WIDTH, max_columns - MENU_COLS, 0, MENU_COLS, WORKSPACE_TITLE_BAR_COLOR_ON);
	}
	else if (!footer_window_is_rendered)
	{
		footerWindowRender((char *)"UP/DOWN: NAVIGATE | BACKSPACE: BACK TO REPORT MENU", FOOTER_BAR_WIDTH, max_columns, max_rows - FOOTER_BAR_WIDTH, 0, WORKSPACE_FOOTER_BAR_COLOR);
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

void StateStudentsList::mainWindowHandler()
{
	next_state = State_list_t::State_Students_List;

	if (!done_calculations && window != nullptr)
	{
		getmaxyx(window, max_rows, max_columns);
		list_size = portal.studentsListLength();
		page_size = max_rows - LIST_PAGE_NUMBER_OFFSET_FROM_BOTTOM - 2 - LIST_GUIDE_LINE_FROM_TOP - 1;

		if (list_size < page_size)
		{
			total_list_pages = 1;
		}
		else if (list_size % page_size == 0)
		{
			total_list_pages = list_size / page_size;
		}
		else
		{
			total_list_pages = (list_size / page_size) + 1;
		}

		done_calculations = true;

		int list_cols = max_columns - 9;
		last_name_cols = (LAST_NAME_COLS_PERCENT * list_cols) / 100;
		first_name_cols = (FIRST_NAME_COLS_PERCENT * list_cols) / 100;
		id_cols = (ID_COLS_PERCENT * list_cols) / 100;
	}

	if (screen_update_flag)
	{
		werase(window);

		if (!portal.exists())
		{
			wattron(window, A_DIM);
			mvwprintw(window, 2, 1, "%s", "No list to report: Create a new list by adding students.");
			wattroff(window, A_DIM);
		}
		else
		{
			if (list_size == 0)
			{
				wattron(window, A_DIM);
				mvwprintw(window, 2, 1, "%s", "Empty list.");
				wattroff(window, A_DIM);
			}
			else if (done_calculations)
			{
				std::string first_name, last_name, id;

				current_list_page = (current_index / page_size) + 1;

				wattron(window, COLOR_PAIR(WORKSPACE_LIST_GUIDE));

				mvwaddch(window, LIST_GUIDE_LINE_FROM_TOP, 1, ACS_VLINE);
				mvwprintw(window, LIST_GUIDE_LINE_FROM_TOP, 2, "Index");
				mvwaddch(window, LIST_GUIDE_LINE_FROM_TOP, 2 + strlen("Index"), ACS_VLINE);

				for (size_t index = 0; index < (last_name_cols - strlen("Last Name")) / 2; index++)
				{
					mvwaddch(window, LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + index, ' ');
				}

				mvwprintw(window, LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + (last_name_cols - strlen("Last Name")) / 2, "Last Name");

				for (size_t index = 0; index < (last_name_cols - strlen("Last Name")) / 2; index++)
				{
					mvwaddch(window, LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + (last_name_cols + strlen("Last Name")) / 2 + index, ' ');
				}

				mvwaddch(window, LIST_GUIDE_LINE_FROM_TOP, 2 + strlen("Index") + last_name_cols, ACS_VLINE);

				for (size_t index = 0; index < (first_name_cols - strlen("First Name")) / 2; index++)
				{
					mvwaddch(window, LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + last_name_cols + index, ' ');
				}

				mvwprintw(window, LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + last_name_cols + (first_name_cols - strlen("First Name")) / 2, "First Name");

				for (size_t index = 0; index < (first_name_cols - strlen("First Name")) / 2; index++)
				{
					mvwaddch(window, LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + last_name_cols + (first_name_cols + strlen("First Name")) / 2 + index, ' ');
				}

				mvwaddch(window, LIST_GUIDE_LINE_FROM_TOP, 2 + strlen("Index") + last_name_cols + first_name_cols, ACS_VLINE);

				for (size_t index = 0; index < (id_cols - strlen("ID")) / 2; index++)
				{
					mvwaddch(window, LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + last_name_cols + first_name_cols + index, ' ');
				}

				mvwprintw(window, LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + last_name_cols + first_name_cols + (id_cols - strlen("ID")) / 2, "ID");

				for (size_t index = 0; index < (id_cols - strlen("ID")) / 2; index++)
				{
					mvwaddch(window, LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + last_name_cols + first_name_cols + (id_cols + strlen("ID")) / 2 + index, ' ');
				}

				mvwaddch(window, LIST_GUIDE_LINE_FROM_TOP, 2 + strlen("Index") + last_name_cols + first_name_cols + id_cols, ACS_VLINE);

				wattroff(window, COLOR_PAIR(WORKSPACE_LIST_GUIDE));

				for (size_t index = 0; index < page_size; index++)
				{
					if ((current_list_page == total_list_pages) && (index > ((list_size % page_size) - 1)))
						break;

					portal.getStudent(index + (current_list_page - 1) * page_size, first_name, last_name, id);

					if (current_index == (index + (current_list_page - 1) * page_size))
						wattron(window, COLOR_PAIR(WORKSPACE_REPORT_LIST_HIGHLIGHT));

					mvwaddch(window, index + LIST_BEGIN_LINE_FROM_TOP, 1, ACS_VLINE);
					mvwprintw(window, index + LIST_BEGIN_LINE_FROM_TOP, 2, "%d", (int)(index + (current_list_page - 1) * page_size + 1));
					mvwaddch(window, index + LIST_BEGIN_LINE_FROM_TOP, 2 + strlen("Index"), ACS_VLINE);
					mvwprintw(window, index + LIST_BEGIN_LINE_FROM_TOP, 3 + strlen("Index"), "%s", last_name.substr(0, last_name_cols).c_str());
					mvwaddch(window, index + LIST_BEGIN_LINE_FROM_TOP, 2 + strlen("Index") + last_name_cols, ACS_VLINE);
					mvwprintw(window, index + LIST_BEGIN_LINE_FROM_TOP, 3 + strlen("Index") + last_name_cols, "%s", first_name.substr(0, first_name_cols).c_str());
					mvwaddch(window, index + LIST_BEGIN_LINE_FROM_TOP, 2 + strlen("Index") + last_name_cols + first_name_cols, ACS_VLINE);
					mvwprintw(window, index + LIST_BEGIN_LINE_FROM_TOP, 3 + strlen("Index") + last_name_cols + first_name_cols, "%s", id.substr(0, id_cols).c_str());
					mvwaddch(window, index + LIST_BEGIN_LINE_FROM_TOP, 2 + strlen("Index") + last_name_cols + first_name_cols + id_cols, ACS_VLINE);

					if (current_index == (index + (current_list_page - 1) * page_size))
						wattroff(window, COLOR_PAIR(WORKSPACE_REPORT_LIST_HIGHLIGHT));
				}

				getmaxyx(window, max_rows, max_columns);
				mvwprintw(window, max_rows - 1 - LIST_PAGE_NUMBER_OFFSET_FROM_BOTTOM, (max_columns - 3) / 2, "%d/%d", current_list_page, total_list_pages);
			}
		}
	}
}

void StateStudentsList::inputHandler(int input)
{
	switch (input)
	{
	case KEY_UP:
		if (portal.exists() && portal.studentsListLength() > 0)
		{
			current_index = (current_index + list_size - 1) % list_size;
			screen_update_flag = true;
		}
		break;
	case KEY_DOWN:
		if (portal.exists() && portal.studentsListLength() > 0)
		{
			current_index = (current_index + 1) % list_size;
			screen_update_flag = true;
		}
		break;
	case KEY_RIGHT:
		if (current_list_page < total_list_pages)
		{
			current_list_page++;
			current_index = (current_list_page - 1) * page_size;
		}
		break;
	case KEY_LEFT:
		if (current_list_page > 1)
		{
			current_list_page--;
			current_index = (current_list_page - 1) * page_size;
		}
		break;
	case KEY_BACKSPACE:
		current_list_page = 1;
		done_calculations = false;

		next_state = State_list_t::State_Default_Workspace;
		current_index = 0;

		update_window = title_bar_window;
		werase(title_bar_window);

		title_window_is_rendered = false;
		main_window_is_rendered = false;
		footer_window_is_rendered = false;

		Default_Workspace.destination_state = State_list_t::State_Reports_Menu;
		next_state = State_list_t::State_Default_Workspace;
		break;
	default:
		break;
	}
}