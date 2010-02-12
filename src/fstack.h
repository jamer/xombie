#ifndef _FSTACK_H_
#define _FSTACK_H_

#include <stdlib.h>

/* Fstack, a fast stack
 *  rough hack for a stack structure, couldn't get stl's stack to work
 *  something about a 'vtable'
 * really simple, designed to be used with pointers, no optimizations
 */

// I forgot why I wrote this... - Paul M 2009/03/19

template<class T> class Fstack
{
public:
	Fstack(int initbufsz);

	T top();
	T index(int i);
	int size();

	void push(T t);
	T pop();

private:
	T* data;
	unsigned int sz;
	unsigned int total;
};


template<class T> Fstack<T>::Fstack(int initbufsz)
{
	total = initbufsz;
	sz = 0;
	data = (T*)malloc(sizeof(T) * total);
}

template<class T> T Fstack<T>::top()
{
	if (sz == 0)
		return NULL;
	return data[sz - 1];
}

template<class T> T Fstack<T>::index(int i)
{
	if (i == 0 || i >= sz)
		return NULL;
	return data[i];
}

template<class T> int Fstack<T>::size()
{
	return sz;
}

template<class T> void Fstack<T>::push(T t)
{
	data[sz++] = t;
}

template<class T> T Fstack<T>::pop()
{
	if (sz == 0)
		return NULL;
	return data[(sz--) - 1];
}


#endif // _FSTACK_H_

