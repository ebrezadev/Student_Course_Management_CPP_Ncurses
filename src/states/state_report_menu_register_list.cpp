#include "states.hpp"
#include <ncurses.h>
#include <cstring>
#include "portal.hpp"
#include "limits_constants.hpp"

StateRegisterList::StateRegisterList()
{
	done_calculations = false;
	course_current_index = 0;
	student_current_index = 0;

	student_current_list_page = 1;
	course_current_list_page = 1;

	course_selected = false;

	next_state = State_list_t::State_Register_List;
}

StateRegisterList::~StateRegisterList()
{
}

void StateRegisterList::formatHandler()
{
	getmaxyx(stdscr, max_rows, max_columns);

	if (!title_window_is_rendered)
	{
		titleWindowRender((char *)"WORKSPACE: REGISTER LIST", TITLE_BAR_WIDTH, max_columns - MENU_COLS, 0, MENU_COLS, WORKSPACE_TITLE_BAR_COLOR_ON);
	}
	else if (!footer_window_is_rendered)
	{
		footerWindowRender((char *)"UP/DOWN: NAVIGATE | LEFT/RIGHT: SELECT PAGE | BACKSPACE: REPORT MENU", FOOTER_BAR_WIDTH, max_columns, max_rows - FOOTER_BAR_WIDTH, 0, WORKSPACE_FOOTER_BAR_COLOR);
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

void StateRegisterList::mainWindowHandler()
{
	next_state = State_list_t::State_Register_List;

	if (!done_calculations && window != nullptr)
	{
		getmaxyx(window, max_rows, max_columns);
		course_list_size = portal.coursesListLength();
		page_size = max_rows - GRADES_REPORT_LIST_PAGE_NUMBER_OFFSET_FROM_BOTTOM - 2 - GRADES_REPORT_LIST_GUIDE_LINE_FROM_TOP - 1;

		if (course_list_size < page_size)
		{
			course_total_list_pages = 1;
		}
		else if (course_list_size % page_size == 0)
		{
			course_total_list_pages = course_list_size / page_size;
		}
		else
		{
			course_total_list_pages = (course_list_size / page_size) + 1;
		}

		int course_list_cols = max_columns - 25;
		course_name_cols = (COURSE_NAME_COLS_PERCENT * course_list_cols) / 100;
		course_plan_cols = (COURSE_PLAN_COLS_PERCENT * course_list_cols) / 100;
		course_id_cols = (COURSE_ID_COLS_PERCENT * course_list_cols) / 100;

		int student_list_cols = max_columns - 8;
		student_last_name_cols = (LAST_NAME_COLS_PERCENT * student_list_cols) / 100;
		student_first_name_cols = (FIRST_NAME_COLS_PERCENT * student_list_cols) / 100;
		student_id_cols = (ID_COLS_PERCENT * student_list_cols) / 100;

		done_calculations = true;
	}

	if (screen_update_flag)
	{
		werase(window);

		if (!portal.exists())
		{
			wattron(window, A_DIM);
			mvwprintw(window, 2, 1, "%s", "No list to report: Create a new list by adding courses.");
			wattroff(window, A_DIM);
		}
		else
		{
			if (course_list_size == 0)
			{
				wattron(window, A_DIM);
				mvwprintw(window, 2, 1, "%s", "Empty list.");
				wattroff(window, A_DIM);
			}
			else if (done_calculations && !course_selected)
			{
				mvwprintw(window, GRADES_REPORT_LIST_GUIDE_LINE_FROM_TOP - 2, 1, "Please select a course:");

				std::string course_name, course_plan, id;
				int course_unit;
				Link::classification_t course_classification;

				course_current_list_page = (course_current_index / page_size) + 1;

				wattron(window, COLOR_PAIR(WORKSPACE_LIST_GUIDE));

				mvwaddch(window, GRADES_REPORT_LIST_GUIDE_LINE_FROM_TOP, 1, ACS_VLINE);
				mvwprintw(window, GRADES_REPORT_LIST_GUIDE_LINE_FROM_TOP, 2, "Index");
				mvwaddch(window, GRADES_REPORT_LIST_GUIDE_LINE_FROM_TOP, 2 + strlen("Index"), ACS_VLINE);
				mvwprintw(window, GRADES_REPORT_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index"), "Units");
				mvwaddch(window, GRADES_REPORT_LIST_GUIDE_LINE_FROM_TOP, 2 + strlen("Index") + strlen("Units"), ACS_VLINE);
				for (size_t index = 0; index < (strlen("Theoretical ") - strlen("Type")) / 2; index++)
				{
					mvwaddch(window, GRADES_REPORT_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units") + index, ' ');
				}

				mvwprintw(window, GRADES_REPORT_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units") + (strlen("Theoretical ") - strlen("Type")) / 2, "Type");

				for (size_t index = 0; index < (strlen("Theoretical ") - strlen("Type")) / 2; index++)
				{
					mvwaddch(window, GRADES_REPORT_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units") + (strlen("Theoretical ") + strlen("Type")) / 2 + index, ' ');
				}

				mvwaddch(window, GRADES_REPORT_LIST_GUIDE_LINE_FROM_TOP, 2 + strlen("Index") + strlen("Units") + strlen("Theoretical "), ACS_VLINE);

				for (size_t index = 0; index < (course_name_cols - strlen("Name")) / 2; index++)
				{
					mvwaddch(window, GRADES_REPORT_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + index, ' ');
				}

				mvwprintw(window, GRADES_REPORT_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + (course_name_cols - strlen("Name")) / 2, "Name");

				for (size_t index = 0; index < (course_name_cols - strlen("Name")) / 2; index++)
				{
					mvwaddch(window, GRADES_REPORT_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + (course_name_cols + strlen("Name")) / 2 + index, ' ');
				}

				mvwaddch(window, GRADES_REPORT_LIST_GUIDE_LINE_FROM_TOP, 2 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + course_name_cols, ACS_VLINE);

				for (size_t index = 0; index < (course_id_cols - strlen("ID")) / 2; index++)
				{
					mvwaddch(window, GRADES_REPORT_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + course_name_cols + index, ' ');
				}

				mvwprintw(window, GRADES_REPORT_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + course_name_cols + (course_id_cols - strlen("ID")) / 2, "ID");

				for (size_t index = 0; index < (course_id_cols - strlen("ID")) / 2; index++)
				{
					mvwaddch(window, GRADES_REPORT_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + course_name_cols + (course_id_cols + strlen("ID")) / 2 + index, ' ');
				}

				mvwaddch(window, GRADES_REPORT_LIST_GUIDE_LINE_FROM_TOP, 2 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + course_name_cols + course_id_cols, ACS_VLINE);

				for (size_t index = 0; index < (course_plan_cols - strlen("Plan")) / 2; index++)
				{
					mvwaddch(window, GRADES_REPORT_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + course_name_cols + course_id_cols + index, ' ');
				}

				mvwprintw(window, GRADES_REPORT_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + course_name_cols + course_id_cols + (course_plan_cols - strlen("Plan")) / 2, "Plan");

				for (size_t index = 0; index < (course_plan_cols - strlen("Plan")) / 2; index++)
				{
					mvwaddch(window, GRADES_REPORT_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + course_name_cols + course_id_cols + (course_plan_cols + strlen("Plan")) / 2 + index, ' ');
				}

				mvwaddch(window, GRADES_REPORT_LIST_GUIDE_LINE_FROM_TOP, 2 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + course_name_cols + course_id_cols + course_plan_cols, ACS_VLINE);

				wattroff(window, COLOR_PAIR(WORKSPACE_LIST_GUIDE));

				for (size_t index = 0; index < page_size; index++)
				{
					if ((course_current_list_page == course_total_list_pages) && (index > ((course_list_size % page_size) - 1)))
						break;

					portal.getCourse(index + (course_current_list_page - 1) * page_size, course_classification, course_name, id, course_plan, course_unit);

					mvwaddch(window, index + GRADES_REPORT_LIST_BEGIN_LINE_FROM_TOP, 1, ACS_VLINE);

					if (course_current_index == (index + (course_current_list_page - 1) * page_size))
						wattron(window, COLOR_PAIR(WORKSPACE_REPORT_LIST_HIGHLIGHT));

					mvwprintw(window, index + GRADES_REPORT_LIST_BEGIN_LINE_FROM_TOP, 2, "%d", (int)(index + (course_current_list_page - 1) * page_size + 1));

					mvwaddch(window, index + GRADES_REPORT_LIST_BEGIN_LINE_FROM_TOP, 2 + strlen("Index"), ACS_VLINE);

					mvwprintw(window, index + GRADES_REPORT_LIST_BEGIN_LINE_FROM_TOP, 3 + strlen("Index"), "%d", course_unit);

					mvwaddch(window, index + GRADES_REPORT_LIST_BEGIN_LINE_FROM_TOP, 2 + strlen("Index") + strlen("Units"), ACS_VLINE);

					if (course_classification == Link::classification_t::PRACTICAL_COURSE)
					{
						mvwprintw(window, index + GRADES_REPORT_LIST_BEGIN_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units"), "Practical");
					}
					else if (course_classification == Link::classification_t::THEORETICAL_COURSE)
					{
						mvwprintw(window, index + GRADES_REPORT_LIST_BEGIN_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units"), "Theoretical ");
					}

					mvwaddch(window, index + GRADES_REPORT_LIST_BEGIN_LINE_FROM_TOP, 2 + strlen("Index") + strlen("Units") + strlen("Theoretical "), ACS_VLINE);

					mvwprintw(window, index + GRADES_REPORT_LIST_BEGIN_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units") + strlen("Theoretical "), "%s", course_name.substr(0, course_name_cols).c_str());

					mvwaddch(window, index + GRADES_REPORT_LIST_BEGIN_LINE_FROM_TOP, 2 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + course_name_cols, ACS_VLINE);

					mvwprintw(window, index + GRADES_REPORT_LIST_BEGIN_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + course_name_cols, "%s", id.substr(0, course_id_cols).c_str());

					mvwaddch(window, index + GRADES_REPORT_LIST_BEGIN_LINE_FROM_TOP, 2 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + course_name_cols + course_id_cols, ACS_VLINE);

					mvwprintw(window, index + GRADES_REPORT_LIST_BEGIN_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + course_name_cols + course_id_cols, "%s", course_plan.substr(0, course_plan_cols).c_str());

					mvwaddch(window, index + GRADES_REPORT_LIST_BEGIN_LINE_FROM_TOP, 2 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + course_name_cols + course_id_cols + course_plan_cols, ACS_VLINE);

					if (course_current_index == (index + (course_current_list_page - 1) * page_size))
						wattroff(window, COLOR_PAIR(WORKSPACE_REPORT_LIST_HIGHLIGHT));
				}

				getmaxyx(window, max_rows, max_columns);
				mvwprintw(window, max_rows - 1 - GRADES_REPORT_LIST_PAGE_NUMBER_OFFSET_FROM_BOTTOM, (max_columns - 3) / 2, "%d/%d", course_current_list_page, course_total_list_pages);
			}
			else if (done_calculations && course_selected && portal.courseRegisterListLength(course_current_index) != 0)
			{
				mvwprintw(window, GRADES_REPORT_LIST_GUIDE_LINE_FROM_TOP - 2, 1, "Grade list for the selected course:");

				wattron(window, COLOR_PAIR(WORKSPACE_LIST_GUIDE));

				mvwaddch(window, GRADES_REPORT_LIST_GUIDE_LINE_FROM_TOP, 1, ACS_VLINE);
				mvwprintw(window, GRADES_REPORT_LIST_GUIDE_LINE_FROM_TOP, 2, "Index");
				mvwaddch(window, GRADES_REPORT_LIST_GUIDE_LINE_FROM_TOP, 2 + strlen("Index"), ACS_VLINE);

				for (size_t index = 0; index < (student_last_name_cols - strlen("Last Name")) / 2; index++)
				{
					mvwaddch(window, GRADES_REPORT_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + index, ' ');
				}

				mvwprintw(window, GRADES_REPORT_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + (student_last_name_cols - strlen("Last Name")) / 2, "Last Name");

				for (size_t index = 0; index < (student_last_name_cols - strlen("Last Name")) / 2; index++)
				{
					mvwaddch(window, GRADES_REPORT_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + (student_last_name_cols + strlen("Last Name")) / 2 + index, ' ');
				}

				mvwaddch(window, GRADES_REPORT_LIST_GUIDE_LINE_FROM_TOP, 2 + strlen("Index") + student_last_name_cols, ACS_VLINE);

				for (size_t index = 0; index < ((size_t)student_first_name_cols - strlen("First Name")) / 2; index++)
				{
					mvwaddch(window, GRADES_REPORT_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + student_last_name_cols + index, ' ');
				}

				mvwprintw(window, GRADES_REPORT_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + student_last_name_cols + (student_first_name_cols - strlen("First Name")) / 2, "First Name");

				for (size_t index = 0; index < ((size_t)student_first_name_cols - strlen("First Name")) / 2; index++)
				{
					mvwaddch(window, GRADES_REPORT_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + student_last_name_cols + (student_first_name_cols + strlen("First Name")) / 2 + index, ' ');
				}

				mvwaddch(window, GRADES_REPORT_LIST_GUIDE_LINE_FROM_TOP, 2 + strlen("Index") + student_last_name_cols + student_first_name_cols, ACS_VLINE);

				for (size_t index = 0; index < ((size_t)student_id_cols - strlen("ID")) / 2; index++)
				{
					mvwaddch(window, GRADES_REPORT_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + student_last_name_cols + student_first_name_cols + index, ' ');
				}

				mvwprintw(window, GRADES_REPORT_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + student_last_name_cols + student_first_name_cols + (student_id_cols - strlen("ID")) / 2, "ID");

				for (size_t index = 0; index < ((size_t)student_id_cols - strlen("ID")) / 2; index++)
				{
					mvwaddch(window, GRADES_REPORT_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + student_last_name_cols + student_first_name_cols + (student_id_cols + strlen("ID")) / 2 + index, ' ');
				}

				mvwaddch(window, GRADES_REPORT_LIST_GUIDE_LINE_FROM_TOP, 2 + strlen("Index") + student_last_name_cols + student_first_name_cols + student_id_cols, ACS_VLINE);

				wattroff(window, COLOR_PAIR(WORKSPACE_LIST_GUIDE));

				std::string student_first_name, student_last_name, student_id;

				student_list_size = portal.courseRegisterListLength(course_current_index);

				if (student_list_size < page_size)
				{
					student_total_list_pages = 1;
				}
				else if (student_list_size % page_size == 0)
				{
					student_total_list_pages = student_list_size / page_size;
				}
				else
				{
					student_total_list_pages = (student_list_size / page_size) + 1;
				}

				student_current_list_page = (student_current_index / page_size) + 1;

				for (size_t index = 0; index < page_size; index++)
				{
					if ((student_current_list_page == student_total_list_pages) && (index > ((student_list_size % page_size) - 1)))
						break;

					float grade = 0;

					portal.getCourseRegisterList(course_current_index, index, student_last_name, student_first_name, student_id);
					portal.getGrade(course_current_index, index, grade);

					if (student_current_index == (index + (student_current_list_page - 1) * page_size))
						wattron(window, COLOR_PAIR(WORKSPACE_REPORT_LIST_HIGHLIGHT));

					mvwaddch(window, index + GRADES_REPORT_LIST_BEGIN_LINE_FROM_TOP, 1, ACS_VLINE);
					mvwprintw(window, index + GRADES_REPORT_LIST_BEGIN_LINE_FROM_TOP, 2, "%d", (int)(index + (student_current_list_page - 1) * page_size + 1));
					mvwaddch(window, index + GRADES_REPORT_LIST_BEGIN_LINE_FROM_TOP, 2 + strlen("Index"), ACS_VLINE);
					mvwprintw(window, index + GRADES_REPORT_LIST_BEGIN_LINE_FROM_TOP, 3 + strlen("Index"), "%s", student_last_name.substr(0, student_last_name_cols).c_str());
					mvwaddch(window, index + GRADES_REPORT_LIST_BEGIN_LINE_FROM_TOP, 2 + strlen("Index") + student_last_name_cols, ACS_VLINE);
					mvwprintw(window, index + GRADES_REPORT_LIST_BEGIN_LINE_FROM_TOP, 3 + strlen("Index") + student_last_name_cols, "%s", student_first_name.substr(0, student_first_name_cols).c_str());
					mvwaddch(window, index + GRADES_REPORT_LIST_BEGIN_LINE_FROM_TOP, 2 + strlen("Index") + student_last_name_cols + student_first_name_cols, ACS_VLINE);
					mvwprintw(window, index + GRADES_REPORT_LIST_BEGIN_LINE_FROM_TOP, 3 + strlen("Index") + student_last_name_cols + student_first_name_cols, "%s", student_id.substr(0, student_id_cols).c_str());
					mvwaddch(window, index + GRADES_REPORT_LIST_BEGIN_LINE_FROM_TOP, 2 + strlen("Index") + student_last_name_cols + student_first_name_cols + student_id_cols, ACS_VLINE);

					if (student_current_index == (index + (student_current_list_page - 1) * page_size))
						wattroff(window, COLOR_PAIR(WORKSPACE_REPORT_LIST_HIGHLIGHT));
				}

				getmaxyx(window, max_rows, max_columns);
				mvwprintw(window, max_rows - 1 - GRADES_REPORT_LIST_PAGE_NUMBER_OFFSET_FROM_BOTTOM, (max_columns - 3) / 2, "%d/%d", student_current_list_page, student_total_list_pages);
			}
			else if (done_calculations && course_selected && portal.courseRegisterListLength(course_current_index) == 0)
			{
				mvwprintw(window, GRADES_REPORT_LIST_GUIDE_LINE_FROM_TOP - 2, 1, "No registered students.");
			}
		}
	}
}

void StateRegisterList::inputHandler(int input)
{
	switch (input)
	{
	case KEY_UP:
		if (!course_selected)
		{
			if (portal.exists() && portal.coursesListLength() > 0)
			{
				course_current_index = (course_current_index + course_list_size - 1) % course_list_size;
				screen_update_flag = true;
			}
		}
		else
		{
			if (portal.exists() && portal.courseRegisterListLength(course_current_index) > 0)
			{
				student_current_index = (student_current_index + portal.courseRegisterListLength(course_current_index) - 1) % portal.courseRegisterListLength(course_current_index);
				screen_update_flag = true;
			}
		}
		break;
	case KEY_DOWN:
		if (!course_selected)
		{
			if (portal.exists() && portal.coursesListLength() > 0)
			{
				course_current_index = (course_current_index + 1) % course_list_size;
				screen_update_flag = true;
			}
		}
		else
		{
			if (portal.exists() && portal.courseRegisterListLength(course_current_index) > 0)
			{
				student_current_index = (student_current_index + 1) % portal.courseRegisterListLength(course_current_index);
				screen_update_flag = true;
			}
		}
		break;
	case KEY_BACKSPACE:
		if (!course_selected)
		{
			werase(window);
			course_current_index = 0;

			update_window = title_bar_window;
			werase(title_bar_window);
			title_window_is_rendered = false;
			main_window_is_rendered = false;
			footer_window_is_rendered = false;

			Default_Workspace.destination_state = State_list_t::State_Reports_Menu;
			next_state = State_list_t::State_Default_Workspace;

			course_selected = false;
			done_calculations = false;
		}
		else
		{
			werase(window);
			course_selected = false;
			student_current_index = 0;
		}
		break;
	case (int)13:
		if (!course_selected)
		{
			if (course_list_size != 0)
			{
				werase(window);
				course_selected = true;
			}
		}
		else
		{
		}
		break;
	default:
		break;
	}
}