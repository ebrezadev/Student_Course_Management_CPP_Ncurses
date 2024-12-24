#include "link.hpp"

Link::Link(classification_t link_classification) : link_classification(link_classification)
{
	data = new Data(link_classification);
	next = previous = nullptr;
};

Link::~Link()
{
	switch (link_classification)
	{
	case classification_t::PRACTICAL_COURSE:
		delete data->practical_course;
		break;
	case classification_t::THEORETICAL_COURSE:
		delete data->theoretical_course;
		break;
	case classification_t::STUDENT:
		delete data->student;
		break;
	default:
		break;
	}

	delete data;
};

Link::Data::Data(classification_t data_classification)
{
	switch (data_classification)
	{
	case classification_t::PRACTICAL_COURSE:
		practical_course = new PracticalCourse;
		break;
	case classification_t::THEORETICAL_COURSE:
		theoretical_course = new TheoreticalCourse;
		break;
	case classification_t::STUDENT:
		student = new Student;
		break;
	default:
		break;
	}
};

Link::Data::~Data()
{

};