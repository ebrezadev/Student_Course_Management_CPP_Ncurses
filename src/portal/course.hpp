#ifndef __COURSE_H__
#define __COURSE_H__

#include "error.hpp"
#include <string>
#include <vector>

class List;

class Course
{
public:
	Course() 
	{
		units = 0;
		list_of_enrolled_students = nullptr;
	};
	virtual ~Course() = 0;
	
	List *list_of_enrolled_students;
	std::vector<float> grades;
	std::string name;
	std::string id;
	int units;
	
protected:
};

#endif
