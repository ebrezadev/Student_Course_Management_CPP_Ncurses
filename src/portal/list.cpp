#include "list.hpp"

List::List(classification_t classification)
{
	list_classification = classification;

	head = tail = nullptr;

	counter = 0;
}

List::~List()
{
	
}

Error_code_t List::addLink(const Link::Data &data, Link::classification_t classification)
{
	if(list_classification == List::classification_t::COURSE_LIST && classification == Link::classification_t::STUDENT)
	{
		return Error_code_t::ERROR_CLASSIFICATION_MISMATCH;
	}

	if(list_classification == List::classification_t::STUDENT_LIST && classification != Link::classification_t::STUDENT)
	{
		return Error_code_t::ERROR_CLASSIFICATION_MISMATCH;
	}

	if (counter == 0)
	{
		head = new Link(classification);

		if (head == nullptr)
		{
			return Error_code_t::ERROR_MEMORY_ALLOCATION_LINK;
		}

		head->previous = nullptr;
		head->next = nullptr;
		head->link_classification = classification;

		switch (classification)
		{
		case Link::classification_t::PRACTICAL_COURSE:
			head->data->practical_course = data.practical_course;
			head->data->practical_course->list_of_enrolled_students = new List(List::classification_t::STUDENT_LIST);
			break;
		case Link::classification_t::THEORETICAL_COURSE:
			head->data->theoretical_course = data.theoretical_course;
			head->data->theoretical_course->list_of_enrolled_students = new List(List::classification_t::STUDENT_LIST);
			break;
		case Link::classification_t::STUDENT:
			head->data->student = data.student;
			break;
		default:
			break;
		}

		tail = head;
	}
	else
	{
		tail->next = new Link(classification);

		if (tail->next == nullptr)
		{
			return Error_code_t::ERROR_MEMORY_ALLOCATION_LINK;
		}

		tail->next->next = nullptr;
		tail->next->previous = tail;
		tail->next->link_classification = classification;

		tail = tail->next;

		switch (classification)
		{
		case Link::classification_t::PRACTICAL_COURSE:
			tail->data->practical_course = data.practical_course;
			tail->data->practical_course->list_of_enrolled_students = new List(List::classification_t::STUDENT_LIST);
			break;
		case Link::classification_t::THEORETICAL_COURSE:
			tail->data->theoretical_course = data.theoretical_course;	
			tail->data->theoretical_course->list_of_enrolled_students = new List(List::classification_t::STUDENT_LIST);
			break;
		case Link::classification_t::STUDENT:
			tail->data->student = data.student;
		default:
			break;
		}
	}

	counter++;

	return Error_code_t::ERROR_OK;
}

Error_code_t List::removeLink(size_t index)
{
	Link *link = head;

	if (link == nullptr)
	{
		return Error_code_t::ERROR_EMPTY_LIST;
	}

	for (size_t i = 0; i < index; i++)
	{
		if (link->next == nullptr)
		{
			return Error_code_t::ERROR_INVALID_INDEX;
		}

		link = link->next;
	}

	if (link->previous != nullptr)
	{
		link->previous->next = link->next;

		if (link == tail)
		{
			tail = link->previous;
		}
	}
	else
	{
		head = link->next;
	}

	if (link->next != nullptr)
	{
		link->next->previous = link->previous;

		if (link == head)
		{
			head = link->next;
		}
	}
	else
	{
		tail = link->previous;
	}

	delete link;
	counter--;

	return Error_code_t::ERROR_OK;
}

Error_code_t List::searchLinkID(const std::string &id, size_t &index)
{
	for (size_t i = 0; i < size(); i++)
	{
		switch ((*this)[i].link_classification)
		{
		case Link::classification_t::PRACTICAL_COURSE:
			if ((*this)[i].data->practical_course->id == id)
			{
				index = i;
				return Error_code_t::ERROR_OK;
			}
			break;
		case Link::classification_t::THEORETICAL_COURSE:
			if ((*this)[i].data->theoretical_course->id == id)
			{
				index = i;
				return Error_code_t::ERROR_OK;
			}
			break;
		case Link::classification_t::STUDENT:
			if ((*this)[i].data->student->id == id)
			{
				index = i;
				return Error_code_t::ERROR_OK;
			}
			break;
		default:
			break;
		}
	}

	return Error_code_t::ERROR_ID_NOT_FOUND;
}

size_t List::size()
{
	return counter;
}

Link &List::operator[](size_t index)
{
	Link *link;

	if (index < (size() / 2))
	{
		link = head;

		for (size_t i = 0; i < index; i++)
		{
			if (link == nullptr)
			{
				break;
			}

			link = link->next;
		}
	}
	else
	{
		link = tail;

		for (size_t i = 0; i < (size() - index - 1); i++)
		{
			if (link == nullptr)
			{
				break;
			}

			link = link->previous;
		}
	}

	return *link;
}