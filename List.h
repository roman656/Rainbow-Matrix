#ifndef _LIST_H_
#define _LIST_H_

template <class T>
class List
{
private:
	int length;
	typedef struct ListElem
	{
		T data;
		struct ListElem* next;
	} ListElement;
	ListElement* begin;
	ListElement* end;
public:
	List();
	
	~List();
};

#endif // _LIST_H_