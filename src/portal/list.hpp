#ifndef __LIST_H__
#define __LIST_H__

#include <string>
#include "link.hpp"
#include "error.hpp"

class List
{
public:
	enum class classification_t
	{
		COURSE_LIST,
		STUDENT_LIST
	};

	List() = delete;
	List(classification_t classification);
	~List();
	Error_code_t addLink(const Link::Data &data, Link::classification_t classification);
	Error_code_t removeLink(size_t index);
	size_t size();
	Error_code_t searchLinkID(const std::string &id, size_t &index);
	Link &operator[](size_t index);

private:
	Link *head;
	Link *tail;
	int counter;
	classification_t list_classification;
};

#endif