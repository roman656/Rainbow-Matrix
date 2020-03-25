#ifndef _LIST_H_
#define _LIST_H_

#include "ListException.h"

///Шаблонный класс списка (односвязного, не кольцевого).
template <class T>
class List
{
private:
	int length;				//Размер списка (Количество элементов в списке).
	struct ListElement			//Структура элемента списка.
	{
		T data;				//Полезная нагрузка элемента списка.
		ListElement* next;		//Указатель на следующий элемент списка (или NULL, если это последний элемент списка).
	};
	ListElement* begin;			//Указатель на первый элемент списка.
	ListElement* end;			//Указатель на последний элемент списка.
public:
	///Конструктор.
	List();

	///Метод, добавляющий новый элемент в конец списка.
	///Входные параметры:
	///\param T value - полезная нагрузка элемента списка.
	void push_back(T value);

	///Метод, очищающий весь список.
	///Удаляются все элементы списка, поэтому, если в качестве полезной нагрузки выступает указатель на динамически выделенную область памяти,
	///необходимо до вызова данного метода освободить эту область.
	void clear();

	///Метод, удаляющий определенный элемент списка.
	///Входные параметры:
	///\param unsigned long index - индекс удаляемого элемента списка.
	void erase(int index);

	///Геттер, возвращающий текущий размер списка.
	int getLength();

	///Перегрузка оператора []. Реализует доступ к полезной нагрузке определенного элемента списка.
	///Входные параметры:
	///\param unsigned long index - индекс элемента списка.
	T& operator[] (int index);

	///Деструктор.
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
	catch (std::bad_alloc& allocEx)
	{
		throw ListException("List -> push_back() -> Memory error.\n");
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
	ListElement* currentElement = begin;
	ListElement* prevElement = begin;
	int counter = index;
	while (counter != 0)
	{
		currentElement = currentElement->next;
		counter--;
	}
	while ((prevElement->next != currentElement) && (index != 0))
	{
		prevElement = prevElement->next;
	}
	if (currentElement == begin)
	{
		if (length > 1)
		{
			begin = prevElement->next;
		}
		else
		{
			begin = nullptr;
			end = nullptr;
		}
	}
	else if (currentElement == end)
	{
		prevElement->next = nullptr;
		end = prevElement;
	}
	else
	{
		prevElement->next = currentElement->next;
	}
	delete currentElement;
	length--;
}

template <class T>
T& List<T>::operator[] (int index)
{
	if ((index >= length) || (index < 0))
	{
		throw ListException("List -> operator [] -> Wrong index.\n");
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
