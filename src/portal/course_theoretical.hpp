#ifndef __THEORETICAL_COURSE_H__
#define __THEORETICAL_COURSE_H__

#include "error.hpp"
#include <string>
#include "course.hpp"

class TheoreticalCourse : public Course
{
public:
	TheoreticalCourse(){};
	~TheoreticalCourse(){};
	std::string lesson_plan;

private:
};

#endif