#include "states.hpp"
#include <ncurses.h>
#include <cstring>
#include "portal.hpp"
#include "limits_constants.hpp"
#include <vector>
#include "error.hpp"

inline constexpr int NUMBER_OF_MENU_OPTIONS = 3;
std::vector<size_t> registered_courses_vector;

StateGradeEntry::StateGradeEntry()
{
	student_current_index = 0;
	student_current_list_page = 1;
	student_total_list_pages = 1;

	course_current_index = 0;
	course_current_list_page = 1;
	course_total_list_pages = 1;

	done_calculations = false;
	student_selected = false;
	grade_entry_page = false;

	grade_cursor = 0;

	next_state = State_list_t::State_Grade_Entry;
}

StateGradeEntry::~StateGradeEntry()
{
}

void StateGradeEntry::formatHandler()
{
	getmaxyx(stdscr, max_rows, max_columns);

	if (!title_window_is_rendered)
	{
		titleWindowRender((char *)"WORKSPACE: GRADE ENTRY", TITLE_BAR_WIDTH, max_columns - MENU_COLS, 0, MENU_COLS, WORKSPACE_TITLE_BAR_COLOR_ON);
	}
	else if (!footer_window_is_rendered)
	{
		footerWindowRender((char *)"UP/DOWN: NAVIGATE | BACKSPACE: REGISTER MENU", FOOTER_BAR_WIDTH, max_columns, max_rows - FOOTER_BAR_WIDTH, 0, WORKSPACE_FOOTER_BAR_COLOR);
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

void StateGradeEntry::mainWindowHandler()
{
	next_state = State_list_t::State_Grade_Entry;

	if (!done_calculations && window != nullptr)
	{
		getmaxyx(window, max_rows, max_columns);
		student_list_size = portal.studentsListLength();

		page_size = max_rows - REGISTER_LIST_PAGE_NUMBER_OFFSET_FROM_BOTTOM - 2 - REGISTER_LIST_GUIDE_LINE_FROM_TOP - 1;

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

		int student_list_cols = max_columns - 10;
		student_last_name_cols = (LAST_NAME_COLS_PERCENT * student_list_cols) / 100;
		student_first_name_cols = (FIRST_NAME_COLS_PERCENT * student_list_cols) / 100;
		student_id_cols = (ID_COLS_PERCENT * student_list_cols) / 100;

		int course_list_cols = max_columns - 32;
		course_name_cols = (COURSE_NAME_COLS_PERCENT * course_list_cols) / 100;
		course_plan_cols = (COURSE_PLAN_COLS_PERCENT * course_list_cols) / 100;
		course_id_cols = (COURSE_ID_COLS_PERCENT * course_list_cols) / 100;

		done_calculations = true;
	}

	if (screen_update_flag)
	{
		if (!portal.exists())
		{
			werase(window);

			wattron(window, A_DIM);
			mvwprintw(window, 2, 1, "%s", "No list to report: Create a new list by adding students or courses.");
			wattroff(window, A_DIM);
		}
		else
		{
			if (!student_selected && !grade_entry_page)
			{
				werase(window);

				if (student_list_size == 0)
				{
					wattron(window, A_DIM);
					mvwprintw(window, 2, 1, "%s", "Empty list.");
					wattroff(window, A_DIM);
				}
				else if (done_calculations)
				{
					mvwprintw(window, REGISTER_LIST_GUIDE_LINE_FROM_TOP - 2, 1, "Please select a student:");

					std::string first_name, last_name, id;

					student_current_list_page = (student_current_index / page_size) + 1;

					wattron(window, COLOR_PAIR(WORKSPACE_LIST_GUIDE));

					mvwaddch(window, REGISTER_LIST_GUIDE_LINE_FROM_TOP, 1, ACS_VLINE);
					mvwprintw(window, REGISTER_LIST_GUIDE_LINE_FROM_TOP, 2, "Index");
					mvwaddch(window, REGISTER_LIST_GUIDE_LINE_FROM_TOP, 2 + strlen("Index"), ACS_VLINE);

					for (size_t index = 0; index < (student_last_name_cols - strlen("Last Name")) / 2; index++)
					{
						mvwaddch(window, REGISTER_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + index, ' ');
					}

					mvwprintw(window, REGISTER_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + (student_last_name_cols - strlen("Last Name")) / 2, "Last Name");

					for (size_t index = 0; index < (student_last_name_cols - strlen("Last Name")) / 2; index++)
					{
						mvwaddch(window, REGISTER_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + (student_last_name_cols + strlen("Last Name")) / 2 + index, ' ');
					}

					mvwaddch(window, REGISTER_LIST_GUIDE_LINE_FROM_TOP, 2 + strlen("Index") + student_last_name_cols, ACS_VLINE);

					for (size_t index = 0; index < (student_first_name_cols - strlen("First Name")) / 2; index++)
					{
						mvwaddch(window, REGISTER_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + student_last_name_cols + index, ' ');
					}

					mvwprintw(window, REGISTER_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + student_last_name_cols + (student_first_name_cols - strlen("First Name")) / 2, "First Name");

					for (size_t index = 0; index < (student_first_name_cols - strlen("First Name")) / 2; index++)
					{
						mvwaddch(window, REGISTER_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + student_last_name_cols + (student_first_name_cols + strlen("First Name")) / 2 + index, ' ');
					}

					mvwaddch(window, REGISTER_LIST_GUIDE_LINE_FROM_TOP, 2 + strlen("Index") + student_last_name_cols + student_first_name_cols, ACS_VLINE);

					for (size_t index = 0; index < (student_id_cols - strlen("ID")) / 2; index++)
					{
						mvwaddch(window, REGISTER_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + student_last_name_cols + student_first_name_cols + index, ' ');
					}

					mvwprintw(window, REGISTER_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + student_last_name_cols + student_first_name_cols + (student_id_cols - strlen("ID")) / 2, "ID");

					for (size_t index = 0; index < (student_id_cols - strlen("ID")) / 2; index++)
					{
						mvwaddch(window, REGISTER_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + student_last_name_cols + student_first_name_cols + (student_id_cols + strlen("ID")) / 2 + index, ' ');
					}

					mvwaddch(window, REGISTER_LIST_GUIDE_LINE_FROM_TOP, 2 + strlen("Index") + student_last_name_cols + student_first_name_cols + student_id_cols, ACS_VLINE);

					wattroff(window, COLOR_PAIR(WORKSPACE_LIST_GUIDE));

					for (size_t index = 0; index < page_size; index++)
					{
						if ((student_current_list_page == student_total_list_pages) && (index > ((student_list_size % page_size) - 1)))
							break;

						portal.getStudent(index + (student_current_list_page - 1) * page_size, first_name, last_name, id);

						if (student_current_index == (index + (student_current_list_page - 1) * page_size))
							wattron(window, COLOR_PAIR(WORKSPACE_REPORT_LIST_HIGHLIGHT));

						mvwaddch(window, index + REGISTER_LIST_BEGIN_LINE_FROM_TOP, 1, ACS_VLINE);
						mvwprintw(window, index + REGISTER_LIST_BEGIN_LINE_FROM_TOP, 2, "%d", (int)(index + (student_current_list_page - 1) * page_size + 1));
						mvwaddch(window, index + REGISTER_LIST_BEGIN_LINE_FROM_TOP, 2 + strlen("Index"), ACS_VLINE);
						mvwprintw(window, index + REGISTER_LIST_BEGIN_LINE_FROM_TOP, 3 + strlen("Index"), "%s", last_name.substr(0, student_last_name_cols).c_str());
						mvwaddch(window, index + REGISTER_LIST_BEGIN_LINE_FROM_TOP, 2 + strlen("Index") + student_last_name_cols, ACS_VLINE);
						mvwprintw(window, index + REGISTER_LIST_BEGIN_LINE_FROM_TOP, 3 + strlen("Index") + student_last_name_cols, "%s", first_name.substr(0, student_first_name_cols).c_str());
						mvwaddch(window, index + REGISTER_LIST_BEGIN_LINE_FROM_TOP, 2 + strlen("Index") + student_last_name_cols + student_first_name_cols, ACS_VLINE);
						mvwprintw(window, index + REGISTER_LIST_BEGIN_LINE_FROM_TOP, 3 + strlen("Index") + student_last_name_cols + student_first_name_cols, "%s", id.substr(0, student_id_cols).c_str());
						mvwaddch(window, index + REGISTER_LIST_BEGIN_LINE_FROM_TOP, 2 + strlen("Index") + student_last_name_cols + student_first_name_cols + student_id_cols, ACS_VLINE);

						if (student_current_index == (index + (student_current_list_page - 1) * page_size))
							wattroff(window, COLOR_PAIR(WORKSPACE_REPORT_LIST_HIGHLIGHT));
					}

					getmaxyx(window, max_rows, max_columns);
					mvwprintw(window, max_rows - 1 - REGISTER_LIST_PAGE_NUMBER_OFFSET_FROM_BOTTOM, (max_columns - 3) / 2, "%d/%d", student_current_list_page, student_total_list_pages);
				}
			}
			else if (student_selected && !grade_entry_page)
			{
				werase(window);

				registered_courses_vector.clear();

				std::string course_name, course_plan, course_id;
				int course_unit;
				Link::classification_t course_classification;

				std::string student_first_name, student_last_name, student_id;
				std::string registered_student_first_name, registered_student_last_name, registered_student_id;

				bool has_not_any_registers = true;

				portal.getStudent(student_current_index, student_first_name, student_last_name, student_id);

				// iterate over courses and search for the student -> end up with a vector of courses that the student has registered
				for (size_t course_index = 0; course_index < portal.coursesListLength(); course_index++)
				{
					for (size_t registered_student_index = 0; registered_student_index < portal.courseRegisterListLength(course_index); registered_student_index++)
					{
						portal.getCourseRegisterList(course_index, registered_student_index, registered_student_last_name, registered_student_first_name, registered_student_id);

						if (student_first_name == registered_student_first_name && student_last_name == registered_student_last_name && student_id == registered_student_id)
						{
							registered_courses_vector.push_back(course_index);
							has_not_any_registers = false;
							break;
						}
					}
				}

				if (has_not_any_registers)
				{
					mvwprintw(window, REGISTER_LIST_GUIDE_LINE_FROM_TOP - 2, 1, "Selected student has not registered to any courses.");
				}
				else
				{
					mvwprintw(window, REGISTER_LIST_GUIDE_LINE_FROM_TOP - 2, 1, "Student selected. Please select a course to enter grade:");

					// print the list of registered courses for the student
					course_list_size = registered_courses_vector.size();

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

					course_current_list_page = (course_current_index / page_size) + 1;

					wattron(window, COLOR_PAIR(WORKSPACE_LIST_GUIDE));

					mvwaddch(window, REGISTER_LIST_GUIDE_LINE_FROM_TOP, 1, ACS_VLINE);
					mvwprintw(window, REGISTER_LIST_GUIDE_LINE_FROM_TOP, 2, "Index");
					mvwaddch(window, REGISTER_LIST_GUIDE_LINE_FROM_TOP, 2 + strlen("Index"), ACS_VLINE);
					mvwprintw(window, REGISTER_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index"), "Units");
					mvwaddch(window, REGISTER_LIST_GUIDE_LINE_FROM_TOP, 2 + strlen("Index") + strlen("Units"), ACS_VLINE);
					for (size_t index = 0; index < (strlen("Theoretical ") - strlen("Type")) / 2; index++)
					{
						mvwaddch(window, REGISTER_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units") + index, ' ');
					}

					mvwprintw(window, REGISTER_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units") + (strlen("Theoretical ") - strlen("Type")) / 2, "Type");

					for (size_t index = 0; index < (strlen("Theoretical ") - strlen("Type")) / 2; index++)
					{
						mvwaddch(window, REGISTER_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units") + (strlen("Theoretical ") + strlen("Type")) / 2 + index, ' ');
					}

					mvwaddch(window, REGISTER_LIST_GUIDE_LINE_FROM_TOP, 2 + strlen("Index") + strlen("Units") + strlen("Theoretical "), ACS_VLINE);

					for (size_t index = 0; index < (course_name_cols - strlen("Name")) / 2; index++)
					{
						mvwaddch(window, REGISTER_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + index, ' ');
					}

					mvwprintw(window, REGISTER_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + (course_name_cols - strlen("Name")) / 2, "Name");

					for (size_t index = 0; index < (course_name_cols - strlen("Name")) / 2; index++)
					{
						mvwaddch(window, REGISTER_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + (course_name_cols + strlen("Name")) / 2 + index, ' ');
					}

					mvwaddch(window, REGISTER_LIST_GUIDE_LINE_FROM_TOP, 2 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + course_name_cols, ACS_VLINE);

					for (size_t index = 0; index < (course_id_cols - strlen("ID")) / 2; index++)
					{
						mvwaddch(window, REGISTER_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + course_name_cols + index, ' ');
					}

					mvwprintw(window, REGISTER_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + course_name_cols + (course_id_cols - strlen("ID")) / 2, "ID");

					for (size_t index = 0; index < (course_id_cols - strlen("ID")) / 2; index++)
					{
						mvwaddch(window, REGISTER_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + course_name_cols + (course_id_cols + strlen("ID")) / 2 + index, ' ');
					}

					mvwaddch(window, REGISTER_LIST_GUIDE_LINE_FROM_TOP, 2 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + course_name_cols + course_id_cols, ACS_VLINE);

					for (size_t index = 0; index < (course_plan_cols - strlen("Plan")) / 2; index++)
					{
						mvwaddch(window, REGISTER_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + course_name_cols + course_id_cols + index, ' ');
					}

					mvwprintw(window, REGISTER_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + course_name_cols + course_id_cols + (course_plan_cols - strlen("Plan")) / 2, "Plan");

					for (size_t index = 0; index < (course_plan_cols - strlen("Plan")) / 2; index++)
					{
						mvwaddch(window, REGISTER_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + course_name_cols + course_id_cols + (course_plan_cols + strlen("Plan")) / 2 + index, ' ');
					}

					mvwaddch(window, REGISTER_LIST_GUIDE_LINE_FROM_TOP, 2 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + course_name_cols + course_id_cols + course_plan_cols, ACS_VLINE);

					mvwprintw(window, REGISTER_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + course_name_cols + course_id_cols + course_plan_cols, "Grade");

					mvwaddch(window, REGISTER_LIST_GUIDE_LINE_FROM_TOP, 2 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + course_name_cols + course_id_cols + course_plan_cols + strlen("Grade "), ACS_VLINE);

					wattroff(window, COLOR_PAIR(WORKSPACE_LIST_GUIDE));

					for (size_t index = 0; index < page_size; index++)
					{
						if ((course_current_list_page == course_total_list_pages) && (index > ((course_list_size % page_size) - 1)))
							break;

						portal.getCourse(registered_courses_vector[index + (course_current_list_page - 1) * page_size], course_classification, course_name, course_id, course_plan, course_unit);

						mvwaddch(window, index + REGISTER_LIST_BEGIN_LINE_FROM_TOP, 1, ACS_VLINE);

						if (course_current_index == (index + (course_current_list_page - 1) * page_size))
							wattron(window, COLOR_PAIR(WORKSPACE_REPORT_LIST_HIGHLIGHT));

						mvwprintw(window, index + REGISTER_LIST_BEGIN_LINE_FROM_TOP, 2, "%d", (int)(index + (course_current_list_page - 1) * page_size + 1));

						if (course_current_index == (index + (course_current_list_page - 1) * page_size))
							wattroff(window, COLOR_PAIR(WORKSPACE_REPORT_LIST_HIGHLIGHT));

						mvwaddch(window, index + REGISTER_LIST_BEGIN_LINE_FROM_TOP, 2 + strlen("Index"), ACS_VLINE);

						mvwprintw(window, index + REGISTER_LIST_BEGIN_LINE_FROM_TOP, 3 + strlen("Index"), "%d", course_unit);

						mvwaddch(window, index + REGISTER_LIST_BEGIN_LINE_FROM_TOP, 2 + strlen("Index") + strlen("Units"), ACS_VLINE);

						if (course_classification == Link::classification_t::PRACTICAL_COURSE)
						{
							mvwprintw(window, index + REGISTER_LIST_BEGIN_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units"), "Practical");
						}
						else if (course_classification == Link::classification_t::THEORETICAL_COURSE)
						{
							mvwprintw(window, index + REGISTER_LIST_BEGIN_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units"), "Theoretical ");
						}

						mvwaddch(window, index + REGISTER_LIST_BEGIN_LINE_FROM_TOP, 2 + strlen("Index") + strlen("Units") + strlen("Theoretical "), ACS_VLINE);

						mvwprintw(window, index + REGISTER_LIST_BEGIN_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units") + strlen("Theoretical "), "%s", course_name.substr(0, course_name_cols).c_str());

						mvwaddch(window, index + REGISTER_LIST_BEGIN_LINE_FROM_TOP, 2 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + course_name_cols, ACS_VLINE);

						mvwprintw(window, index + REGISTER_LIST_BEGIN_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + course_name_cols, "%s", course_id.substr(0, course_id_cols).c_str());

						mvwaddch(window, index + REGISTER_LIST_BEGIN_LINE_FROM_TOP, 2 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + course_name_cols + course_id_cols, ACS_VLINE);

						mvwprintw(window, index + REGISTER_LIST_BEGIN_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + course_name_cols + course_id_cols, "%s", course_plan.substr(0, course_plan_cols).c_str());

						mvwaddch(window, index + REGISTER_LIST_BEGIN_LINE_FROM_TOP, 2 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + course_name_cols + course_id_cols + course_plan_cols, ACS_VLINE);

						float grade;
						int student_index;

						for (size_t registered_student_index = 0; registered_student_index < portal.courseRegisterListLength(registered_courses_vector[index + (course_current_list_page - 1) * page_size]); registered_student_index++)
						{
							portal.getCourseRegisterList(registered_courses_vector[index + (course_current_list_page - 1) * page_size], registered_student_index, registered_student_last_name, registered_student_first_name, registered_student_id);

							if (student_first_name == registered_student_first_name && student_last_name == registered_student_last_name && student_id == registered_student_id)
							{
								student_index = registered_student_index;
								break;
							}
						}

						portal.getGrade(registered_courses_vector[index + (course_current_list_page - 1) * page_size], student_index, grade);

						if (grade == -1)
						{
							mvwprintw(window, index + REGISTER_LIST_BEGIN_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + course_name_cols + course_id_cols + course_plan_cols, "%s", "NULL");
						}
						else
						{
							mvwprintw(window, index + REGISTER_LIST_BEGIN_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + course_name_cols + course_id_cols + course_plan_cols, "%0.1f", grade);
						}

						mvwaddch(window, index + REGISTER_LIST_BEGIN_LINE_FROM_TOP, 2 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + course_name_cols + course_id_cols + course_plan_cols + strlen("Grade "), ACS_VLINE);
					}

					getmaxyx(window, max_rows, max_columns);
					mvwprintw(window, max_rows - 1 - REGISTER_LIST_PAGE_NUMBER_OFFSET_FROM_BOTTOM, (max_columns - 3) / 2, "%d/%d", course_current_list_page, course_total_list_pages);
				}
			}
			else if (grade_entry_page)
			{
				if (current_menu == menu_t::GRADE)
				{
					wattron(window, A_BOLD | COLOR_PAIR(WORKSPACE_HIGHLIGHT_COLOR));
					mvwprintw(window, 1, 1, "%s", (char *)" Grade: ");
					wattroff(window, A_BOLD | COLOR_PAIR(WORKSPACE_HIGHLIGHT_COLOR));

					wattron(window, COLOR_PAIR(WORKSPACE_BUTTON_COLOR_OFF));
					mvwprintw(window, 3, 1, "%s", (char *)"  Cancel  ");
					mvwprintw(window, 3, 12, "%s", (char *)"    OK    ");
					wattroff(window, COLOR_PAIR(WORKSPACE_BUTTON_COLOR_OFF));

					wattron(window, A_BLINK);
					mvwaddch(window, 1, strlen(" Grade: ") + 2 + grade_cursor, ACS_CKBOARD);
					wattroff(window, A_BLINK);
				}
				else if (current_menu == menu_t::CANCEL)
				{
					wattron(window, A_BOLD | COLOR_PAIR(WORKSPACE_BUTTON_COLOR_ON));
					mvwprintw(window, 3, 1, "%s", (char *)"  Cancel  ");
					wattroff(window, A_BOLD | COLOR_PAIR(WORKSPACE_BUTTON_COLOR_ON));

					wattron(window, A_DIM);
					mvwprintw(window, 1, 1, "%s", (char *)" Grade: ");
					wattroff(window, A_DIM);
					wattron(window, COLOR_PAIR(WORKSPACE_BUTTON_COLOR_OFF));
					mvwprintw(window, 3, 12, "%s", (char *)"    OK    ");
					wattroff(window, COLOR_PAIR(WORKSPACE_BUTTON_COLOR_OFF));

					mvwaddch(window, 1, strlen(" Grade: ") + 2 + grade_cursor, ' ');
				}
				else if (current_menu == menu_t::OKAY)
				{
					wattron(window, A_BOLD | COLOR_PAIR(WORKSPACE_BUTTON_COLOR_ON));
					mvwprintw(window, 3, 12, "%s", (char *)"    OK    ");
					wattroff(window, A_BOLD | COLOR_PAIR(WORKSPACE_BUTTON_COLOR_ON));

					wattron(window, A_DIM);
					mvwprintw(window, 1, 1, "%s", (char *)" Grade: ");
					wattroff(window, A_DIM);
					wattron(window, COLOR_PAIR(WORKSPACE_BUTTON_COLOR_OFF));
					mvwprintw(window, 3, 1, "%s", (char *)"  Cancel  ");
					wattroff(window, COLOR_PAIR(WORKSPACE_BUTTON_COLOR_OFF));

					mvwaddch(window, 1, strlen(" Grade: ") + 2 + grade_cursor, ' ');
				}
			}
		}
	}
}

void StateGradeEntry::inputHandler(int input)
{
	switch (input)
	{
	case KEY_UP:
		if (!student_selected && !grade_entry_page)
		{
			if (portal.exists() && portal.studentsListLength() > 0)
			{
				student_current_index = (student_current_index + student_list_size - 1) % student_list_size;
				screen_update_flag = true;
			}
		}
		else if (student_selected && !grade_entry_page)
		{
			if (portal.exists() && registered_courses_vector.size() > 0)
			{
				course_current_index = (course_current_index + course_list_size - 1) % course_list_size;
				screen_update_flag = true;
			}
		}
		else if (grade_entry_page)
		{
			current_menu = (menu_t)(((int)current_menu + NUMBER_OF_MENU_OPTIONS - 1) % NUMBER_OF_MENU_OPTIONS);
		}
		break;
	case KEY_DOWN:
		if (!student_selected && !grade_entry_page)
		{
			if (portal.exists() && portal.studentsListLength() > 0)
			{
				student_current_index = (student_current_index + 1) % student_list_size;
				screen_update_flag = true;
			}
		}
		else if (student_selected && !grade_entry_page)
		{
			if (portal.exists() && registered_courses_vector.size() > 0)
			{
				course_current_index = (course_current_index + 1) % course_list_size;
				screen_update_flag = true;
			}
		}
		else if (grade_entry_page)
		{
			current_menu = (menu_t)(((int)current_menu + 1) % NUMBER_OF_MENU_OPTIONS);
		}
		break;
	case KEY_RIGHT:
		if (!student_selected && !grade_entry_page)
		{
			if (student_current_list_page < student_total_list_pages)
			{
				student_current_list_page++;
				student_current_index = (student_current_list_page - 1) * page_size;
			}
		}
		else if (student_selected && !grade_entry_page)
		{
			if (course_current_list_page < course_total_list_pages)
			{
				course_current_list_page++;
				course_current_index = (course_current_list_page - 1) * page_size;
			}
		}
		else if (grade_entry_page)
		{
		}
		break;
	case KEY_LEFT:
		if (!student_selected && !grade_entry_page)
		{
			if (student_current_list_page > 1)
			{
				student_current_list_page--;
				student_current_index = (student_current_list_page - 1) * page_size;
			}
		}
		else if (student_selected && !grade_entry_page)
		{
			if (course_current_list_page > 1)
			{
				course_current_list_page--;
				course_current_index = (course_current_list_page - 1) * page_size;
			}
		}
		else if (grade_entry_page)
		{
		}
		break;
	case (int)13:
		if (!student_selected && !grade_entry_page)
		{
			if (student_list_size != 0)
			{
				student_selected = true;
				werase(window);
			}
		}
		else if (student_selected && registered_courses_vector.size() > 0 && !grade_entry_page)
		{
			if (registered_courses_vector.size() > 0)
			{
				grade_entry_page = true;
				student_selected = true;
				werase(window);
			}
		}
		else if (grade_entry_page)
		{
			switch (current_menu)
			{
			case menu_t::OKAY:
			{
				float grade = 0;

				if (grade_string.length() <= MAXIMUM_COURSE_GRADE_STRING_LENGTH)
				{
					for (size_t index = 0; index < grade_string.length(); index++)
					{
						if (grade_string.c_str()[index] < '0' || grade_string.c_str()[index] > '9')
						{
							if (grade_string[index] != '.')
							{
								grade = 0;
								break;
							}
						}

						if (index == (grade_string.length() - 1))
						{
							grade = stof(grade_string);
							break;
						}
					}
				}
				else
				{
					grade = 0;
				}

				int student_index = 0;
				std::string registered_student_first_name, registered_student_last_name, registered_student_id;
				std::string student_first_name, student_last_name, student_id;

				portal.getStudent(student_current_index, student_first_name, student_last_name, student_id);

				for (size_t registered_student_index = 0; registered_student_index < portal.courseRegisterListLength(registered_courses_vector[course_current_index]); registered_student_index++)
				{
					portal.getCourseRegisterList(registered_courses_vector[course_current_index], registered_student_index, registered_student_last_name, registered_student_first_name, registered_student_id);

					if (student_first_name == registered_student_first_name && student_last_name == registered_student_last_name && student_id == registered_student_id)
					{
						student_index = registered_student_index;
						break;
					}
				}

				if (portal.addGrade(registered_courses_vector[course_current_index], student_index, grade) == Error_code_t::ERROR_OK)
				{
					next_state = State_list_t::State_Notification_Grade_Entry_Success;
					title_window_is_rendered = false;
					main_window_is_rendered = false;
					footer_window_is_rendered = false;

					wbkgd(title_bar_window, COLOR_PAIR(WORKSPACE_TITLE_BAR_COLOR_OFF));

					update_window = title_bar_window;

					current_menu = menu_t::GRADE;
					grade_entry_page = false;
					student_selected = true;
					grade_string.clear();
					grade_cursor = 0;
				}
				else
				{
					next_state = State_list_t::State_Notification_Grade_Entry_Fail;
					title_window_is_rendered = false;
					main_window_is_rendered = false;
					footer_window_is_rendered = false;

					wbkgd(title_bar_window, COLOR_PAIR(WORKSPACE_TITLE_BAR_COLOR_OFF));

					update_window = title_bar_window;

					current_menu = menu_t::GRADE;
					grade_string.clear();
					grade_cursor = 0;
				}

				break;
			}
			case menu_t::CANCEL:
				current_menu = menu_t::GRADE;
				grade_entry_page = false;
				student_selected = true;
				grade_string.clear();
				grade_cursor = 0;
				werase(window);
				break;
			default:
				break;
			}
		}
		break;
	case KEY_BACKSPACE:
		if (!student_selected && !grade_entry_page)
		{
			student_current_list_page = 1;
			course_current_list_page = 1;
			done_calculations = false;

			student_current_index = 0;
			course_current_index = 0;

			update_window = title_bar_window;
			werase(title_bar_window);

			title_window_is_rendered = false;
			main_window_is_rendered = false;
			footer_window_is_rendered = false;

			Default_Workspace.destination_state = State_list_t::State_Registeration_Menu;
			next_state = State_list_t::State_Default_Workspace;
			werase(window);
		}
		else if (student_selected && !grade_entry_page)
		{
			course_current_list_page = 1;
			course_current_index = 0;

			student_selected = false;
			grade_entry_page = false;
			werase(window);
		}
		else if (grade_entry_page)
		{
			if ((current_menu == menu_t::GRADE) && (grade_string.length() > 0))
			{
				mvwaddch(window, 1, strlen(" Grade: ") + 2 + grade_cursor, ' ');
				grade_string.pop_back();
				grade_cursor--;
			}
		}
		break;
	default:
		if (grade_entry_page)
		{
			if ((input >= 32) && (input <= 126))
			{
				if (current_menu == menu_t::GRADE && grade_cursor < MAXIMUM_COURSE_GRADE_STRING_LENGTH)
				{
					grade_string.append((char *)&input);
					mvwprintw(window, 1, strlen(" Grade: ") + 2 + grade_cursor, "%c", input);
					grade_cursor++;
				}
			}
		}
		screen_update_flag = true;
		break;
	}
}
