#include "states.hpp"
#include <ncurses.h>
#include <cstring>
#include "limits_constants.hpp"
#include "portal.hpp"
#include <libgen.h>		  // dirname
#include <unistd.h>		  // readlink
#include <linux/limits.h> // PATH_MAX

StateSaveToFile::StateSaveToFile()
{
}

StateSaveToFile::~StateSaveToFile()
{
}

StateSaveToFile::state_return_t StateSaveToFile::stateFunction(int input)
{
	State_list_t next_state = State_list_t::State_Exit;

	if (open_file((char *)"/student.txt", output_student_f, std::ios::out | std::ios::trunc) != Error_code_t::ERROR_OK)
	{
		return {next_state, update_window, screen_update_flag};
	}

	if (open_file((char *)"/course.txt", output_course_f, std::ios::out | std::ios::trunc) != Error_code_t::ERROR_OK)
	{
		return {next_state, update_window, screen_update_flag};
	}

	printToStudent();
	printToCourse();

	output_course_f.close();
	output_student_f.close();

	return {next_state, update_window, screen_update_flag};
}

Error_code_t StateSaveToFile::printToCourse()
{
	size_t course_count = portal.coursesListLength();
	output_course_f << course_count << "\n";

	for (size_t course_index = 0; course_index < course_count; course_index++)
	{
		std::string course_name, course_id, course_plan;
		int course_unit;
		Link::classification_t course_classification;

		portal.getCourse(course_index, course_classification, course_name, course_id, course_plan, course_unit);

		output_course_f << course_index << "|";
		output_course_f << (int)course_classification << "|";
		output_course_f << course_name.length() << "|";
		output_course_f << course_name << "|";
		output_course_f << course_id.length() << "|";
		output_course_f << course_id << "|";
		output_course_f << course_plan.length() << "|";
		output_course_f << course_plan << "|";
		output_course_f << std::to_string(course_unit) << "|";
		output_course_f << portal.courseRegisterListLength(course_index);

		for (size_t registered_student_index = 0; registered_student_index < portal.courseRegisterListLength(course_index); registered_student_index++)
		{
			size_t student_index;
			float grade;

			portal.getStudentIndexFromRegisteredIndex(course_index, registered_student_index, student_index);

			output_course_f << "|" << student_index;

			portal.getGrade(course_index, registered_student_index, grade);

			output_course_f << "|" << grade;
		}

		output_course_f << '\n';
	}

	return Error_code_t::ERROR_OK;
}

Error_code_t StateSaveToFile::printToStudent()
{
	size_t student_count = portal.studentsListLength();
	output_student_f << student_count << "\n";

	for (size_t student_index = 0; student_index < student_count; student_index++)
	{
		std::string student_first_name, student_last_name, student_id;

		portal.getStudent(student_index, student_first_name, student_last_name, student_id);

		output_student_f << student_index << "|" << student_last_name.length() << "|" << student_last_name << "|" << student_first_name.length() << "|" << student_first_name << "|" << student_id.length() << "|" << student_id << '\n';
	}

	return Error_code_t::ERROR_OK;
}