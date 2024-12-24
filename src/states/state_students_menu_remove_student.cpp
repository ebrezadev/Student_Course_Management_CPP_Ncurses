#include "states.hpp"
#include <ncurses.h>
#include <cstring>
#include "portal.hpp"
#include "limits_constants.hpp"

StateRemoveStudent::StateRemoveStudent()
{
	remove = false;
	current_index = 0;
	next_state = State_list_t::State_Remove_Student;
}

StateRemoveStudent::~StateRemoveStudent()
{
}

void StateRemoveStudent::formatHandler()
{
	getmaxyx(stdscr, max_rows, max_columns);

	if (!title_window_is_rendered)
	{
		titleWindowRender((char *)"WORKSPACE: REMOVE STUDENT", TITLE_BAR_WIDTH, max_columns - MENU_COLS, 0, MENU_COLS, WORKSPACE_TITLE_BAR_COLOR_ON);
	}
	else if (!footer_window_is_rendered)
	{
		footerWindowRender((char *)"UP/DOWN: NAVIGATE | ENTER: DELETE | BACKSPACE: STUDENTS MENU", FOOTER_BAR_WIDTH, max_columns, max_rows - FOOTER_BAR_WIDTH, 0, WORKSPACE_FOOTER_BAR_COLOR);
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

void StateRemoveStudent::mainWindowHandler()
{
	next_state = State_list_t::State_Remove_Student;

	// remove student from students list, also from all courses register lists
	if (remove)
	{
		for (size_t course_index = 0; course_index < portal.coursesListLength(); course_index++)
		{
			for (size_t registered_student_index = 0; registered_student_index < portal.courseRegisterListLength(course_index); registered_student_index++)
			{
				portal.unregisterCourse(current_index, course_index);
			}
		}

		portal.removeStudent(current_index);

		werase(window);
		current_index = 0;
		done_calculations = false;
		remove = false;
	}

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

		int list_cols = max_columns - 9;
		last_name_cols = (LAST_NAME_COLS_PERCENT * list_cols) / 100;
		first_name_cols = (FIRST_NAME_COLS_PERCENT * list_cols) / 100;
		id_cols = (ID_COLS_PERCENT * list_cols) / 100;

		done_calculations = true;
	}

	if (screen_update_flag)
	{
		werase(window);

		if (!portal.exists())
		{
			wattron(window, A_DIM);
			mvwprintw(window, 2, 1, "%s", "Empty list.");
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
						wattron(window, COLOR_PAIR(WORKSPACE_REMOVE_LIST_HIGHLIGHT));

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
						wattroff(window, COLOR_PAIR(WORKSPACE_REMOVE_LIST_HIGHLIGHT));
				}

				getmaxyx(window, max_rows, max_columns);
				mvwprintw(window, max_rows - 1 - LIST_PAGE_NUMBER_OFFSET_FROM_BOTTOM, (max_columns - 3) / 2, "%d/%d", current_list_page, total_list_pages);
			}
		}
	}
}

void StateRemoveStudent::inputHandler(int input)
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

		Default_Workspace.destination_state = State_list_t::State_Students_Menu;
		next_state = State_list_t::State_Default_Workspace;
		break;
	case (int)13:
		if (list_size != 0)
		{
			wbkgd(title_bar_window, WORKSPACE_TITLE_BAR_COLOR_OFF);
			update_window = title_bar_window;
			title_window_is_rendered = false;
			main_window_is_rendered = false;
			footer_window_is_rendered = false;
			next_state = State_list_t::State_Notification_Remove_Student_Popup;
		}
		break;
	default:
		break;
	}
}