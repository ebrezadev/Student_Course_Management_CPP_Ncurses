#include "states.hpp"
#include <ncurses.h>
#include <cstring>
#include "portal.hpp"
#include "limits_constants.hpp"

inline constexpr int NUMBER_OF_MENU_OPTIONS = 3;

StateSearchCourse::StateSearchCourse()
{
	current_menu = menu_t::COURSE_ID;
	id_cursor = 0;
	next_state = State_list_t::State_Search_Course;
}

StateSearchCourse::~StateSearchCourse()
{
}

void StateSearchCourse::formatHandler()
{
	getmaxyx(stdscr, max_rows, max_columns);

	if (!title_window_is_rendered)
	{
		titleWindowRender((char *)"WORKSPACE: SEARCH COURSE", TITLE_BAR_WIDTH, max_columns - MENU_COLS, 0, MENU_COLS, WORKSPACE_TITLE_BAR_COLOR_ON);
	}
	else if (!footer_window_is_rendered)
	{
		footerWindowRender((char *)"UP/DOWN: NAVIGATE | CANCEL: COURSE MENU | SEARCH: SEARCH COURSE", FOOTER_BAR_WIDTH, max_columns, max_rows - FOOTER_BAR_WIDTH, 0, WORKSPACE_FOOTER_BAR_COLOR);
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

void StateSearchCourse::mainWindowHandler()
{
	next_state = State_list_t::State_Search_Course;

	if (screen_update_flag)
	{
		if (window != nullptr)
		{
			if (current_menu == menu_t::COURSE_ID)
			{
				wattron(window, A_BOLD | COLOR_PAIR(WORKSPACE_HIGHLIGHT_COLOR));
				mvwprintw(window, 1, 1, "%s", (char *)" Course ID: ");
				wattroff(window, A_BOLD | COLOR_PAIR(WORKSPACE_HIGHLIGHT_COLOR));

				wattron(window, COLOR_PAIR(WORKSPACE_BUTTON_COLOR_OFF));
				mvwprintw(window, 3, 1, "%s", (char *)"  Cancel  ");
				mvwprintw(window, 3, 12, "%s", (char *)"  Search  ");
				wattroff(window, COLOR_PAIR(WORKSPACE_BUTTON_COLOR_OFF));

				wattron(window, A_BLINK);
				mvwaddch(window, 1, strlen(" Course ID: ") + 2 + id_cursor, ACS_CKBOARD);
				wattroff(window, A_BLINK);
			}
			else if (current_menu == menu_t::CANCEL)
			{
				wattron(window, A_BOLD | COLOR_PAIR(WORKSPACE_BUTTON_COLOR_ON));
				mvwprintw(window, 3, 1, "%s", (char *)"  Cancel  ");
				wattroff(window, A_BOLD | COLOR_PAIR(WORKSPACE_BUTTON_COLOR_ON));

				wattron(window, A_DIM);
				mvwprintw(window, 1, 1, "%s", (char *)" Course ID: ");
				wattroff(window, A_DIM);
				wattron(window, COLOR_PAIR(WORKSPACE_BUTTON_COLOR_OFF));
				mvwprintw(window, 3, 12, "%s", (char *)"  Search  ");
				wattroff(window, COLOR_PAIR(WORKSPACE_BUTTON_COLOR_OFF));

				mvwaddch(window, 1, strlen(" Course ID: ") + 2 + id_cursor, ' ');
			}
			else if (current_menu == menu_t::SEARCH)
			{
				wattron(window, A_BOLD | COLOR_PAIR(WORKSPACE_BUTTON_COLOR_ON));
				mvwprintw(window, 3, 12, "%s", (char *)"  Search  ");
				wattroff(window, A_BOLD | COLOR_PAIR(WORKSPACE_BUTTON_COLOR_ON));

				wattron(window, A_DIM);
				mvwprintw(window, 1, 1, "%s", (char *)" Course ID: ");
				wattroff(window, A_DIM);
				wattron(window, COLOR_PAIR(WORKSPACE_BUTTON_COLOR_OFF));
				mvwprintw(window, 3, 1, "%s", (char *)"  Cancel  ");
				wattroff(window, COLOR_PAIR(WORKSPACE_BUTTON_COLOR_OFF));

				mvwaddch(window, 1, strlen(" Course ID: ") + 2 + id_cursor, ' ');
			}
		}
	}
}

void StateSearchCourse::inputHandler(int input)
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

					if (portal.searchCourseByID(id, index) == Error_code_t::ERROR_OK)
					{
						getmaxyx(window, max_rows, max_columns);

						int list_cols = max_columns - 25;
						int name_cols = (COURSE_NAME_COLS_PERCENT * list_cols) / 100;
						int plan_cols = (COURSE_PLAN_COLS_PERCENT * list_cols) / 100;
						int id_cols = (COURSE_ID_COLS_PERCENT * list_cols) / 100;

						wattron(window, COLOR_PAIR(WORKSPACE_LIST_GUIDE));

						mvwaddch(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP, 1, ACS_VLINE);
						mvwprintw(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP, 2, "Index");
						mvwaddch(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP, 2 + strlen("Index"), ACS_VLINE);
						mvwprintw(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index"), "Units");
						mvwaddch(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP, 2 + strlen("Index") + strlen("Units"), ACS_VLINE);
						for (size_t index = 0; index < (strlen("Theoretical ") - strlen("Type")) / 2; index++)
						{
							mvwaddch(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units") + index, ' ');
						}

						mvwprintw(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units") + (strlen("Theoretical ") - strlen("Type")) / 2, "Type");

						for (size_t index = 0; index < (strlen("Theoretical ") - strlen("Type")) / 2; index++)
						{
							mvwaddch(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units") + (strlen("Theoretical ") + strlen("Type")) / 2 + index, ' ');
						}

						mvwaddch(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP, 2 + strlen("Index") + strlen("Units") + strlen("Theoretical "), ACS_VLINE);

						for (size_t index = 0; index < (name_cols - strlen("Name")) / 2; index++)
						{
							mvwaddch(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + index, ' ');
						}

						mvwprintw(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + (name_cols - strlen("Name")) / 2, "Name");

						for (size_t index = 0; index < (name_cols - strlen("Name")) / 2; index++)
						{
							mvwaddch(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + (name_cols + strlen("Name")) / 2 + index, ' ');
						}

						mvwaddch(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP, 2 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + name_cols, ACS_VLINE);

						for (size_t index = 0; index < (id_cols - strlen("ID")) / 2; index++)
						{
							mvwaddch(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + name_cols + index, ' ');
						}

						mvwprintw(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + name_cols + (id_cols - strlen("ID")) / 2, "ID");

						for (size_t index = 0; index < (id_cols - strlen("ID")) / 2; index++)
						{
							mvwaddch(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + name_cols + (id_cols + strlen("ID")) / 2 + index, ' ');
						}

						mvwaddch(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP, 2 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + name_cols + id_cols, ACS_VLINE);

						for (size_t index = 0; index < (plan_cols - strlen("Plan")) / 2; index++)
						{
							mvwaddch(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + name_cols + id_cols + index, ' ');
						}

						mvwprintw(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + name_cols + id_cols + (plan_cols - strlen("Plan")) / 2, "Plan");

						for (size_t index = 0; index < (plan_cols - strlen("Plan")) / 2; index++)
						{
							mvwaddch(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP, 3 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + name_cols + id_cols + (plan_cols + strlen("Plan")) / 2 + index, ' ');
						}

						mvwaddch(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP, 2 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + name_cols + id_cols + plan_cols, ACS_VLINE);

						wattroff(window, COLOR_PAIR(WORKSPACE_LIST_GUIDE));

						Link::classification_t course_classification;

						portal.getCourse(index, course_classification, course_name, id, course_plan, course_unit);

						mvwaddch(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP + 1, 1, ACS_VLINE);

						mvwprintw(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP + 1, 2, "%d", (int)index);

						mvwaddch(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP + 1, 2 + strlen("Index"), ACS_VLINE);

						mvwprintw(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP + 1, 3 + strlen("Index"), "%d", course_unit);

						mvwaddch(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP + 1, 2 + strlen("Index") + strlen("Units"), ACS_VLINE);

						if (course_classification == Link::classification_t::PRACTICAL_COURSE)
						{
							mvwprintw(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP + 1, 3 + strlen("Index") + strlen("Units"), "Practical");
						}
						else if (course_classification == Link::classification_t::THEORETICAL_COURSE)
						{
							mvwprintw(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP + 1, 3 + strlen("Index") + strlen("Units"), "Theoretical ");
						}

						mvwaddch(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP + 1, 2 + strlen("Index") + strlen("Units") + strlen("Theoretical "), ACS_VLINE);

						mvwprintw(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP + 1, 3 + strlen("Index") + strlen("Units") + strlen("Theoretical "), "%s", course_name.substr(0, name_cols).c_str());

						mvwaddch(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP + 1, 2 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + name_cols, ACS_VLINE);

						mvwprintw(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP + 1, 3 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + name_cols, "%s", id.substr(0, id_cols).c_str());

						mvwaddch(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP + 1, 2 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + name_cols + id_cols, ACS_VLINE);

						mvwprintw(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP + 1, 3 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + name_cols + id_cols, "%s", course_plan.substr(0, plan_cols).c_str());

						mvwaddch(window, SEARCH_LIST_GUIDE_LINE_FROM_TOP + 1, 2 + strlen("Index") + strlen("Units") + strlen("Theoretical ") + name_cols + id_cols + plan_cols, ACS_VLINE);
					}
					else
					{
						mvwprintw(window, 5, 1, "No course found.");
					}
				}
				else
				{
					mvwprintw(window, 5, 1, "No course list.");
				}
				course_name.clear();
				course_plan.clear();
				id.clear();
				course_unit = 0;

				id_cursor = 0;
			}
			else
			{
				mvwprintw(window, 5, 1, "Please enter a valid value.");

				current_menu = menu_t::COURSE_ID;
			}
		}
		else if (current_menu == menu_t::CANCEL)
		{
			course_name.clear();
			course_plan.clear();
			id.clear();

			id_cursor = 0;

			current_menu = menu_t::COURSE_ID;

			title_window_is_rendered = false;
			main_window_is_rendered = false;
			footer_window_is_rendered = false;

			wbkgd(title_bar_window, COLOR_PAIR(WORKSPACE_TITLE_BAR_COLOR_OFF));

			update_window = title_bar_window;
			Default_Workspace.destination_state = State_list_t::State_Courses_Menu;
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
		if ((current_menu == menu_t::COURSE_ID) && (id.length() > 0))
		{
			mvwaddch(window, 1, strlen(" Course ID: ") + 2 + id_cursor, ' ');
			id.pop_back();
			id_cursor--;
		}
		break;
	case -1:
		break;
	default:
		if ((input >= 32) && (input <= 126))
		{
			if (current_menu == menu_t::COURSE_ID && id_cursor < MAXIMUM_COURSE_ID_LENGTH)
			{
				id.append((char *)&input);
				mvwprintw(window, 1, strlen(" Course ID: ") + 2 + id_cursor, "%c", input);
				id_cursor++;
			}
		}

		screen_update_flag = true;
		break;
	}
}
