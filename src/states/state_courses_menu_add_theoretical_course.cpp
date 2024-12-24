#include "states.hpp"
#include <ncurses.h>
#include <cstring>
#include "portal.hpp"
#include "limits_constants.hpp"

inline constexpr int NUMBER_OF_MENU_OPTIONS = 6;

StateAddTheoreticalCourse::StateAddTheoreticalCourse()
{
	current_menu = menu_t::COURSE_NAME;

	course_name_cursor = 0;
	units_cursor = 0;
	id_cursor = 0;
	lesson_plan_cursor = 0;
	units = 0;

	invalid_input = false;

	next_state = State_list_t::State_Add_Theoretical_Course;
}

StateAddTheoreticalCourse::~StateAddTheoreticalCourse()
{
}

void StateAddTheoreticalCourse::formatHandler()
{
	getmaxyx(stdscr, max_rows, max_columns);

	if (!title_window_is_rendered)
	{
		titleWindowRender((char *)"WORKSPACE: ADD THEORETICAL COURSE", TITLE_BAR_WIDTH, max_columns - MENU_COLS, 0, MENU_COLS, WORKSPACE_TITLE_BAR_COLOR_ON);
	}
	else if (!footer_window_is_rendered)
	{
		footerWindowRender((char *)"UP/DOWN: NAVIGATE | OK: ADD COURSE | CANCEL: CANCEL ADD COURSE", FOOTER_BAR_WIDTH, max_columns, max_rows - FOOTER_BAR_WIDTH, 0, WORKSPACE_FOOTER_BAR_COLOR);
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

void StateAddTheoreticalCourse::mainWindowHandler()
{
	next_state = State_list_t::State_Add_Theoretical_Course;

	if (screen_update_flag)
	{
		if (window != nullptr)
		{
			if (invalid_input)
			{
				mvwprintw(window, 1, 0, " ");
				mvwprintw(window, 2, 0, " ");
				mvwprintw(window, 3, 0, " ");
				mvwprintw(window, 4, 0, " ");

				if (course_name.length() == 0)
				{
					wattron(window, A_BOLD | COLOR_PAIR(WORKSPACE_X_COLOR));
					mvwprintw(window, 1, 0, "X");
					wattroff(window, A_BOLD | COLOR_PAIR(WORKSPACE_X_COLOR));
				}

				if (id.length() == 0)
				{
					wattron(window, A_BOLD | COLOR_PAIR(WORKSPACE_X_COLOR));
					mvwprintw(window, 2, 0, "X");
					wattroff(window, A_BOLD | COLOR_PAIR(WORKSPACE_X_COLOR));
				}

				if (units_string.length() == 0)
				{
					wattron(window, A_BOLD | COLOR_PAIR(WORKSPACE_X_COLOR));
					mvwprintw(window, 3, 0, "X");
					wattroff(window, A_BOLD | COLOR_PAIR(WORKSPACE_X_COLOR));
				}

				if (lesson_plan.length() == 0)
				{
					wattron(window, A_BOLD | COLOR_PAIR(WORKSPACE_X_COLOR));
					mvwprintw(window, 4, 0, "X");
					wattroff(window, A_BOLD | COLOR_PAIR(WORKSPACE_X_COLOR));
				}

				mvwprintw(window, 1, strlen(" Course Name: ") + 2, "%s", course_name.c_str());
				mvwprintw(window, 2, strlen(" Course ID: ") + 2, "%s", id.c_str());
				mvwprintw(window, 3, strlen(" Course Units: ") + 2, "%s", units_string.c_str());
				mvwprintw(window, 4, strlen(" Project Plan: ") + 2, "%s", lesson_plan.c_str());
			}

			if (current_menu == menu_t::COURSE_NAME)
			{
				wattron(window, A_BOLD | COLOR_PAIR(WORKSPACE_HIGHLIGHT_COLOR));
				mvwprintw(window, 1, 1, "%s", (char *)" Course Name: ");
				wattroff(window, A_BOLD | COLOR_PAIR(WORKSPACE_HIGHLIGHT_COLOR));

				wattron(window, A_DIM);
				mvwprintw(window, 2, 1, "%s", (char *)" Course ID: ");
				mvwprintw(window, 3, 1, "%s", (char *)" Course Units: ");
				mvwprintw(window, 4, 1, "%s", (char *)" Lesson Plan: ");
				wattroff(window, A_DIM);
				wattron(window, COLOR_PAIR(WORKSPACE_BUTTON_COLOR_OFF));
				mvwprintw(window, 6, 1, "%s", (char *)"  Cancel  ");
				mvwprintw(window, 6, 12, "%s", (char *)"    OK    ");
				wattroff(window, COLOR_PAIR(WORKSPACE_BUTTON_COLOR_OFF));

				wattron(window, A_BLINK);
				mvwaddch(window, 1, strlen(" Course Name: ") + 2 + course_name_cursor, ACS_CKBOARD);
				wattroff(window, A_BLINK);
				mvwaddch(window, 2, strlen(" Course ID: ") + 2 + id_cursor, ' ');
				mvwaddch(window, 3, strlen(" Course Units: ") + 2 + units_cursor, ' ');
				mvwaddch(window, 4, strlen(" Lesson Plan: ") + 2 + lesson_plan_cursor, ' ');
			}
			else if (current_menu == menu_t::ID)
			{
				wattron(window, A_BOLD | COLOR_PAIR(WORKSPACE_HIGHLIGHT_COLOR));
				mvwprintw(window, 2, 1, "%s", (char *)" Course ID: ");
				wattroff(window, A_BOLD | COLOR_PAIR(WORKSPACE_HIGHLIGHT_COLOR));

				wattron(window, A_DIM);
				mvwprintw(window, 1, 1, "%s", (char *)" Course Name: ");
				mvwprintw(window, 3, 1, "%s", (char *)" Course Units: ");
				mvwprintw(window, 4, 1, "%s", (char *)" Lesson Plan: ");
				wattroff(window, A_DIM);
				wattron(window, COLOR_PAIR(WORKSPACE_BUTTON_COLOR_OFF));
				mvwprintw(window, 6, 1, "%s", (char *)"  Cancel  ");
				mvwprintw(window, 6, 12, "%s", (char *)"    OK    ");
				wattroff(window, COLOR_PAIR(WORKSPACE_BUTTON_COLOR_OFF));

				wattron(window, A_BLINK);
				mvwaddch(window, 2, strlen(" Course ID: ") + 2 + id_cursor, ACS_CKBOARD);
				wattroff(window, A_BLINK);
				mvwaddch(window, 1, strlen(" Course Name: ") + 2 + course_name_cursor, ' ');
				mvwaddch(window, 3, strlen(" Course Units: ") + 2 + units_cursor, ' ');
				mvwaddch(window, 4, strlen(" Lesson Plan: ") + 2 + lesson_plan_cursor, ' ');
			}
			else if (current_menu == menu_t::UNITS)
			{
				wattron(window, A_BOLD | COLOR_PAIR(WORKSPACE_HIGHLIGHT_COLOR));
				mvwprintw(window, 3, 1, "%s", (char *)" Course Units: ");
				wattroff(window, A_BOLD | COLOR_PAIR(WORKSPACE_HIGHLIGHT_COLOR));

				wattron(window, A_DIM);
				mvwprintw(window, 1, 1, "%s", (char *)" Course Name: ");
				mvwprintw(window, 2, 1, "%s", (char *)" Course ID: ");
				mvwprintw(window, 4, 1, "%s", (char *)" Lesson Plan: ");
				wattroff(window, A_DIM);
				wattron(window, COLOR_PAIR(WORKSPACE_BUTTON_COLOR_OFF));
				mvwprintw(window, 6, 1, "%s", (char *)"  Cancel  ");
				mvwprintw(window, 6, 12, "%s", (char *)"    OK    ");
				wattroff(window, COLOR_PAIR(WORKSPACE_BUTTON_COLOR_OFF));

				wattron(window, A_BLINK);
				mvwaddch(window, 3, strlen(" Course Units: ") + 2 + units_cursor, ACS_CKBOARD);
				wattroff(window, A_BLINK);
				mvwaddch(window, 1, strlen(" Course Name: ") + 2 + course_name_cursor, ' ');
				mvwaddch(window, 2, strlen(" Course ID: ") + 2 + id_cursor, ' ');
				mvwaddch(window, 4, strlen(" Lesson Plan: ") + 2 + lesson_plan_cursor, ' ');
			}
			else if (current_menu == menu_t::LESSON_PLAN)
			{
				wattron(window, A_BOLD | COLOR_PAIR(WORKSPACE_HIGHLIGHT_COLOR));
				mvwprintw(window, 4, 1, "%s", (char *)" Lesson Plan: ");
				wattroff(window, A_BOLD | COLOR_PAIR(WORKSPACE_HIGHLIGHT_COLOR));

				wattron(window, A_DIM);
				mvwprintw(window, 1, 1, "%s", (char *)" Course Name: ");
				mvwprintw(window, 2, 1, "%s", (char *)" Course ID: ");
				mvwprintw(window, 3, 1, "%s", (char *)" Course Units: ");
				wattroff(window, A_DIM);
				wattron(window, COLOR_PAIR(WORKSPACE_BUTTON_COLOR_OFF));
				mvwprintw(window, 6, 1, "%s", (char *)"  Cancel  ");
				mvwprintw(window, 6, 12, "%s", (char *)"    OK    ");
				wattroff(window, COLOR_PAIR(WORKSPACE_BUTTON_COLOR_OFF));

				wattron(window, A_BLINK);
				mvwaddch(window, 4, strlen(" Lesson Plan: ") + 2 + lesson_plan_cursor, ACS_CKBOARD);
				wattroff(window, A_BLINK);
				mvwaddch(window, 1, strlen(" Course Name: ") + 2 + course_name_cursor, ' ');
				mvwaddch(window, 2, strlen(" Course ID: ") + 2 + id_cursor, ' ');
				mvwaddch(window, 3, strlen(" Course Units: ") + 2 + units_cursor, ' ');
			}
			else if (current_menu == menu_t::CANCEL)
			{
				wattron(window, A_BOLD | COLOR_PAIR(WORKSPACE_BUTTON_COLOR_ON));
				mvwprintw(window, 6, 1, "%s", (char *)"  Cancel  ");
				wattroff(window, A_BOLD | COLOR_PAIR(WORKSPACE_BUTTON_COLOR_ON));

				wattron(window, A_DIM);
				mvwprintw(window, 1, 1, "%s", (char *)" Course Name: ");
				mvwprintw(window, 2, 1, "%s", (char *)" Course ID: ");
				mvwprintw(window, 3, 1, "%s", (char *)" Course Units: ");
				mvwprintw(window, 4, 1, "%s", (char *)" Lesson Plan: ");
				wattroff(window, A_DIM);
				wattron(window, COLOR_PAIR(WORKSPACE_BUTTON_COLOR_OFF));
				mvwprintw(window, 6, 12, "%s", (char *)"    OK    ");
				wattroff(window, COLOR_PAIR(WORKSPACE_BUTTON_COLOR_OFF));

				mvwaddch(window, 1, strlen(" Course Name: ") + 2 + course_name_cursor, ' ');
				mvwaddch(window, 2, strlen(" Course ID: ") + 2 + id_cursor, ' ');
				mvwaddch(window, 3, strlen(" Course Units: ") + 2 + units_cursor, ' ');
				mvwaddch(window, 4, strlen(" Lesson Plan: ") + 2 + lesson_plan_cursor, ' ');
			}
			else if (current_menu == menu_t::OKAY)
			{
				wattron(window, A_BOLD | COLOR_PAIR(WORKSPACE_BUTTON_COLOR_ON));
				mvwprintw(window, 6, 12, "%s", (char *)"    OK    ");
				wattroff(window, A_BOLD | COLOR_PAIR(WORKSPACE_BUTTON_COLOR_ON));

				wattron(window, A_DIM);
				mvwprintw(window, 1, 1, "%s", (char *)" Course Name: ");
				mvwprintw(window, 2, 1, "%s", (char *)" Course ID: ");
				mvwprintw(window, 3, 1, "%s", (char *)" Course Units: ");
				mvwprintw(window, 4, 1, "%s", (char *)" Lesson Plan: ");
				wattroff(window, A_DIM);
				wattron(window, COLOR_PAIR(WORKSPACE_BUTTON_COLOR_OFF));
				mvwprintw(window, 6, 1, "%s", (char *)"  Cancel  ");
				wattroff(window, COLOR_PAIR(WORKSPACE_BUTTON_COLOR_OFF));

				mvwaddch(window, 1, strlen(" Course Name: ") + 2 + course_name_cursor, ' ');
				mvwaddch(window, 2, strlen(" Course ID: ") + 2 + id_cursor, ' ');
				mvwaddch(window, 3, strlen(" Course Units: ") + 2 + units_cursor, ' ');
				mvwaddch(window, 4, strlen(" Lesson Plan: ") + 2 + lesson_plan_cursor, ' ');
			}
		}
	}
}

void StateAddTheoreticalCourse::inputHandler(int input)
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
			if ((course_name.length() != 0) && (lesson_plan.length() != 0) && (id.length() != 0) && (units_string.length() != 0))
			{
				if (!portal.exists())
				{
					portal.newPortal();
				}

				if (units_string.length() <= 10)
				{
					for (size_t index = 0; index < units_string.length(); index++)
					{
						if (units_string.c_str()[index] < '0' || units_string.c_str()[index] > '9')
						{
							units = 0;
							break;
						}

						if (index == (units_string.length() - 1))
						{
							units = stoi(units_string);
							break;
						}
					}
				}
				else
				{
					units = 0;
				}

				size_t dummy_index;
				if (portal.searchCourseByID(id, dummy_index) != Error_code_t::ERROR_OK)
				{
					portal.addCourse(Link::classification_t::THEORETICAL_COURSE, course_name, id, lesson_plan, units);

					course_name.clear();
					units_string.clear();
					id.clear();
					lesson_plan.clear();

					course_name_cursor = units_cursor = id_cursor = lesson_plan_cursor = 0;

					current_menu = menu_t::COURSE_NAME;

					title_window_is_rendered = false;
					main_window_is_rendered = false;
					footer_window_is_rendered = false;

					invalid_input = false;

					next_state = State_list_t::State_Notification_Course_Add_Success;
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
					next_state = State_list_t::State_Notification_Theoretical_Course_Add_Duplicate;
				}
			}
			else
			{
				title_window_is_rendered = false;
				main_window_is_rendered = false;
				footer_window_is_rendered = false;

				invalid_input = true;

				current_menu = menu_t::COURSE_NAME;

				next_state = State_list_t::State_Notification_Theoretical_Course_Add_Fail;
			}

			wbkgd(title_bar_window, WORKSPACE_TITLE_BAR_COLOR_OFF);
			update_window = title_bar_window;
		}
		else if (current_menu == menu_t::CANCEL)
		{
			course_name.clear();
			id.clear();
			lesson_plan.clear();

			course_name_cursor = id_cursor = lesson_plan_cursor = units_cursor = 0;

			current_menu = menu_t::COURSE_NAME;

			invalid_input = false;

			title_window_is_rendered = false;
			main_window_is_rendered = false;
			footer_window_is_rendered = false;

			Default_Workspace.destination_state = State_list_t::State_Courses_Menu;
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
		if (current_menu == menu_t::COURSE_NAME)
		{
			if (course_name.length() > 0)
			{
				mvwaddch(window, 1, strlen(" Course Name: ") + 2 + course_name_cursor, ' ');
				course_name.pop_back();
				course_name_cursor--;
			}
		}
		else if (current_menu == menu_t::LESSON_PLAN)
		{
			if (lesson_plan.length() > 0)
			{
				mvwaddch(window, 4, strlen(" Lesson Plan: ") + 2 + lesson_plan_cursor, ' ');
				lesson_plan.pop_back();
				lesson_plan_cursor--;
			}
		}
		else if (current_menu == menu_t::ID)
		{
			if (id.length() > 0)
			{
				mvwaddch(window, 2, strlen(" Course ID: ") + 2 + id_cursor, ' ');
				id.pop_back();
				id_cursor--;
			}
		}
		else if (current_menu == menu_t::UNITS)
		{
			if (units_string.length() > 0)
			{
				mvwaddch(window, 3, strlen(" Course Units: ") + 2 + units_cursor, ' ');
				units_string.pop_back();
				units_cursor--;
			}
		}
		break;
	case -1:
		break;
	default:
		if ((input >= 32) && (input <= 126))
		{
			if (current_menu == menu_t::COURSE_NAME && course_name_cursor < MAXIMUM_COURSE_NAME_LENGTH)
			{
				course_name.append((char *)&input);
				mvwprintw(window, 1, strlen(" Course Name: ") + 2 + course_name_cursor, "%c", input);
				course_name_cursor++;
			}
			else if (current_menu == menu_t::UNITS)
			{
				units_string.append((char *)&input);
				mvwprintw(window, 3, strlen(" Course Units: ") + 2 + units_cursor, "%c", input);
				units_cursor++;
			}
			else if (current_menu == menu_t::LESSON_PLAN && lesson_plan_cursor < MAXIMUM_PLAN_LENGTH)
			{
				lesson_plan.append((char *)&input);
				mvwprintw(window, 4, strlen(" Lesson Plan: ") + 2 + lesson_plan_cursor, "%c", input);
				lesson_plan_cursor++;
			}
			else if (current_menu == menu_t::ID && id_cursor < MAXIMUM_COURSE_ID_LENGTH)
			{
				id.append((char *)&input);
				mvwprintw(window, 2, strlen(" Course ID: ") + 2 + id_cursor, "%c", input);
				id_cursor++;
			}
		}

		screen_update_flag = true;
		break;
	}
}
