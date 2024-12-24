#include "states.hpp"
#include <ncurses.h>
#include <cstring>
#include "limits_constants.hpp"
#include "portal.hpp"
#include <libgen.h>		  // dirname
#include <unistd.h>		  // readlink
#include <linux/limits.h> // PATH_MAX

StateLoadFromFile::StateLoadFromFile()
{
}

StateLoadFromFile::~StateLoadFromFile()
{
}

StateLoadFromFile::state_return_t StateLoadFromFile::stateFunction(int input)
{
	State_list_t next_state = State_list_t::State_Default_Workspace;

	if (open_file((char *)"/student.txt", input_student_f, std::ios::in) != Error_code_t::ERROR_OK)
	{
		Default_Workspace.load_success = false;
		return {next_state, update_window, screen_update_flag};
	}

	if (open_file((char *)"/course.txt", input_course_f, std::ios::in) != Error_code_t::ERROR_OK)
	{
		Default_Workspace.load_success = false;
		return {next_state, update_window, screen_update_flag};
	}

	if (!portal.exists())
	{
		portal.newPortal();
	}

	if (loadStudent() != Error_code_t::ERROR_OK)
	{
	}

	if (loadCourse() != Error_code_t::ERROR_OK)
	{
	}

	input_student_f.close();
	input_course_f.close();

	return {next_state, update_window, screen_update_flag};
}

Error_code_t StateLoadFromFile::loadStudent()
{
	std::string line, last_name, first_name, id;
	int student_count;
	bool has_error = false;

	// getting student count from first line
	input_student_f >> student_count;
	getline(input_student_f, line, '\n');

	if ((line.length() != 0) || (student_count < 0))
	{
		has_error = true;
	}

	for (int counter = 0; (!input_student_f.eof()) && (counter < student_count) && (!has_error); counter++)
	{
		int student_index;
		int last_name_length;
		int first_name_length;
		int id_length;

		// getting student index
		input_student_f >> student_index;
		if (student_index != counter)
		{
			has_error = true;
			continue;
		}

		getline(input_student_f, line, '|');

		// getting last name length
		input_student_f >> last_name_length;
		if (last_name_length < 0 || last_name_length > MAXIMUM_LAST_NAME_LENGTH)
		{
			has_error = true;
			continue;
		}

		getline(input_student_f, line, '|');

		// getting last name
		char last_name_char[MAXIMUM_LAST_NAME_LENGTH];
		input_student_f.read(last_name_char, last_name_length);
		last_name_char[last_name_length] = '\0';
		last_name = last_name_char;

		getline(input_student_f, line, '|');

		// getting first name length
		input_student_f >> first_name_length;
		if (first_name_length < 0 || first_name_length > MAXIMUM_FIRST_NAME_LENGTH)
		{
			has_error = true;
			continue;
		}

		getline(input_student_f, line, '|');

		// getting first name
		char first_name_char[MAXIMUM_FIRST_NAME_LENGTH];
		input_student_f.read(first_name_char, first_name_length);
		first_name_char[first_name_length] = '\0';
		first_name = first_name_char;

		getline(input_student_f, line, '|');

		// getting id length
		input_student_f >> id_length;
		if (id_length < 0 || id_length > MAXIMUM_STUDENT_ID_LENGTH)
		{
			has_error = true;
			continue;
		}

		getline(input_student_f, line, '|');

		// getting id
		char id_char[MAXIMUM_STUDENT_ID_LENGTH];
		input_student_f.read(id_char, id_length);
		id_char[id_length] = '\0';
		id = id_char;

		getline(input_student_f, line, '\n');

		portal.addStudent(first_name, last_name, id);

		first_name.clear();
		last_name.clear();
		id.clear();
	}

	if (has_error)
	{
		return Error_code_t::ERROR_FILE_LOAD;
	}

	return Error_code_t::ERROR_OK;
}

Error_code_t StateLoadFromFile::loadCourse()
{
	bool has_error = false;
	std::string line, course_name, course_id, course_plan;
	Link::classification_t course_classification;
	int course_units;

	int course_count;

	// getting course count from first line
	input_course_f >> course_count;
	getline(input_course_f, line, '\n');

	if ((line.length() != 0) || (course_count < 0))
	{
		has_error = true;
	}

	for (int counter = 0; (!input_course_f.eof()) && (counter < course_count) && (!has_error); counter++)
	{
		int course_index;
		int course_name_length;
		int course_id_length;
		int course_plan_length;
		int course_classification_int;

		// getting course index
		input_course_f >> course_index;
		if (course_index != counter)
		{
			has_error = true;
			continue;
		}

		getline(input_course_f, line, '|');

		// getting course classification
		input_course_f >> course_classification_int;

		switch (course_classification_int)
		{
		case 0:
			course_classification = Link::classification_t::THEORETICAL_COURSE;
			break;
		case 1:
			course_classification = Link::classification_t::PRACTICAL_COURSE;
			break;
		default:
			has_error = true;
			break;
		}

		getline(input_course_f, line, '|');

		// getting course name length
		input_course_f >> course_name_length;

		if (course_name_length < 0 || course_name_length > MAXIMUM_COURSE_NAME_LENGTH)
		{
			has_error = true;
			continue;
		}

		getline(input_course_f, line, '|');

		// getting course name
		char course_name_char[MAXIMUM_COURSE_NAME_LENGTH];
		input_course_f.read(course_name_char, course_name_length);
		course_name_char[course_name_length] = '\0';
		course_name = course_name_char;

		getline(input_course_f, line, '|');

		// getting course id length
		input_course_f >> course_id_length;

		if (course_id_length < 0 || course_id_length > MAXIMUM_COURSE_ID_LENGTH)
		{
			has_error = true;
			continue;
		}

		getline(input_course_f, line, '|');

		// getting course id
		char course_id_char[MAXIMUM_COURSE_ID_LENGTH];
		input_course_f.read(course_id_char, course_id_length);
		course_id_char[course_id_length] = '\0';
		course_id = course_id_char;

		getline(input_course_f, line, '|');

		// getting course plan length
		input_course_f >> course_plan_length;

		if (course_plan_length < 0 || course_plan_length > MAXIMUM_PLAN_LENGTH)
		{
			has_error = true;
			continue;
		}

		getline(input_course_f, line, '|');

		// getting course plan
		char course_plan_char[MAXIMUM_PLAN_LENGTH];
		input_course_f.read(course_plan_char, course_plan_length);
		course_plan_char[course_plan_length] = '\0';
		course_plan = course_plan_char;

		getline(input_course_f, line, '|');

		// getting course unit
		input_course_f >> course_units;

		if (course_units <= 0)
		{
			has_error = true;
			continue;
		}

		getline(input_course_f, line, '|');

		// add course
		portal.addCourse(course_classification, course_name, course_id, course_plan, course_units);

		// getting course register list length
		int course_register_list_length;
		input_course_f >> course_register_list_length;

		if (course_register_list_length < 0)
		{
			has_error = true;
			continue;
		}

		if (course_register_list_length != 0)
		{
			getline(input_course_f, line, '|');
		}

		// getting the list of registered students and their grades
		for (int registered_student_counter = 0; (registered_student_counter < course_register_list_length) && (!has_error); registered_student_counter++)
		{
			size_t student_index;
			float student_grade;

			// get student index
			input_course_f >> student_index;

			if (student_index > portal.studentsListLength())
			{
				has_error = true;
				continue;
			}

			getline(input_course_f, line, '|');

			// get student grade
			input_course_f >> student_grade;

			if ((student_grade > MAXIMUM_GRADE) || (student_grade < GRADES_NOT_ENTERED))
			{
				has_error = true;
				continue;
			}

			if (registered_student_counter != (course_register_list_length - 1))
			{
				getline(input_course_f, line, '|');
			}

			// register student and add grade
			portal.registerCourse(student_index, course_index);
			portal.addGrade(course_index, registered_student_counter, student_grade);
		}

		if (has_error)
		{
			continue;
		}

		getline(input_course_f, line, '\n');
	}

	if (has_error)
	{
		return Error_code_t::ERROR_FILE_LOAD;
	}

	return Error_code_t::ERROR_OK;
}