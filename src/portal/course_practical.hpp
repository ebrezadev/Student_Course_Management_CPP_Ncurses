#ifndef __PRACTICAL_COURSE_H__
#define __PRACTICAL_COURSE_H__

#include "error.hpp"
#include <string>
#include "course.hpp"

class PracticalCourse : public Course
{
public:
	PracticalCourse(){};
	~PracticalCourse(){};
	std::string project_plan;

private:
};

#endif