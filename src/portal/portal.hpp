#ifndef __PORTAL_H__
#define __PORTAL_H__

#include "list.hpp"

class Portal
{
public:
	Portal();
	~Portal();
	bool exists();
	void newPortal();
	void deletePortal();
	size_t studentsListLength();
	size_t coursesListLength();
	size_t courseRegisterListLength(size_t course_index);
	Error_code_t addStudent(const std::string &first_name, const std::string &last_name, const std::string &id);
	Error_code_t getStudent(size_t index, std::string &first_name, std::string &last_name, std::string &id);
	Error_code_t addCourse(Link::classification_t course_classification, const std::string &course_name, const std::string &course_id, const std::string &course_plan, int course_units);
	Error_code_t getCourse(size_t index, Link::classification_t &course_classification, std::string &course_name, std::string &course_id, std::string &course_plan, int &course_units);
	Error_code_t removeStudent(size_t index);
	Error_code_t removeCourse(size_t index);
	Error_code_t searchStudentByID(std::string id, size_t &index);
	Error_code_t searchCourseByID(std::string id, size_t &index);
	Error_code_t registerCourse(size_t student_index, size_t course_index);
	Error_code_t unregisterCourse(size_t student_index, size_t course_index);
	Error_code_t getCourseRegisterList(size_t course_index, size_t student_index, std::string &student_last_name, std::string &student_first_name, std::string &student_id);
	Error_code_t addGrade(size_t course_index, size_t student_index, float grade);
	Error_code_t getGrade(size_t course_index, size_t registered_student_index, float &grade);
	Error_code_t getStudentIndexFromRegisteredIndex(const size_t course_index, const size_t registered_student_index, size_t &student_index);

private:
	List *students_list;
	List *courses_list;
	bool exists_b;
};

extern Portal portal;

#endif