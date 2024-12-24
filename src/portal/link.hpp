#ifndef __LINK_H__
#define __LINK_H__

#include <string>
#include "course_theoretical.hpp"
#include "course_practical.hpp"
#include "student.hpp"

class Link
{
public:
	enum class classification_t
	{
		THEORETICAL_COURSE = 0,
		PRACTICAL_COURSE,
		STUDENT
	};

	union Data
	{
		Data(classification_t data_classification);
		~Data();

		PracticalCourse *practical_course;
		TheoreticalCourse *theoretical_course;
		Student *student;
	};

	Link() = delete;
	Link(classification_t link_classification);
	~Link();

	classification_t link_classification;
	Data *data;
	Link *next;
	Link *previous;

private:
};

#endif