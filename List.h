#ifndef _LIST_H_
#define _LIST_H_

#include "ListException.h"

template <class T>
class List
{
private:
	int length;
	struct ListElement
	{
		T data;
		ListElement* next;
	};
	ListElement* begin;
	ListElement* end;
public:
	List();
	void push_back(T value);
	void clear();
	void erase(int index);
	int getLength();
	T& operator[] (int index);
	~List();
};

template <class T>
List<T>::List()
{
	length = 0;
	begin = nullptr;
	end = nullptr;
}

template <class T>
int List<T>::getLength()
{
	return length;
}

template <class T>
void List<T>::push_back(T value)
{
	ListElement* temp = nullptr;
	try
	{
		temp = new ListElement;
	}
	catch (bad_alloc& allocEx)
	{
		throw ListException("List -> push_back() -> Memory error\n");
	}
	temp->next = nullptr;
	temp->data = value;
	if (length == 0)
	{
		begin = temp;
		end = temp;
	}
	else
	{
		end->next = temp;
		end = end->next;
	}
	length++;
}

template <class T>
void List<T>::clear()
{
	int limit = length;
	for (int i = 0; i < limit; i++)
	{
		erase(0);
	}
}

template <class T>
void List<T>::erase(int index)
{
	if ((index >= length) || (index < 0))
	{
		throw ListException("List -> erase() -> Wrong index.\n");
	}
	ListElement* curElem = begin;
	ListElement* tempPrev = begin;
	int counter = index;
	while (counter != 0)
	{
		curElem = curElem->next;
		counter--;
	}
	while ((tempPrev->next != curElem) && (index != 0))
	{
		tempPrev = tempPrev->next;
	}
	if (curElem == begin)
	{
		if (length > 1)
		{
			begin = tempPrev->next;
		}
		else
		{
			begin = nullptr;
			end = nullptr;
		}
	}
	else if (curElem == end)
	{
		tempPrev->next = nullptr;
		end = tempPrev;
	}
	else
	{
		tempPrev->next = curElem->next;
	}
	delete curElem;
	length--;
}

template <class T>
T& List<T>::operator[] (int index)
{
	if ((index >= length) || (index < 0))
	{
		throw ListException("List -> Wrong index.\n");
	}
	ListElement* temp = begin;
	while (index != 0)
	{
		temp = temp->next;
		index--;
	}
	return temp->data;
}

template <class T>
List<T>::~List()
{
	clear();
}

#endif // _LIST_H_