#include "portal.hpp"
#include "ncurses.h"
#include "limits_constants.hpp"

Portal portal;

Portal::Portal()
{
	students_list = nullptr;
	courses_list = nullptr;
	exists_b = false;
}

Portal::~Portal()
{
}

void Portal::newPortal()
{
	students_list = new List(List::classification_t::STUDENT_LIST);
	courses_list = new List(List::classification_t::COURSE_LIST);
	exists_b = true;
}

void Portal::deletePortal()
{
	if (students_list != nullptr)
		delete students_list;

	if (courses_list != nullptr)
		delete courses_list;

	students_list = courses_list = nullptr;

	exists_b = false;
}

bool Portal::exists()
{
	return exists_b;
}

size_t Portal::studentsListLength()
{
	if (students_list == nullptr)
	{
		return 0;
	}

	return students_list->size();
}

size_t Portal::coursesListLength()
{
	if (courses_list == nullptr)
	{
		return 0;
	}

	return courses_list->size();
}

size_t Portal::courseRegisterListLength(size_t course_index)
{
	size_t size;

	switch ((*courses_list)[course_index].link_classification)
	{
	case Link::classification_t::PRACTICAL_COURSE:
		size = (*courses_list)[course_index].data->practical_course->list_of_enrolled_students->size();
		break;
	case Link::classification_t::THEORETICAL_COURSE:
		size = (*courses_list)[course_index].data->theoretical_course->list_of_enrolled_students->size();
		break;
	default:
		size = 0;
		break;
	}
	
	return size;
}

Error_code_t Portal::addStudent(const std::string &first_name, const std::string &last_name, const std::string &id)
{
	if (students_list == nullptr)
	{
		return Error_code_t::ERROR_NO_PORTAL;
	}

	Link::Data student(Link::classification_t::STUDENT);

	student.student->first_name = first_name;
	student.student->family_name = last_name;
	student.student->id = id;

	return students_list->addLink(student, Link::classification_t::STUDENT);
}

Error_code_t Portal::getStudent(size_t index, std::string &first_name, std::string &last_name, std::string &id)
{
	if (students_list == nullptr)
	{
		return Error_code_t::ERROR_NO_PORTAL;
	}

	if (index >= students_list->size())
	{
		return Error_code_t::ERROR_INVALID_INDEX;
	}

	first_name = (*students_list)[index].data->student->first_name;
	last_name = (*students_list)[index].data->student->family_name;
	id = (*students_list)[index].data->student->id;

	return Error_code_t::ERROR_OK;
}

Error_code_t Portal::addCourse(Link::classification_t course_classification, const std::string &course_name, const std::string &course_id, const std::string &course_plan, int course_units)
{
	if (courses_list == nullptr)
	{
		return Error_code_t::ERROR_NO_PORTAL;
	}

	Link::Data course(course_classification);

	switch (course_classification)
	{
	case Link::classification_t::PRACTICAL_COURSE:
		course.practical_course->name = course_name;
		course.practical_course->id = course_id;
		course.practical_course->project_plan = course_plan;
		course.practical_course->units = course_units;
		break;
	case Link::classification_t::THEORETICAL_COURSE:
		course.theoretical_course->name = course_name;
		course.theoretical_course->id = course_id;
		course.theoretical_course->lesson_plan = course_plan;
		course.theoretical_course->units = course_units;
		break;
	default:
		return Error_code_t::ERROR_CLASSIFICATION_MISMATCH;
		break;
	}

	return courses_list->addLink(course, course_classification);
}

Error_code_t Portal::getCourse(size_t index, Link::classification_t &course_classification, std::string &course_name, std::string &course_id, std::string &course_plan, int &course_units)
{
	if (courses_list == nullptr)
	{
		return Error_code_t::ERROR_NO_PORTAL;
	}

	if (index >= courses_list->size())
	{
		return Error_code_t::ERROR_INVALID_INDEX;
	}

	course_classification = (*courses_list)[index].link_classification;

	switch ((*courses_list)[index].link_classification)
	{
	case Link::classification_t::THEORETICAL_COURSE:
		course_name = (*courses_list)[index].data->theoretical_course->name;
		course_id = (*courses_list)[index].data->theoretical_course->id;
		course_plan = (*courses_list)[index].data->theoretical_course->lesson_plan;
		course_units = (*courses_list)[index].data->theoretical_course->units;
		break;
	case Link::classification_t::PRACTICAL_COURSE:
		course_name = (*courses_list)[index].data->practical_course->name;
		course_id = (*courses_list)[index].data->practical_course->id;
		course_plan = (*courses_list)[index].data->practical_course->project_plan;
		course_units = (*courses_list)[index].data->practical_course->units;
		break;
	default:
		return Error_code_t::ERROR_CLASSIFICATION_MISMATCH;
	}

	return Error_code_t::ERROR_OK;
}

Error_code_t Portal::removeStudent(size_t index)
{
	return students_list->removeLink(index);
}

Error_code_t Portal::removeCourse(size_t index)
{
	return courses_list->removeLink(index);
}

Error_code_t Portal::searchStudentByID(std::string id, size_t &index)
{
	return students_list->searchLinkID(id, index);
}

Error_code_t Portal::searchCourseByID(std::string id, size_t &index)
{
	return courses_list->searchLinkID(id, index);
}

Error_code_t Portal::registerCourse(size_t student_index, size_t course_index)
{
	if (student_index >= studentsListLength() || course_index >= coursesListLength())
	{
		return Error_code_t::ERROR_INVALID_INDEX;
	}

	std::string student_last_name, student_first_name, student_id;

	getStudent(student_index, student_first_name, student_last_name, student_id);

	switch ((*courses_list)[course_index].link_classification)
	{
	case Link::classification_t::PRACTICAL_COURSE:
		for (size_t index = 0; index < (*courses_list)[course_index].data->practical_course->list_of_enrolled_students->size(); index++)
		{
			if (student_id == (*((*courses_list)[course_index].data->practical_course->list_of_enrolled_students))[index].data->student->id)
			{
				return Error_code_t::ERROR_DUPLICATE_REGISTER;
			}
		}
		break;
	case Link::classification_t::THEORETICAL_COURSE:
		for (size_t index = 0; index < (*courses_list)[course_index].data->theoretical_course->list_of_enrolled_students->size(); index++)
		{
			if (student_id == (*((*courses_list)[course_index].data->theoretical_course->list_of_enrolled_students))[index].data->student->id)
			{
				return Error_code_t::ERROR_DUPLICATE_REGISTER;
			}
		}
		break;
	default:
		break;
	}

	Link::Data student(Link::classification_t::STUDENT);

	student.student->first_name = student_first_name;
	student.student->family_name = student_last_name;
	student.student->id = student_id;

	switch ((*courses_list)[course_index].link_classification)
	{
	case Link::classification_t::PRACTICAL_COURSE:
		(*courses_list)[course_index].data->practical_course->list_of_enrolled_students->addLink(student, Link::classification_t::STUDENT);
		(*courses_list)[course_index].data->practical_course->grades.push_back(GRADES_NOT_ENTERED);
		break;
	case Link::classification_t::THEORETICAL_COURSE:
		(*courses_list)[course_index].data->theoretical_course->list_of_enrolled_students->addLink(student, Link::classification_t::STUDENT);
		(*courses_list)[course_index].data->theoretical_course->grades.push_back(GRADES_NOT_ENTERED);
		break;
	default:
		break;
	}

	return Error_code_t::ERROR_OK;
}

Error_code_t Portal::unregisterCourse(size_t student_index, size_t course_index)
{
	if (student_index >= studentsListLength() || course_index >= coursesListLength())
	{
		return Error_code_t::ERROR_INVALID_INDEX;
	}

	std::string student_last_name, student_first_name, student_id;

	getStudent(student_index, student_first_name, student_last_name, student_id);

	switch ((*courses_list)[course_index].link_classification)
	{
	case Link::classification_t::PRACTICAL_COURSE:
		for (size_t index = 0; index < (*courses_list)[course_index].data->practical_course->list_of_enrolled_students->size(); index++)
		{
			if (student_id == (*((*courses_list)[course_index].data->practical_course->list_of_enrolled_students))[index].data->student->id &&
				student_last_name == (*((*courses_list)[course_index].data->practical_course->list_of_enrolled_students))[index].data->student->family_name &&
				student_first_name == (*((*courses_list)[course_index].data->practical_course->list_of_enrolled_students))[index].data->student->first_name)
			{
				(*courses_list)[course_index].data->practical_course->list_of_enrolled_students->removeLink(index);
				(*courses_list)[course_index].data->practical_course->grades.erase((*courses_list)[course_index].data->practical_course->grades.begin() + index);
				break;
			}
		}
		break;
	case Link::classification_t::THEORETICAL_COURSE:
		for (size_t index = 0; index < (*courses_list)[course_index].data->theoretical_course->list_of_enrolled_students->size(); index++)
		{
			if (student_id == (*((*courses_list)[course_index].data->theoretical_course->list_of_enrolled_students))[index].data->student->id &&
				student_last_name == (*((*courses_list)[course_index].data->theoretical_course->list_of_enrolled_students))[index].data->student->family_name &&
				student_first_name == (*((*courses_list)[course_index].data->theoretical_course->list_of_enrolled_students))[index].data->student->first_name)
			{
				(*courses_list)[course_index].data->theoretical_course->list_of_enrolled_students->removeLink(index);
				(*courses_list)[course_index].data->theoretical_course->grades.erase((*courses_list)[course_index].data->theoretical_course->grades.begin() + index);
				break;
			}
		}
		break;
	default:
		break;
	}

	return Error_code_t::ERROR_OK;
}

Error_code_t Portal::getCourseRegisterList(size_t course_index, size_t student_index, std::string &student_last_name, std::string &student_first_name, std::string &student_id)
{
	if(course_index >= coursesListLength() || student_index >= courseRegisterListLength(course_index))
	{
		return Error_code_t::ERROR_INVALID_INDEX;
	}

	switch ((*courses_list)[course_index].link_classification)
	{
	case Link::classification_t::PRACTICAL_COURSE:
		student_last_name = (*((*courses_list)[course_index].data->practical_course->list_of_enrolled_students))[student_index].data->student->family_name;
		student_first_name = (*((*courses_list)[course_index].data->practical_course->list_of_enrolled_students))[student_index].data->student->first_name;
		student_id = (*((*courses_list)[course_index].data->practical_course->list_of_enrolled_students))[student_index].data->student->id;
		break;
	case Link::classification_t::THEORETICAL_COURSE:
		student_last_name = (*((*courses_list)[course_index].data->theoretical_course->list_of_enrolled_students))[student_index].data->student->family_name;
		student_first_name = (*((*courses_list)[course_index].data->theoretical_course->list_of_enrolled_students))[student_index].data->student->first_name;
		student_id = (*((*courses_list)[course_index].data->theoretical_course->list_of_enrolled_students))[student_index].data->student->id;
		break;
	default:
		break;
	}

	return Error_code_t::ERROR_OK;
}

// student_index is the index of the register list for the course
Error_code_t Portal::addGrade(size_t course_index, size_t student_index, float grade)
{
	if(course_index >= coursesListLength() || student_index >= courseRegisterListLength(course_index))
	{
		return Error_code_t::ERROR_INVALID_INDEX;
	}

	if(grade > GRADES_MAXIMUM || grade < GRADES_MINIMUM)
	{
		return Error_code_t::ERROR_INVALID_GRADE;
	}

	switch ((*courses_list)[course_index].link_classification)
	{
	case Link::classification_t::PRACTICAL_COURSE:
		(*courses_list)[course_index].data->practical_course->grades[student_index] = grade;
		break;
	case Link::classification_t::THEORETICAL_COURSE:
		(*courses_list)[course_index].data->theoretical_course->grades[student_index] = grade;
		break;
	default:
		break;
	}

	return Error_code_t::ERROR_OK;
}

Error_code_t Portal::getGrade(size_t course_index, size_t registered_student_index, float &grade)
{
	if(course_index >= coursesListLength() || registered_student_index >= courseRegisterListLength(course_index))
	{
		return Error_code_t::ERROR_INVALID_INDEX;
	}

	grade = (*courses_list)[course_index].data->theoretical_course->grades[registered_student_index];

	return Error_code_t::ERROR_OK;
}

Error_code_t Portal::getStudentIndexFromRegisteredIndex(const size_t course_index, const size_t registered_student_index, size_t &student_index)
{
	std::string student_first_name, student_last_name, student_id, registered_student_first_name, registered_student_last_name, registered_student_id;

	getCourseRegisterList(course_index, registered_student_index, registered_student_last_name, registered_student_first_name, registered_student_id);

	for (size_t index = 0; index < portal.studentsListLength(); index++)
	{
		getStudent(index, student_first_name, student_last_name, student_id);

		if (student_first_name == registered_student_first_name && student_last_name == registered_student_last_name && student_id == registered_student_id)
		{
			student_index = index;

			return Error_code_t::ERROR_OK;
		}
	}

	return Error_code_t::ERROR_INVALID_INDEX;
}